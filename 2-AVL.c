#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define TAM 15000

typedef struct arvAVL{
    int info;
    int fb;
    struct arvAVL *esq;
    struct arvAVL *dir;
} arvAVL;

arvAVL *criaVazia();
arvAVL *criaNo(int var);
int insere(arvAVL **raiz, arvAVL *No);
int maxDep(arvAVL *arv);
void imprime(arvAVL *arvore);
int minDep(arvAVL *arv);
void liberar(arvAVL *arv);
arvAVL *busca(arvAVL *arv, int elem);
void rotacaoRR(arvAVL **arv);
void rotacaoLL(arvAVL **arv);
void rotacaoRL(arvAVL **arv);
void rotacaoLR(arvAVL **arv);
void insercao(arvAVL **raiz, arvAVL *No);
int maiorAltura(arvAVL *arv);
int *aleatorio();

int main(){
    srand(time(NULL));
    arvAVL *arvore;
    arvore = criaVazia();
    int *vet = (int*)calloc(sizeof(int), 1000);
    float *tempo = (float*)malloc(sizeof(float)* 30);
    double *tempobusca = (double*)malloc(sizeof(double)* 30);
    int maxdep, mindep;
    int *ptr;

    clock_t start, finish;
    for(int i=0; i<30;i++){
        arvore = criaVazia();
        start=clock();
        ptr = aleatorio();
        for(int j=0; j<TAM;j++){
            insere(&arvore, criaNo(ptr[j]));
        }
        finish=clock();
        maxdep = maxDep(arvore);
        mindep = minDep(arvore);
        tempo[i] = (finish-start)*1000/CLOCKS_PER_SEC;
        printf("---------Teste %d----------",i+1);
        printf("\nMax Depth %d || ", maxdep);
        printf("Min Depth %d \n", mindep);
        start=clock();
        busca(arvore,50000);
        finish=clock();
        tempobusca[i] = ( (double)(finish-start))*1000/CLOCKS_PER_SEC;

        vet[maxdep-mindep]++;
        liberar(arvore);
    }
    printf("\n\n");
    for (int i = 0; i < 1000; i++)
        if(vet[i]!=0)
            printf("Diferença  %d >> %d ocorrências\n",i,vet[i]);
    puts("\n");
    for(int i = 0; i < 30; i++){
        printf("-----------------Teste %d------------\nTempo de Criação %f >> Tempo de busca %lf\n",i+1,tempo[i],tempobusca[i]);
    }
    printf("\n\n");

}

int *aleatorio(){
    int *ptr;
    ptr = (int *)malloc(sizeof(int) * TAM);
    for (int i = 0; i < TAM; i++)
    {
        ptr[i] = rand() % (TAM + (TAM / 2));
    }
    return ptr;
}

void rotacaoRR(arvAVL **arv){
    arvAVL *aux = (*arv)->dir;
    (*arv)->dir = aux->esq;
    aux->esq = *arv;
    *arv = aux;
}
void rotacaoLL(arvAVL **arv){
    arvAVL *aux = (*arv)->esq;
    (*arv)->esq = aux->dir;
    aux->dir = *arv;
    *arv = aux;
}
void rotacaoRL(arvAVL **arv){
    rotacaoLL(&(*arv)->dir);
    rotacaoRR(arv);
}
void rotacaoLR(arvAVL **arv){
    rotacaoRR(&(*arv)->esq);
    rotacaoLL(arv);
}

arvAVL *criaNo(int var){
    arvAVL *x;
    x = (arvAVL *)malloc(sizeof(arvAVL));
    x->dir = NULL;
    x->esq = NULL;
    x->info = var;
    return x;
}

int insere(arvAVL **raiz, arvAVL *no){
    int inseriu = 1;
    if (*raiz == NULL)
        *raiz = no;

    else{
        if (no->info < (**raiz).info){
            if (insere(&((*raiz)->esq), no) == 1){
                if (abs(maxDep((**raiz).esq) - maxDep((**raiz).dir)) == 2){
                    if (no->info < (**raiz).esq->info)
                        rotacaoLL(raiz);
                    else
                        rotacaoLR(raiz);
                }
            }
        }
        else if (no->info > (**raiz).info){
            if (insere(&((*raiz)->dir), no)){
                if (abs(maxDep((**raiz).esq) - maxDep((**raiz).dir)) == 2){
                    if (no->info > (**raiz).dir->info)
                        rotacaoRR(raiz);
                    else
                        rotacaoRL(raiz);
               }
            }
        }
        else
            inseriu = 0;
        (**raiz).fb = maiorAltura(*raiz);
    }
    return inseriu;
}

int maiorAltura(arvAVL *arv){
    int left = maxDep(arv->esq);
    int right = maxDep(arv->dir);
    if (left > right)
        return left + 1;
    else
        return right + 1;
}
arvAVL *busca(arvAVL *arv, int elem){
    if (!arv || arv->info == elem)
        return arv;
    if (arv->info > elem)
        return busca(arv->esq, elem);
    else
        return busca(arv->dir, elem);
}
arvAVL *criaVazia(){
    return NULL;
}
void imprime(arvAVL *arvore){
    if (arvore){
        printf("< ");
        printf("%d(%d)", arvore->info, arvore->fb);
        imprime(arvore->esq);
        imprime(arvore->dir);
        printf(" >");
    }
}
int maxDep(arvAVL *arv){
    if (!arv)
        return -1;
    else{
        int he = maxDep(arv->esq);
        int hd = maxDep(arv->dir);
        if (he < hd)
            return hd + 1;
        else
            return he + 1;
    }
}
int minDep(arvAVL *arv){
    if (!arv)
        return -1;
    else{
        int he = minDep(arv->esq);
        int hd = minDep(arv->dir);
        if (he > hd)
            return hd + 1;
        else
            return he + 1;
    }
}
void liberar(arvAVL *arv){
    if (!arv)
        return;
    liberar(arv->esq);
    liberar(arv->dir);
    free(arv);
}