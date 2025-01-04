#include <stdio.h>
#include <stdlib.h>

int main() {
    int *ptr;
    int n, i;

    // Solicita ao usuário o número de elementos
    printf("Digite o número de elementos: ");
    scanf("%d", &n);

    // Aloca memória para n elementos
    ptr = (int*) malloc(n * sizeof(int));

    // Verifica se a memória foi alocada com sucesso
    if (ptr == NULL) {
        printf("Erro: Memória não alocada.\n");
        return 1;
    }

    printf("Memória alocada com sucesso. Insira os valores:\n");
    for (i = 0; i < n; i++) {
        scanf("%d", &ptr[i]);
    }

    printf("Os valores inseridos são:\n");
    for (i = 0; i < n; i++) {
        printf("%d ", ptr[i]);
    }

    printf("\nLiberando memória...\n");
    free(ptr);

    return 0;
}
