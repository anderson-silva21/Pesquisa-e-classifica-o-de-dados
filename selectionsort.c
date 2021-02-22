#include <stdio.h>
#include <stdlib.h>

int selectionSort(int *vetor, int tamanho){
  int menor, troca;
  for(int i = 0; i < tamanho - 1; i++){
    menor = i;
    for(int j = i + 1; j < tamanho; j++){
      if(vetor[j] < vetor[menor]){
        menor = j;
      }
    }
    if(i != menor){
      troca = vetor[i];
      vetor[i] = vetor[menor];
      vetor[menor] = troca;
    }
  }
}

int printarvetor(int *vetor, int tamanho){
  for(int i = 0; i < tamanho; i++){
    printf("[%d] ", vetor[i]);
  }
}

int main(int argc, char const *argv[]) {
  int *vetor = NULL;
  int tamanho;
  printf("Insira o tamnho do vetor\n");
  scanf("%d", &tamanho);
  vetor = (int*)malloc(tamanho * sizeof(int));
  for(int i = 0; i < tamanho; i++){
    printf("Insira o valor [%i] no vetor\n", i);
    scanf("%d", &vetor[i]);
  }
  selectionSort(vetor, tamanho);
  printarvetor(vetor, tamanho);

  return 0;
}