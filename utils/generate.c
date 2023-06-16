//
// Created by Борис Новоселов on 13.06.2023.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <number of integers>\n", argv[0]);
        return 1;
    }

    int count = atoi(argv[1]);

    FILE *fp = fopen("output.bin", "wb");
    if (fp == NULL) {
        printf("Error opening file for writing.\n");
        return 1;
    }
    srand(time(NULL));
    for (int i = 0; i < count; i++) {
        int num = rand() % 100;
        fwrite(&num, sizeof(num), 1, fp);
    }

    fclose(fp);

    return 0;
}
