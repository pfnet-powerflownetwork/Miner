#include <stdio.h>
#include <stdlib.h>

int main() {
    int *ptr;
    int n, i;

    // Prompt the user for the number of elements
    printf("Enter the number of elements: ");
    scanf("%d", &n);

    // Allocate memory for n elements
    ptr = (int*) malloc(n * sizeof(int));

    // Check if the memory was successfully allocated
    if (ptr == NULL) {
        printf("Error: Memory not allocated.\n");
        return 1;
    }

    printf("Memory successfully allocated. Enter the values:\n");
    for (i = 0; i < n; i++) {
        scanf("%d", &ptr[i]);
    }

    printf("The entered values are:\n");
    for (i = 0; i < n; i++) {
        printf("%d ", ptr[i]);
    }

    printf("\nFreeing memory...\n");
    free(ptr);

    return 0;
}
