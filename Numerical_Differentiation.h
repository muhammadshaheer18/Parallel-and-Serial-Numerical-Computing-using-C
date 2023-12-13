#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

typedef double (*Function)(double);

double example_function(double x) {
    return x * x;
}

int forward_printing_done = 0;
int backward_printing_done = 0;
int user_input;

void print_results_and_write_to_file_serial(double x[], Function func, double fdf[], int n, const char *output_filename) {
    
    printf("|   x   |   f(x)   | df(x)/dx |\n");
    printf("|-------|----------|----------|\n");
    for (int i = 0; i < n; i++) {
        printf("| %.3f | %.6f | %.5f |\n", x[i], func(x[i]), fdf[i]);
    } 

    FILE *output_file = fopen(output_filename, "w");
    if (!output_file) {
        perror("Error opening output file");
        exit(EXIT_FAILURE);
    }

    fprintf(output_file, "x     | f(x)     | f'(x)\n__________________________\n");
    for (int i = 0; i < n; i++) {
        fprintf(output_file, "%.3f | %.6f | %.5f\n", x[i], func(x[i]), fdf[i]);
    }

    fclose(output_file);
}

void print_results_and_write_to_file(double x[], Function func, double fdf[], int n, const char *output_filename) {
    
    FILE *output_file = fopen(output_filename, "w");
    if (!output_file) {
        perror("Error opening output file");
        exit(EXIT_FAILURE);
    }

    fprintf(output_file, "x     | f(x)     | f'(x)\n__________________________\n");
    for (int i = 0; i < n; i++) {
        fprintf(output_file, "%.3f | %.6f | %.5f\n", x[i], func(x[i]), fdf[i]);
    }

    fclose(output_file);
}

void backward_diff(double x[], Function func, int n, int num_threads, const char *output_filename) {
    double h = x[1] - x[0];

    double fdf[n];
    fdf[0] = (func(x[1]) - func(x[0])) / h;

    #pragma omp parallel for num_threads(num_threads)
    for (int i = 1; i < n; i++) {
        fdf[i] = (func(x[i]) - func(x[i - 1])) / h;
    }

    #pragma omp critical
    {
        if (!backward_printing_done) {
            print_results_and_write_to_file(x, func, fdf, n, output_filename);
            backward_printing_done = 1;
        }
    }
}

int backward_main() {
    do {
        printf("Enter the number of values of x you want to read (between 2 and 100000): ");
        scanf("%d", &user_input);

        if (user_input < 2 || user_input > 100000) {
            printf("Invalid input. Please enter a value between 2 and 100000.\n");
        }

    } while (user_input < 2 || user_input > 100000);

    const char *filename = "num_diff_dataset.csv";
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    int total_lines = 0;
    char line[256];

    // Count the lines without changing the file pointer position
    while (fgets(line, sizeof(line), file) != NULL && total_lines < user_input) {
        total_lines++;
    }

    fseek(file, 0, SEEK_SET);

    double *x = malloc(total_lines * sizeof(double));

    for (int i = 0; i < total_lines; i++) {
        fscanf(file, "%lf\n", &x[i]);
    }

    fclose(file);

    const char *output_filename = "backward_output.csv";

    double start_time = omp_get_wtime();

    #pragma omp parallel
    #pragma omp single
    {
        backward_diff(x, example_function, total_lines, 1, output_filename);
    }

    double end_time = omp_get_wtime();
    printf("\n");
    printf("Execution Time (1 Thread): %f seconds\n\n", end_time - start_time);

    for (int num_threads = 2; num_threads <= 10; num_threads += 2) {
        start_time = omp_get_wtime();
        backward_diff(x, example_function, total_lines, num_threads, output_filename);
        end_time = omp_get_wtime();
        printf("Execution Time (%d Threads): %f seconds\n", num_threads, end_time - start_time);
        printf("\n");
    }

    free(x);

    return 0;
}
















void forward_diff(double x[], Function func, int n, int num_threads, const char *output_filename) {
    double h = x[1] - x[0];

    double fdf[n];
    fdf[n - 1] = (func(x[n - 1]) - func(x[n - 2])) / h;

    #pragma omp parallel for num_threads(num_threads)
    for (int i = 0; i < n - 1; i++) {
        fdf[i] = (func(x[i + 1]) - func(x[i])) / h;
    }

    #pragma omp critical
    {
        if (!forward_printing_done) {
            print_results_and_write_to_file(x, func, fdf, n, output_filename);
            forward_printing_done = 1;
        }
    }
}

