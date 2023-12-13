#include<stdio.h>
#include<omp.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>
#include<sys/wait.h>
#include<unistd.h>
#include "Numerical_Differentiation.h"
#define MAX_POINTS 10000
#define clear() printf("\033[H\033[J")

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



void dolittle_serial(double **A, int n) {
    for (int k = 0; k < n - 1; ++k) {
        for (int i = k + 1; i < n; ++i) {
            double factor = A[i][k] / A[k][k];
            for (int j = k; j < n; ++j) {
                A[i][j] -= factor * A[k][j];
            }
        }
    }
}

void dolittle_parallel(double **A, int n) {
    #pragma omp parallel for
    for (int k = 0; k < n - 1; ++k) {
        for (int i = k + 1; i < n; ++i) {
            double factor = A[i][k] / A[k][k];
            for (int j = k; j < n; ++j) {
                A[i][j] -= factor * A[k][j];
            }
        }
    }
}
void createRandomMatrixFile(const char *filename, int n) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("\t\t\t\t\tError opening file.\n");
        return;
    }

    srand(time(0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            fprintf(file, "%lf ", (double)rand() / RAND_MAX); // Writing random numbers to the file
        }
        fprintf(file, "\n");
    }

    fclose(file);
}
int dolittle_main() {
    int num_threads_list[] = {4, 8, 16, 32, 64};
    int num_threads_count = sizeof(num_threads_list) / sizeof(num_threads_list[0]);

    FILE *file;
    file = fopen("matrix.txt", "r");
    if (file == NULL) {
        printf("\t\t\t\t\tError opening file.\n");
        return -1;
    }

    int matrix_sizes[] = {64, 128, 256, 512, 1024, 2048, 4096};
    int matrix_count = sizeof(matrix_sizes) / sizeof(matrix_sizes[0]);
    
    printf("\t\t\t\t\t+--------------+-------------+------------+----------------------------------------------+\n");
    printf("\t\t\t\t\t|    Matrix Size             |        ThreadsSerial Time     |          Parallel Time    |\n");
    printf("\t\t\t\t\t+--------------+-------------+------------+----------------------------------------------+\n");

    for (int i = 0; i < matrix_count; ++i) {
        int n = matrix_sizes[i];
	 createRandomMatrixFile("matrix.txt",matrix_sizes[i]);
        double **a = (double **)malloc(n * sizeof(double *));
        for (int i = 0; i < n; ++i) {
            a[i] = (double *)malloc(n * sizeof(double));
        }

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                fscanf(file, "%lf", &a[i][j]);
            }
        }

        printf("\n\t\t\t\t\tSize of Matrix: %d\n\n", n);
        
        double t1_serial = omp_get_wtime();
        dolittle_serial(a, n);
        double t2_serial = omp_get_wtime();
        
        for (int j = 0; j < num_threads_count; ++j) {
            int num_threads = num_threads_list[j];

  

            double t1_parallel = omp_get_wtime();
            dolittle_parallel(a, n);
            double t2_parallel = omp_get_wtime();

            printf("\t\t\t\t\t%d\t\t\t\t%0.6f\t\t\t\t%0.6f\n", num_threads, t2_serial - t1_serial, t2_parallel - t1_parallel);
        }

        for (int i = 0; i < n; ++i) {
            free(a[i]);
        }
        free(a);
    }

    fclose(file);
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void crout_serial(double **A, double **L, double **U, int n) {
    for (int j = 0; j < n; ++j) {
        U[j][j] = 1.0;
        for (int i = j; i < n; ++i) {
            double s1 = 0.0;
            for (int k = 0; k < j; ++k) {
                s1 += L[i][k] * U[k][j];
            }
            L[i][j] = A[i][j] - s1;
        }

        for (int i = j + 1; i < n; ++i) {
            double s2 = 0.0;
            for (int k = 0; k < j; ++k) {
                s2 += L[j][k] * U[k][i];
            }
            if (L[j][j] == 0) {
                printf("\t\t\t\t\tCrout decomposition not possible without row exchange\n");
                exit(1);
            } else {
                U[j][i] = (A[j][i] - s2) / L[j][j];
            }
        }
    }
}

void crout_parallel(double **A, double **L, double **U, int n) {
    #pragma omp parallel for schedule(dynamic)
    for (int j = 0; j < n; ++j) {
        U[j][j] = 1.0;
        for (int i = j; i < n; ++i) {
            double s1 = 0.0;
            #pragma omp parallel for reduction(+:s1)
            for (int k = 0; k < j; ++k) {
                s1 += L[i][k] * U[k][j];
            }
            L[i][j] = A[i][j] - s1;
        }

        #pragma omp parallel for schedule(dynamic)
        for (int i = j + 1; i < n; ++i) {
            double s2 = 0.0;
            #pragma omp parallel for reduction(+:s2)
            for (int k = 0; k < j; ++k) {
                s2 += L[j][k] * U[k][i];
            }
            if (L[j][j] == 0) {
                printf("\t\t\t\t\tCrout decomposition not possible without row exchange\n");
                exit(1);
            } else {
                U[j][i] = (A[j][i] - s2) / L[j][j];
            }
        }
    }
}

