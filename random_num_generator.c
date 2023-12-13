#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    const char *filename = "test.csv";
    FILE *file = fopen(filename, "w");  // Open the file in append mode

    if (!file) {
        perror("Error opening file");
        return 1;
    }

    // Seed the random number generator with the current time
    srand((float)time(NULL));
    float x = 0.0001;
    float h = 0.001;
    for (int i = 0; i <= 1000; i++) {
        fprintf(file, "%f\n", x);  // Assuming you want random numbers between 0 and 99
        x += h;
    }

    fclose(file);

    printf("Random numbers generated and appended to the file.\n");

    return 0;
}