int forward_main() {
    
    do {
        printf("Enter the number of values of x to read (2-100000): ");
        scanf("%d", &user_input);
    } while (user_input < 2 || user_input > 100000);

    const char *filename = "num_diff_dataset.csv";
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    int n = 0;
    char line[256];

    for (int i = 0; i < user_input; i++) {
        if (fgets(line, sizeof(line), file) == NULL) {
            fprintf(stderr, "Error: Not enough lines in the file.\n");
            fclose(file);
            return 1;
        }
        n++;
    }

    fseek(file, 0, SEEK_SET);

    double *x = malloc(n * sizeof(double));

    for (int i = 0; i < n; i++) {
        fscanf(file, "%lf\n", &x[i]);
    }

    fclose(file);

    const char *output_filename = "forward_output.csv";

    double start_time = omp_get_wtime();

    #pragma omp parallel
    #pragma omp single
    {
        printf("Function and its Derivative (1 Thread):\n");
        forward_diff(x, example_function, n, 1, output_filename);
    }

    double end_time = omp_get_wtime();
    printf("\nExecution Time (1 Thread): %f seconds\n\n", end_time - start_time);

    for (int num_threads = 2; num_threads <= 10; num_threads += 2) {
        start_time = omp_get_wtime();
        forward_diff(x, example_function, n, num_threads, output_filename);
        end_time = omp_get_wtime();
        printf("Execution Time (%d Threads): %f seconds\n", num_threads, end_time - start_time);
        printf("\n");
    }

    free(x);

    return 0;
}















double forward_serial_function(double x[], Function func, int n, const char *output_filename){
    double h = x[1] - x[0];
    double fdf[n];
    fdf[n - 1] = (func(x[n - 1]) - func(x[n - 2])) / h;

    for (int i = 0; i < n - 1; i++) {
        fdf[i] = (func(x[i + 1]) - func(x[i])) / h;
    }
    print_results_and_write_to_file_serial(x, example_function, fdf, n, output_filename);
}
double forward_serial() {

    const char *filename = "num_diff_dataset.csv";
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    int n = 0;
    char line[256];

    for (int i = 0; i < user_input; i++) {
        if (fgets(line, sizeof(line), file) == NULL) {
            fprintf(stderr, "Error: Not enough lines in the file.\n");
            fclose(file);
            return 1;
        }
        n++;
    }

    fseek(file, 0, SEEK_SET);

    double *x = malloc(n * sizeof(double));

    for (int i = 0; i < n; i++) {
        fscanf(file, "%lf\n", &x[i]);
    }

    fclose(file);

    const char *output_filename = "forward_output.csv";
    double start_time = omp_get_wtime();
    forward_serial_function(x, example_function, n, output_filename);
    double end_time = omp_get_wtime();

    return end_time - start_time;
}
















double backward_serial_function(double x[], Function func, int n, const char *output_filename){
    double h = x[1] - x[0];
    double fdf[n];
    fdf[0] = (func(x[1]) - func(x[0])) / h;

    for (int i = 1; i < n; i++) {
        fdf[i] = (func(x[i]) - func(x[i - 1])) / h;
    }
    print_results_and_write_to_file_serial(x, example_function, fdf, n, output_filename);
}
double backward_serial() {

    const char *filename = "num_diff_dataset.csv";
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    int n = 0;
    char line[256];

    for (int i = 0; i < user_input; i++) {
        if (fgets(line, sizeof(line), file) == NULL) {
            fprintf(stderr, "Error: Not enough lines in the file.\n");
            fclose(file);
            return 1;
        }
        n++;
    }

    fseek(file, 0, SEEK_SET);

    double *x = malloc(n * sizeof(double));

    for (int i = 0; i < n; i++) {
        fscanf(file, "%lf\n", &x[i]);
    }

    fclose(file);

    const char *output_filename = "backward_output.csv";

    

    double start_time = omp_get_wtime();
    backward_serial_function(x, example_function, n, output_filename);
    double end_time = omp_get_wtime();

    return end_time - start_time;
}
