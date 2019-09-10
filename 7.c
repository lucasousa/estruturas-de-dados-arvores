#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define TAM 2048

typedef struct i{
    int ini, fim, tam;
    char status;
}bloco;

typedef struct arvAVL{
    bloco bl;
    int fb;
    struct arvAVL *esq;
    struct arvAVL *dir;
} arvAVL;

arvAVL *criaVazia();                        //
void rotacaoRR(arvAVL **arv);               //
void rotacaoLL(arvAVL **arv);               //
void rotacaoRL(arvAVL **arv);               //
void rotacaoLR(arvAVL **arv);               //
int maiorAltura(arvAVL *arv);               //
void liberar(arvAVL *arv);                  //
int maxDep(arvAVL *arv);
int minDep(arvAVL *arv);
arvAVL *criaNo(int ini, int fim, char status);                    


void imprime(arvAVL *arvore);
int insere(arvAVL **raiz, arvAVL *No);           
arvAVL *busca(arvAVL *arv, int elem);
void insercao(arvAVL **raiz, arvAVL *No);

int main(){
    arvAVL *raiz = criaVazia();
    char first;
    int cont = 1;
    int ini = 1;
    int fim = 0;
    printf("Primeiro nó (L ou O) >> "  );
    setbuf(stdin, NULL);
    scanf("%c",&first);
    int Livre = 1;
    if(first == 'O')
        Livre--;
    while(fim<TAM){
        printf("início do nó %d ",ini);
        printf("Final do nó %d (limite de %d) >> ",cont,TAM);
        scanf("%d",&fim);
        if(Livre){
            insere(&raiz,criaNo(ini, fim, 'L'));
            Livre--;    
        }else{
            insere(&raiz,criaNo(ini, fim, 'O'));
            Livre++;
        }
        cont++;
        ini = fim+1;
    }
    imprime(raiz);
    printf("\n");


}


int insere(arvAVL **raiz, arvAVL *no){
    if (*raiz == NULL)
        *raiz = no;
    else{
        insere(&((*raiz)->dir), no);
        if (abs(maxDep((**raiz).esq) - maxDep((**raiz).dir)) == 2)
            rotacaoRR(raiz);     
    }
}
void imprime(arvAVL *arvore){
    if (arvore){
        printf("< ");
        imprime(arvore->esq);
        printf("%d-%d(%d)[%c]", arvore->bl.ini, arvore->bl.fim, arvore->bl.tam, arvore->bl.status);
        imprime(arvore->dir);
        printf(" >");
    }
}

/* arvAVL *busca(arvAVL *arv, int elem){
    if (!arv || arv->info == elem)
        return arv;
    if (arv->info > elem)
        return busca(arv->esq, elem);
    else
        return busca(arv->dir, elem);
} */




arvAVL *criaVazia(){
    return NULL;
}
arvAVL *criaNo(int ini, int fim, char status){
    arvAVL *x;
    x = (arvAVL *)malloc(sizeof(arvAVL));
    x->dir = NULL;
    x->esq = NULL;
    x->bl.fim = fim;
    x->bl.ini = ini;
    x->bl.status = status;
    x->bl.tam = fim - ini + 1;
    return x;
}
int maiorAltura(arvAVL *arv){
    int left = maxDep(arv->esq);
    int right = maxDep(arv->dir);
    if (left > right)
        return left + 1;
    else
        return right + 1;
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