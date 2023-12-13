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