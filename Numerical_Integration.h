void print_results_table(double serial_result, double parallel_result, double serial_time, double parallel_time, int num_threads) {
    printf("\t\t\t\t\t+------------------+------------------+------------------+------------------+\n");
    printf("\t\t\t\t\t|   Threads | Serial Result | Parallel Result | Serial Time | Parallel Time |\n");
    printf("\t\t\t\t\t+------------------+------------------+------------------+------------------+\n");
    printf("\t\t\t\t\t|   %2d     |  %12lf        |  %13lf          | %12.6f (s)  |  %13.6f (s)   |\n", num_threads, serial_result, parallel_result, serial_time, parallel_time);
    printf("\t\t\t\t\t+------------------+------------------+------------------+------------------+\n");
}

double f(double x) {
    return 2 / (pow(x, 2) + 4);
}

double comp_simpsons_serial(double (*func)(double), double a, double b, int n) {
    double h = (b - a) / n;
    double x[n + 1];
    double y[n + 1];

    for (int i = 0; i <= n; ++i) {
        x[i] = a + i * h;
        y[i] = func(x[i]);
    }

    double s1 = 0.0;
    double s2 = 0.0;

    for (int i = 1; i < n; i += 2) {
        s1 += y[i];
    }
    for (int i = 2; i < n - 1; i += 2) {
        s2 += y[i];
    }

    double ans = h / 3 * (y[0] + 4 * s1 + 2 * s2 + y[n]);

    return ans;
}
double comp_simpsons_parallel(double (*func)(double), double a, double b, int n, int num_threads) {
    double h = (b - a) / n;
    double x[n + 1];
    double y[n + 1];
    double s1 = 0.0;
    double s2 = 0.0;

    #pragma omp parallel for num_threads(num_threads) reduction(+:s1, s2)
    for (int i = 0; i <= n; ++i) {
        x[i] = a + i * h;
        y[i] = func(x[i]);
    }

    #pragma omp parallel for num_threads(num_threads) reduction(+:s1)
    for (int i = 1; i < n; i += 2) {
        s1 += y[i];
    }

    #pragma omp parallel for num_threads(num_threads) reduction(+:s2)
    for (int i = 2; i < n - 1; i += 2) {
        s2 += y[i];
    }

    double ans = h / 3 * (y[0] + 4 * s1 + 2 * s2 + y[n]);

    return ans;
}

int simpsons_main() {

    int num_threads_list[] = {4, 8, 16, 32, 64};
    int num_threads_count = sizeof(num_threads_list) / sizeof(num_threads_list[0]);
    int a,b,n;
    double h;
    printf("\t\t\t\t\t+------------------+------------------+------------------+--------------------------+\n");
    printf("\t\t\t\t\t|                Simpson's 1/3  Rule - Parallel Execution Performance               |\n");
    printf("\t\t\t\t\t+------------------+------------------+------------------+--------------------------+\n");
    printf("\t\t\t\t\tEnter Lower limit: "); 
    scanf(" %d",&a);
    printf("\t\t\t\t\tEnter Upper limit: "); 
    scanf(" %d",&b);
    printf("\t\t\t\t\tEnter Number of Intervals: "); 
    scanf("%d",&n);
    h= (b - a) / n;
    for (int i = 0; i < num_threads_count; ++i) {
        double t1 = omp_get_wtime();
        double serial_result = comp_simpsons_serial(f, a, b, n);
        double t2 = omp_get_wtime();

        double t3 = omp_get_wtime();
        double parallel_result = comp_simpsons_parallel(f, a, b, n, num_threads_list[i]);
        double t4 = omp_get_wtime();

        printf("\t\t\t\t\tParallel Execution with %2d Threads:\n", num_threads_list[i]);
        print_results_table(serial_result, parallel_result, t2 - t1, t4 - t3, num_threads_list[i]);
    }

    return 0;
}


double comp_trapezoidal_parallel(double (*func)(double), double a, double b, int n, int num_threads) {
    double h = (b - a) / n;
    double x[n + 1];
    double y[n + 1];
    double s = 0.0;

    #pragma omp parallel for num_threads(num_threads) reduction(+:s)
    for (int i = 0; i <= n; ++i) {
        x[i] = a + i * h;
        y[i] = func(x[i]);

        if (i > 0 && i < n) {
            if (i % 2 == 0) {
                s += 2 * y[i];
            } else {
                s += 4 * y[i];
            }
        }
    }

    double ans = (h / 3) * (y[0] + s + y[n]);

    return ans;
}


double comp_trapezoidal_serial(double (*func)(double), double a, double b, int n) {
    double h = (b - a) / n;
    double x[n + 1];
    double y[n + 1];

    for (int i = 0; i <= n; ++i) {
        x[i] = a + i * h;
        y[i] = func(x[i]);
    }

    double s = 0.0;

    for (int i = 1; i < n; ++i) {
        s += y[i];
    }

    double ans = h / 2 * (y[0] + 2 * s + y[n]);

    return ans;
}

int trapezoidal_main(){

    int num_threads_list[] = {4, 8, 16, 32, 64};
    int num_threads_count = sizeof(num_threads_list) / sizeof(num_threads_list[0]);
    int a,b,n;
    double h;
    printf("\t\t\t\t\t+------------------+------------------+------------------+----------------------------+\n");
    printf("\t\t\t\t\t|                  Trapezoidal Rule - Parallel Execution Performance                  |\n");
    printf("\t\t\t\t\t+------------------+------------------+------------------+----------------------------+\n");
    printf("\t\t\t\t\tEnter Lower limit: "); 
    scanf(" %d",&a);
    printf("\t\t\t\t\tEnter Upper limit: "); 
    scanf(" %d",&b);
    printf("\t\t\t\t\tEnter Number of Intervals: "); 
    scanf("%d",&n);
    h= (b - a) / n;
    for (int i = 0; i < num_threads_count; ++i) {
        double t1 = omp_get_wtime();
        double serial_result = comp_trapezoidal_serial(f, a, b, n);
        double t2 = omp_get_wtime();

        double t3 = omp_get_wtime();
        double parallel_result = comp_trapezoidal_parallel(f, a, b, n, num_threads_list[i]);
        double t4 = omp_get_wtime();

        printf("\t\t\t\t\tParallel Execution with %2d Threads:\n", num_threads_list[i]);
        print_results_table(serial_result, parallel_result, t2 - t1, t4 - t3, num_threads_list[i]);
    }

    return 0;
}
