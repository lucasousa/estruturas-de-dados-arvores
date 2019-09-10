#include <stdio.h>
#include <stdlib.h>
#include <time.h>


typedef struct arvBin{
	int info;
	struct arvBin* esq;
	struct arvBin* dir;
}arvBin;

int vazia(arvBin *arvore);
arvBin *criaVazia();
arvBin *criaNo(int var);
void insere(arvBin **raiz, arvBin *No);
int maxDep(arvBin *arv);
int ehFolha(arvBin *arv);
void imprime(arvBin *arvore);
int minDep(arvBin *arv);
void liberar(arvBin *arv);
arvBin* busca(arvBin *arv, int elem);

int main(){
    srand(time(NULL));
    arvBin *arvore;
    arvore = criaVazia();
    int *vet = (int*)calloc(sizeof(int), 1000);
    float *tempo = (float*)malloc(sizeof(float)* 30);
    float *tempobusca = (float*)malloc(sizeof(float)* 30);
    int maxdep, mindep;
    clock_t start, finish;
    for(int i=0; i<30;i++){
        arvore = criaVazia();
        start=clock();
        for(int j=0; j<1000;j++){
            int n = rand() % 10000;
            insere(&arvore, criaNo(n));
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
    


    return 0;
}

arvBin* busca(arvBin *arv, int elem) {
    if (!arv || arv->info == elem)
       return arv;
    if (arv->info > elem)
       return busca(arv->esq, elem);
    else
       return busca(arv->dir, elem);
}

arvBin *criaVazia(){
	return NULL;
}

int vazia(arvBin *arvore){
	return arvore == NULL;
}


arvBin *criaNo(int var){
	arvBin *x;
	x = (arvBin*)malloc(sizeof(arvBin));

	x->dir = NULL;
	x->esq = NULL;
	x->info = var;

	return x;
}

void insere(arvBin **raiz, arvBin *No){
	if(*raiz == NULL)
		*raiz = No;
	else if(No->info < (*raiz)->info)
		insere(&(*raiz)->esq, No);
	else
		insere(&(*raiz)->dir, No);
}

void imprime(arvBin *arvore){
	if(!vazia(arvore)){
		printf("< ");
		printf("%d", arvore->info);
		imprime(arvore->esq);
		imprime(arvore->dir);
		printf(" >");
	}	
}

int ehFolha(arvBin *arv){
    if(!arv->dir && !arv->esq) return 0;
    return 1;
}

int maxDep(arvBin *arv){
    if(!arv) return -1;
    else{
        int he = maxDep(arv->esq);
        int hd = maxDep(arv->dir);
        if(he < hd) return hd+1;
        else return he+1;
    }
}

int minDep(arvBin *arv){
    if(!arv) return -1;
    else{
        int he = minDep(arv->esq);
        int hd = minDep(arv->dir);
        if(he > hd) return hd+1;
        else return he+1;
    } 
}

void liberar(arvBin *arv){
    if(!arv) return;
    liberar(arv->esq);
    liberar(arv->dir);
    free(arv);
}