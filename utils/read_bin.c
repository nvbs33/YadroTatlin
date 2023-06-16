#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s filename", argv[0]);
        return 1;
    }
    char *filename = argv[1];

    FILE *fp = fopen(filename, "rb"); // open binary file for reading
    if (fp == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    int num;
    while (fread(&num, sizeof(num), 1, fp) == 1) { // read int values from file
        printf("%d\n", num); // print each value to screen
    }

    fclose(fp); // close file
    return 0;
}
