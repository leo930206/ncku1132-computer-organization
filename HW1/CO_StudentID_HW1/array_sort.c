#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    FILE *input = fopen(argv[1], "r");
    if (!input) {
        fprintf(stderr, "Error opening file: %s\n", argv[1]);
        return 1;
    }

    int arr_size;
    fscanf(input, "%d", &arr_size);
    int arr[arr_size];

    // Read integers from input file into the array
    for (int i = 0; i < arr_size; i++) {
        int data;
        fscanf(input, "%d", &data);
        arr[i] = data;
    }
    fclose(input);

    int *p_a = &arr[0];

    // array a bubble sort
    for (int i = 0; i < arr_size - 1; i++) {
        for (int j = 0; j < arr_size - i - 1; j++) {
            asm volatile(
                "lw t0, 0(%0)        \n\t" // Load arr[j] into t0
                "lw t1, 4(%0)        \n\t" // Load arr[j+1] into t1
                "blt t0, t1, 1f      \n\t" // If arr[j] < arr[j+1], skip the swap
                "sw t1, 0(%0)        \n\t" // Store arr[j+1] into arr[j]
                "sw t0, 4(%0)        \n\t" // Store arr[j] into arr[j+1]
                "1:                  \n\t"
                : // no output operands
                : "r"(&arr[j])  // input operand: address of arr[j]
                : "t0", "t1"    // clobbered registers
            );
        }
    }

    p_a = &arr[0];
    for (int i = 0; i < arr_size; i++)
        printf("%d ", *p_a++);
    printf("\n");

    return 0;
}