int crout_main() {
    int num_threads_list[] = {4, 8, 16, 32, 64};
    int num_threads_count = sizeof(num_threads_list) / sizeof(num_threads_list[0]);

    int matrix_sizes[] = {64, 128, 256, 512, 1024, 2048, 4096};
    int matrix_count = sizeof(matrix_sizes) / sizeof(matrix_sizes[0]);

    FILE *file;
    file = fopen("matrix.txt", "r");
    if (file == NULL) {
        printf("\t\t\t\t\tError opening file.\n");
        return -1;
    }

    printf("\t\t\t\t\t+--------------+-------------+------------+--------------+---------------------------------+\n");
    printf("\t\t\t\t\t| # of processes               Serial Time                           Parallel Time         |\n");
    printf("\t\t\t\t\t+--------------+-------------+------------+--------------+---------------------------------+\n");

    for (int i = 0; i < matrix_count; ++i) {
        int n = matrix_sizes[i];
        double **a = (double **)malloc(n * sizeof(double *));
        for (int i = 0; i < n; ++i) {
            a[i] = (double *)malloc(n * sizeof(double));
        }

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                fscanf(file, "%lf", &a[i][j]);
            }
        }

        printf("\n\t\t\t\t\tSize of Matrix: %d\n\n", n);

        double t1_serial = omp_get_wtime();
        crout_serial(a, a, a, n);
        double t2_serial = omp_get_wtime();
        
        for (int j = 0; j < num_threads_count; ++j) {
            int num_threads = num_threads_list[j];
            omp_set_num_threads(num_threads);

            double t1_parallel = omp_get_wtime();
            crout_parallel(a, a, a, n);
            double t2_parallel = omp_get_wtime();

            printf("\t\t\t\t\t%d\t\t\t\t%0.6fs\t\t\t\t%0.6fs\n", num_threads, t2_serial - t1_serial, t2_parallel - t1_parallel);
        }

        for (int i = 0; i < n; ++i) {
            free(a[i]);
        }
        free(a);
    }

    fclose(file);
    return 0;
}

int main()
{	
	int c;
	printf("\n\n\n\t\t\t\t\t-----------------------------------------------------");	
	printf("\n\n\t\t\t\t\tWelcome to Numerical Integration Code!");
	printf("\n\n\t\t\t\t\t1. Choose Methods");
	printf("\n\n\t\t\t\t\t2. Exit"); 
    printf("\n\n\t\t\t\t\tEnter choice: ");
	scanf("\t\t\t\t\t%d",&c);
	printf("\n\t\t\t\t\t-----------------------------------------------------\n\n\n");
	
	
	if(c==1){
	block:
		int c2;
		block2:
		printf("\t\t\t\t\t1. Numerical Differentation");
		printf("\n\n\t\t\t\t\t2. Numerical Integration");
		printf("\n\n\t\t\t\t\t3. Linear Matrix Systems");
		printf("\n\n\t\t\t\t\t4. Exit");
		printf("\n\n\t\t\t\t\tEnter choice: ");
		scanf("%d",&c2);
		
		if(c2==1){
			int c3;
			printf("\n\t\t\t\t\t1. Forward Difference Method");
			printf("\n\n\t\t\t\t\t2. Backward Difference Method\n");
			printf("\n\t\t\t\t\t3. Go back");
            printf("\n\n\t\t\t\t\tEnter choice: ");
			scanf("%d",&c3);
			if(c3==1){
			            forward_main();
			            sleep(8);
			            printf("\n\n\n\n\n");
			            goto block2;
			}
			if(c3==2){
				    backward_main();
				    sleep(8);
				    printf("\n\n\n\n\n");
				    goto block2;
			}
            if(c3 == 3) {
                clear();
                goto block2;
            }
			else{
				goto block2;
			}
			
		}
		if(c2==2){
			int c3;
			printf("\n\t\t\t\t\t1. Composite Simpsons 1/3rd Rule");
		        printf("\n\n\t\t\t\t\t2. Composite Trapezoidal rule\n");
		        scanf("%d",&c3);
		        if(c3==1){
		        	     simpsons_main();
		        	     sleep(8);
		        	     printf("\n\n\n\n\n");
		        	     goto block2;
		        }
		        if(c3==2){
		        	     trapezoidal_main();
		        	     sleep(8);
		        	     printf("\n\n\n\n\n");
		        	     goto block2;
		        }
		        else{
		        	goto block2;
		        }
		}
		if(c2==3){
			int c3;
			printf("\n\t\t\t\t\t1. LU using Dolittle approach");
			printf("\n\n\t\t\t\t\t2. LU using Crouts approach\n");
			scanf("%d",&c3);
		        if(c3==1){
		        	     dolittle_main();
		        	     sleep(8);
		        	     printf("\n\n\n\n\n");
		        	     goto block2;
		        }
		        if(c3==2){
		        	     crout_main();
		        	     sleep(8);
		        	     printf("\n\n\n\n\n");
		        	     goto block2;
		        }
		        else{
		        	goto block2;
		        }
		}
		else if(c2==4){
			return 0;
		}
	}
	else{
	
		return 0;
	}
}