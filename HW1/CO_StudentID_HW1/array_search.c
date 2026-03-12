#include <stdio.h>

int arraySearch(int *p_a, int arr_size, int target)
{
    int result = -1;

    asm volatile(
        "li t2, 0                \n\t" // Initialize index i to 0
        "1:                       \n\t"
        "bge t2, %1, 2f           \n\t" // if i >= arr_size, exit loop
        "lw t0, 0(%0)             \n\t" // Load arr[i] into t0
        "beq t0, %2, 3f           \n\t" // If arr[i] == target, go to found
        "addi t2, t2, 1           \n\t" // Increment i
        "addi %0, %0, 4           \n\t" // Move to next element
        "j 1b                     \n\t" // Jump back to the start of the loop
        "2:                       \n\t" // Exit loop if i >= arr_size
        "j 4f                     \n\t" // Jump to end
        "3:                       \n\t" // Found target
        "mv %3, t2                \n\t" // Store index in result
        "4:                       \n\t"
        : "+r"(p_a), "+r"(arr_size), "=r"(target), "=r"(result) // Input/output operands
        : // No additional input operands
        : "t0", "t2" // Clobbered registers
    );

    return result;
}

// Main function to test the implementation
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
    int target;
    fscanf(input, "%d", &target);
    fclose(input);

    int *p_a = &arr[0];

    int index = arraySearch(p_a, arr_size, target);

    // Print the result
    printf("%d ", index);
    printf("\n");

    return 0;
}
