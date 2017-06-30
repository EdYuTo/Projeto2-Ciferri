//
// Created by exilio016 on 30/06/17.
//

#include "utils.h"

#ifndef PROJETO2_CIFERRI_HEAP_H
#define PROJETO2_CIFERRI_HEAP_H

typedef struct heap HEAP;

HEAP *criar_heap(int tamanho);
void apagar_heap(HEAP **heap);

int inserir_heap(HEAP *heap, INDEX *index);
INDEX *remover_heap(HEAP* heap);

int cheia_heap(HEAP *heap);
int vazia_heap(HEAP *heap);

void heap_sort(INDEX **indices, int nIndices);

#endif //PROJETO2_CIFERRI_HEAP_H
