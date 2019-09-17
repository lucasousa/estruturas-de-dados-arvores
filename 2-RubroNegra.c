#include <stdlib.h>
#include <stdio.h>
#include<time.h>
#define BLACK 1
#define RED 0

typedef struct RubroNegra{
    int info;
    int cor;
    struct RubroNegra *esq, *dir;
}RubroNegra;

RubroNegra *rotacaoDir(RubroNegra *arvore);
RubroNegra *rotacaoEsq(RubroNegra *arvore);
int cor(RubroNegra *arvore);
void trocaCor(RubroNegra **arvore);
RubroNegra *move2EsqRed(RubroNegra *arvore);
RubroNegra *moveDirRed(RubroNegra *arvore);
RubroNegra *balanceamento(RubroNegra *arvore);
RubroNegra *insere(RubroNegra *arvore, int valor, int *add);
int insereRubroNegra(RubroNegra **arvore, int valor);
void imprime(RubroNegra *arvore);
RubroNegra* busca(RubroNegra *arvore, int elem);
void liberar(RubroNegra *arv);
int maxDep(RubroNegra *arv);
int minDep(RubroNegra *arv);


int main(){
    srand(time(NULL));
    RubroNegra *arvore = NULL;

    int *vet = (int*)calloc(sizeof(int), 1000);
    float *tempo = (float*)malloc(sizeof(float)* 30);
    float *tempobusca = (float*)malloc(sizeof(float)* 30);

    int maxdep, mindep;

    clock_t start, finish;
    for(int i=0; i<30;i++){
        arvore = NULL;
        start=clock();
        for(int j=0; j<1000;j++){
            int n = rand() % 10000;
            insereRubroNegra(&arvore, n);
        }
        finish=clock();

        maxdep = maxDep(arvore);
        mindep = minDep(arvore);

        tempo[i] = (finish-start)*1000/CLOCKS_PER_SEC;
        printf("---------Teste %d----------",i+1);
        printf("\nMax Depth %d || ", maxdep);
        printf("Min Depth %d \n", mindep);

        start=clock();
        busca(arvore,5000);
        finish=clock();

        tempobusca[i] = (finish-start)*1000/CLOCKS_PER_SEC;

        vet[maxdep-mindep]++;
        liberar(arvore);
    }

    printf("\n\n");
    for (int i = 0; i < 1000; i++)
        if(vet[i]!=0)
            printf("Diferença  %d >> %d ocorrências\n",i,vet[i]);
    puts("\n");
    for(int i = 0; i < 30; i++){
        printf("-----------------Teste %d------------\nTempo de Criação %f >> Tempo de busca %f\n",i+1,tempo[i],tempobusca[i]);
    }
    printf("\n\n");

    /* imprime(arvore); */
    return 0;
}



int cor(RubroNegra *arvore){
    if(arvore == NULL) return BLACK;
    else return arvore->cor;
}

void trocaCor(RubroNegra **arvore){
    (*arvore)->cor = !(*arvore)->cor;
    if((*arvore)->esq != NULL)
        (*arvore)->esq->cor = !(*arvore)->esq->cor;
    if ((*arvore)->dir != NULL)
        (*arvore)->dir->cor = !((*arvore)->dir->cor);
}
RubroNegra *rotacaoEsq(RubroNegra *arvore){
    RubroNegra *aux  = arvore->dir;
    (arvore)->dir = aux->esq;
    aux->esq = (arvore);
    aux->cor = (arvore)->cor;
    (arvore)->cor = RED;
    return aux;
}

RubroNegra *rotacaoDir(RubroNegra *arvore){
    RubroNegra *aux = (arvore)->esq;
    (arvore)->esq = aux->dir;
    aux->dir = (arvore);
    aux->cor = (arvore)->cor;
    (arvore)->cor = RED;
    return aux;
}
RubroNegra *move2EsqRed(RubroNegra *arvore){
    trocaCor(&arvore);
    if(cor((arvore)->dir->esq) == RED){
        (arvore)->dir = rotacaoDir((arvore)->dir);
        (arvore) = rotacaoEsq(arvore);
        trocaCor(&arvore);
    }

    return arvore;
}
RubroNegra *moveDirRed(RubroNegra *arvore){
    trocaCor(&arvore);
    if(cor((arvore)->esq->esq) == RED){
       (arvore) = rotacaoDir(arvore);
        trocaCor(&arvore);
    }
    return arvore;
}
RubroNegra *balanceamento(RubroNegra *arvore){
    if(cor((arvore)->dir) == RED)
        arvore = rotacaoEsq(arvore);
    
    if( ((arvore)->esq != NULL) && (cor((arvore)->dir) == RED) && (cor((arvore)->esq->esq) == RED) )
        (arvore) = rotacaoDir(arvore);
    
    if(cor((arvore)->esq) == RED && cor((arvore)->dir) == RED)
        trocaCor(&arvore);
    
    return arvore;
}
RubroNegra *insere(RubroNegra *arvore, int valor, int *add){
    if(arvore == NULL){
        RubroNegra *novo;
        novo = (RubroNegra*)malloc(sizeof(RubroNegra));

        novo->info = valor;
        novo->cor = RED;
        novo->dir = NULL;
        novo->esq = NULL;
        *add = 1;

        return novo;
    }

    if(valor == (arvore)->info)
        *add = 0; //valor repetido

    else{
        if(valor < (arvore)->info)
            (arvore)->esq = insere((arvore)->esq, valor, add);
        else
            (arvore)->dir = insere((arvore)->dir, valor, add);
    }

    if(cor((arvore)->dir) == RED && cor((arvore)->esq) == BLACK)
        arvore = rotacaoEsq(arvore);

    if( (cor((arvore)->esq) == RED) && (cor((arvore)->esq->esq) == RED) )
        arvore = rotacaoDir(arvore);
    
    if(cor((arvore)->esq) == RED && cor((arvore)->dir) == RED)
        trocaCor(&arvore);
    
    return arvore;
}
int insereRubroNegra(RubroNegra **arvore, int valor){
    int add;
    *arvore = insere(*arvore, valor, &add);
    
    if(*arvore != NULL)
        (*arvore)->cor = BLACK;
    
    return add;
}
void imprime(RubroNegra *arvore){
    if(arvore){
        printf("<");
        printf("%d(%d) ", arvore->info, arvore->cor);
        imprime(arvore->esq);
        imprime(arvore->dir);
        printf(">");
    }
}
RubroNegra* busca(RubroNegra *arvore, int elem) {
    if (!arvore || arvore->info == elem)
       return arvore;
    if (arvore->info > elem)
       return busca(arvore->esq, elem);
    else
       return busca(arvore->dir, elem);
}

int maxDep(RubroNegra *arv){
    if(!arv) return -1;
    else{
        int he = maxDep(arv->esq);
        int hd = maxDep(arv->dir);
        if(he < hd) return hd+1;
        else return he+1;
    }
}

int minDep(RubroNegra *arv){
    if(!arv) return -1;
    else{
        int he = minDep(arv->esq);
        int hd = minDep(arv->dir);
        if(he > hd) return hd+1;
        else return he+1;
    } 
}
void liberar(RubroNegra *arv){
    if(!arv) return;
    liberar(arv->esq);
    liberar(arv->dir);
    free(arv);
}