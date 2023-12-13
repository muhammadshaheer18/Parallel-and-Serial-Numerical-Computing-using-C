#include<stdio.h>
#include<omp.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>
#include<sys/wait.h>
#include<unistd.h>
#include "Numerical_Differentiation.h"
#include "Linear_Matrix_Systems.h"
#include "Numerical_Integration.h"
#define MAX_POINTS 10000
#define clear() printf("\033[H\033[J")

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
	printf("\n\t\t\t\t\t-----------------------------------------------------\n\n\n");
		
	// 	if(c2==1){
	// 		int c3;
	// 		printf("\n\t\t\t\t\t1. Forward Difference Method");
	// 		printf("\n\n\t\t\t\t\t2. Backward Difference Method\n");
	// 		printf("\n\t\t\t\t\t3. Go back");
    //         printf("\n\n\t\t\t\t\tEnter choice: ");
	// 		scanf("%d",&c3);
	// printf("\n\t\t\t\t\t-----------------------------------------------------\n\n\n");
	// 		if(c3==1){
	// 		            forward_main();
	// 		            sleep(8);
	// 		            printf("\n\n\n\n\n");
	// 		            goto block2;
	// 		}
	// 		if(c3==2){
	// 			    backward_main();
	// 			    sleep(8);
	// 			    printf("\n\n\n\n\n");
	// 			    goto block2;
	// 		}
    //         if(c3 == 3) {
    //             clear();
    //             goto block2;
    //         }
	// 		else{
	// 			goto block2;
	// 		}
			
	// 	}
	// Inside the block2 loop where numerical differentiation is selected
if (c2 == 1) {
    int c3;
    printf("\n\t\t\t\t\t1. Forward Difference Method");
    printf("\n\n\t\t\t\t\t2. Backward Difference Method\n");
    printf("\n\t\t\t\t\t3. Go back");
    printf("\n\n\t\t\t\t\tEnter choice: ");
    scanf("%d", &c3);
    printf("\n\t\t\t\t\t-----------------------------------------------------\n\n\n");

    if (c3 == 1) {
        // Display the serial computation time for forward difference
        forward_main();
        double serial_time_forward = forward_serial();
        printf("\nSerial Computation Time (Forward Difference): %f seconds\n\n", serial_time_forward);

        sleep(8);
        printf("\n\n\n\n\n");
        goto block2;
    }
    if (c3 == 2) {
        // Display the serial computation time for backward difference
        backward_main();
        double serial_time_backward = backward_serial();
        printf("\nSerial Computation Time (Backward Difference): %f seconds\n\n", serial_time_backward);

        sleep(8);
        printf("\n\n\n\n\n");
        goto block2;
    }
    if (c3 == 3) {
        clear();
        goto block2;
    } else {
        goto block2;
    }
}
		if(c2==2){
			int c3;
			printf("\n\t\t\t\t\t1. Composite Simpsons 1/3rd Rule");
		        printf("\n\n\t\t\t\t\t2. Composite Trapezoidal rule\n");
				printf("\n\t\t\t\t\t3. Go back");
		        printf("\n\n\t\t\t\t\tEnter choice: ");
			scanf("%d",&c3);
	printf("\n\t\t\t\t\t-----------------------------------------------------\n\n\n");
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
				if(c3 == 3) {
                clear();
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
			printf("\n\t\t\t\t\t3. Go back");
			printf("\n\n\t\t\t\t\tEnter choice: ");
			scanf("%d",&c3);
	printf("\n\t\t\t\t\t-----------------------------------------------------\n\n\n");
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
				if(c3 == 3) {
                clear();
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