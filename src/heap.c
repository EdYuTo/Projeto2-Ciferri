#include <utils.h>
#include <heap.h>
#include <stdlib.h>
#define PAI(pos) ((pos -1)/2)

struct heap{
    INDEX **vetor;
    int fim;
    int tamanho;
};

HEAP *criar_heap(int tamanho){
    HEAP *heap = (HEAP*) malloc(sizeof(HEAP));
    if(heap != NULL){
        heap->fim = -1;
        heap->tamanho = tamanho;
        heap->vetor = (INDEX**) malloc(sizeof(INDEX*) * tamanho);
    }
    return heap;
}

void apagar_heap(HEAP** heap){
    if(heap != NULL && *heap != NULL){
        int i;

        //Laço que apaga os itens do vetor heap
        for(i = 0; i <= (*heap)->fim; i++){
            apagar_index(&(*heap)->vetor[i]);
        }
        free(*heap);
        *heap = NULL;
    }
}

//Função que troca is Itens de 2 posições do vetor do HEAP
void swap(HEAP *heap, int pos1, int pos2){
    if(heap != NULL){
        INDEX *aux = heap->vetor[pos1];
        heap->vetor[pos1] = heap->vetor[pos2];
        heap->vetor[pos2] = aux;
    }
}

void fix_up(HEAP *heap, int pos){
    //Laço que rearranja o heap
    while(pos > 0 && heap->vetor[pos]->ticket <= heap->vetor[PAI(pos)]->ticket){

        //Analiza se a chave do pai é menor que do filho
        if(heap->vetor[pos]->ticket < heap->vetor[PAI(pos)]->ticket)
            swap(heap, pos, PAI(pos)); //Se entrou no if, troca o pai com o filho

        pos = PAI(pos);
    }
}

int inserir_heap(HEAP *heap, INDEX *item){
    if(heap != NULL && item != NULL){

        if(cheia_heap(heap)){
            heap->tamanho = heap->tamanho * 2;
            heap->vetor = (INDEX**) realloc(heap->vetor, sizeof(INDEX*) * heap->tamanho);
        }

        heap->fim++;
        heap->vetor[heap->fim] = item; //insere novo item no fim

        fix_up(heap, heap->fim); //rearranja o heap a partir do fim
        return 1;
    }
    return 0;
}



void fix_down(HEAP *heap, int pos){
    int menorfilho = 2*pos +1;

    //Condicional que encontra o menor filho de pos
    if((2*pos+2) <= heap->fim && heap->vetor[menorfilho]->ticket > heap->vetor[2*pos+2]->ticket)
        menorfilho = 2*pos+2;

    //Laço que rearranja o heap
    while(menorfilho <= heap->fim && heap->vetor[pos]->ticket >= heap->vetor[menorfilho]->ticket){
        //Condicional, que analiza as chaves
        if(heap->vetor[pos]->ticket > heap->vetor[menorfilho]->ticket)
            swap(heap, pos, menorfilho); //Se entrou no condicional, troca as posições
        pos = menorfilho;

        menorfilho = 2*pos+1;
        //Condicional que encontra o menor filho
        if((2*pos+2) <= heap->fim && heap->vetor[menorfilho]->ticket > heap->vetor[2*pos+2]->ticket)
            menorfilho = 2*pos+2;
    }

}


INDEX *remover_heap(HEAP *heap){
    if(heap != NULL && !vazia_heap(heap)){
        swap(heap, 0, heap->fim); //Troca o inicio com o fim
        INDEX *ret = heap->vetor[heap->fim]; //remove o fim do heap
        heap->fim--;

        fix_down(heap, 0); //rearranja o heap a partir do inicio
        return ret;
    }
    return NULL;
}

int cheia_heap(HEAP *heap){
    return (heap->fim == heap->tamanho - 1);
}


int vazia_heap(HEAP *heap){
    return(heap->fim == -1);
}

void heap_sort(INDEX **indices, int nIndices){
    if(indices != NULL){
        int i;
        HEAP *heap = criar_heap(100);

        for(i = 0; i < nIndices; i++){
            inserir_heap(heap, indices[i]);
        }

        for(i = 0; i < nIndices; i++){
            indices[i] = remover_heap(heap);
        }

        apagar_heap(&heap);
    }
}
