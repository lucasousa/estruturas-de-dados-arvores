#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
int INI = 1;
int FIM;


typedef struct i{
    int ini, fim, tam;
    int iniG,fimG;
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

int temUmFilho(arvAVL *raiz);
int ehFolha(arvAVL *raiz);
arvAVL *maioresq(arvAVL *raiz, arvAVL **pai);
arvAVL *menordir(arvAVL *raiz, arvAVL **pai);
void alteraestado(arvAVL **no);
arvAVL *alteracao(arvAVL **raiz, int tam, char param);
void imprime(arvAVL *arvore);
int insere(arvAVL **raiz, arvAVL *No);           
arvAVL *busca(arvAVL **arv, int tam, arvAVL **pai, char param);
void insercao(arvAVL **raiz, arvAVL *No);

int main(){
    arvAVL *raiz = criaVazia();
    char first;
    int cont = 1;
    int ini = 1;
    int fim = 0;
    printf("Qual o tamanho da memória em MB? >> "); scanf("%d", &FIM);
    printf("Primeiro nó (L ou O) >> "  );
    setbuf(stdin, NULL);
    scanf("%c",&first);
    int Livre = 1;
    if(first == 'O')
        Livre--;
    
    while(fim<FIM){
        printf("início do nó: %d\n",ini);
        printf("Digite o final do nó número %d (limite de %d) >> ",cont,FIM);
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
    char op = 'A';
    int qt;
    imprime(raiz);
    while(op != 'S'){
        printf("Deseja liberar ou ocupar? O || L >> ");setbuf(stdin,NULL);scanf("%c",&op);
        printf("Quanto deseja alterar em MB?  >> ");scanf("%d",&qt);
        if(op == 'O')
            raiz = alteracao(&raiz, qt, 'L');
        else
            raiz = alteracao(&raiz, qt, 'O');
        printf("\n");
        imprime(raiz);
        printf("\n");
    }
    


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


void alteraestado(arvAVL **no){
    if((*no)->bl.status == 'O')
        (*no)->bl.status = 'L';
    else
        (*no)->bl.status = 'O';
}


arvAVL *maioresq(arvAVL *raiz, arvAVL **pai){
    if((raiz)->dir != NULL)
        return maioresq(raiz->dir, &raiz);
    return raiz;
}

arvAVL *menordir(arvAVL *raiz, arvAVL **pai){
    if((raiz)->esq != NULL)
        return menordir(raiz->esq, &raiz);
    return raiz;
}


arvAVL *alteracao(arvAVL **raiz, int tam, char param){
    arvAVL *pai = malloc(sizeof(arvAVL));
    arvAVL *aux = busca(raiz, tam, &pai, param);
    if(!aux) return *raiz;
    if(ehFolha(aux)){
        if(aux->bl.ini == 1 && aux->bl.fim == FIM){
            if(aux->bl.tam > tam){
                if(aux->bl.status == 'O'){
                    aux->esq = criaNo(1,tam,'L');
                    aux->bl.ini+=tam;
                    aux->bl.tam-=tam;
                }else{
                    aux->esq = criaNo(1,tam,'O');
                    aux->bl.ini+=tam;
                    aux->bl.tam-=tam;
                }
            }else{
                alteraestado(&aux);
            }
        }else if(aux == pai->dir){    //é o filho da direita
            if(aux->bl.tam > tam){ // bloco é maior que o tamanho requerido
                aux->bl.ini+=tam;
                aux->bl.tam-=tam;
                pai->bl.fim+=tam;
                pai->bl.tam+=tam;
            }else{                 // bloco é igual ao tamanho requerido
                pai->dir = NULL;
                free(aux);
                pai->bl.fim+=tam;
                pai->bl.tam+=tam;
            }
        }else{                  //é o filho da esquerda
            if(aux->bl.tam > tam){ // bloco é maior que o tamanho requerido
                aux->bl.fim-=tam;
                aux->bl.tam-=tam;
                pai->bl.ini-=tam;
                pai->bl.tam+=tam;
            }else{                 // bloco é igual ao tamanho requerido
                pai->esq = NULL;
                free(aux);
                pai->bl.ini-=tam;
                pai->bl.tam+=tam;
            }
        }
    }else if(temUmFilho(*raiz)){ //tem 1 filho
        if(aux->esq == NULL){      //é o filho da direita
            arvAVL *paiaux = aux;
            arvAVL *men = aux->dir;
            while(men->esq!=NULL){
                paiaux = men;
                men=men->dir;
            }
            if(aux->bl.tam > tam){ // bloco é maior que o tamanho requerido
                aux->bl.fim-=tam;
                aux->bl.tam-=tam;
                men->bl.ini-=tam;
                men->bl.tam+=tam;
            }else{                 // bloco é igual ao tamanho requerido
                aux->bl.fim+=tam;
                aux->bl.tam+=tam;
                alteraestado(&aux);
                if(paiaux == aux)
                    aux->dir = aux->dir->dir;
                else
                    paiaux->esq = men->dir;
            }
        }else{                     //é o filho da esquerda
            arvAVL *paiaux = aux;
            arvAVL *mai = aux->esq;
            while(mai->dir!=NULL){
                paiaux = mai;
                mai = mai->dir;
            }
            if(aux->bl.tam > tam){ // bloco é maior que o tamanho requerido
                aux->bl.ini+=tam;
                aux->bl.tam-=tam;
                mai->bl.fim+=tam;
                mai->bl.tam+=tam;
            }else{                 // bloco é igual ao tamanho requerido
                aux->bl.ini-=tam;
                aux->bl.tam+=tam;
                alteraestado(&aux);
                if(paiaux == aux)
                    aux->esq = aux->esq->esq;
                else
                    paiaux->dir = mai->esq;
            }
        }
    }else{  // Tem 2 filhos (merge com menordir e maioresq)
        arvAVL *paiauxmen = aux;
        arvAVL *paiauxmai = aux;
        arvAVL *mai = aux->esq;
        arvAVL *men = aux->dir;
        while(mai->dir!=NULL){
            paiauxmai = mai;
            mai = mai->dir;
        }
        while(men->esq!=NULL){
            paiauxmen = men;
            men=men->dir;
        }
        if(aux->bl.tam > tam){ // bloco é maior que o tamanho requerido
            aux->bl.ini+=tam;
            aux->bl.tam-=tam;
            mai->bl.fim+=tam;
            mai->bl.tam+=tam;
        }else{                 // bloco é igual ao tamanho requerido
            aux->bl.ini=mai->bl.ini;
            aux->bl.fim=men->bl.fim;
            aux->bl.tam=aux->bl.fim - aux->bl.ini + 1;
            alteraestado(&aux);
            if(paiauxmai == aux)
                aux->esq = aux->esq->esq;
            else
                paiauxmai->dir = mai->esq;
            if(paiauxmen == aux)
                aux->dir = aux->dir->dir;
            else
                paiauxmen->esq = men->dir;
        }
    }
    return *raiz;
}

arvAVL *busca(arvAVL **arv, int tam, arvAVL **pai, char param){
    if(!(*arv))
        return NULL;
    if((*arv)->bl.tam >= tam && param == (*arv)->bl.status)
        return *arv;
    arvAVL* deucerto;
    *pai = *arv;
    deucerto = busca(&((*arv)->esq), tam, pai, param);
    if(deucerto)
        return deucerto;
    return busca(&((*arv)->dir), tam, pai, param);
} 



int ehFolha(arvAVL *raiz){
    if(!raiz->esq && !raiz->dir) return 1;
    else return 0;
}


int temUmFilho(arvAVL *raiz){
    if ( (!raiz->esq && raiz->dir) || (raiz->esq && !raiz->dir) ) return 1;
    else return 0;
}

int temDoisFilhos(arvAVL *raiz){
    if(raiz->dir && raiz->esq) return 1;
    else return 0;
}




arvAVL *criaVazia(){
    return NULL;
}
arvAVL *criaNo(int ini, int fim, char status){
    arvAVL *x;
    x = (arvAVL *)malloc(sizeof(arvAVL));
    x->dir = NULL;
    x->esq = NULL;
    x->bl.fimG = FIM;
    x->bl.iniG = 1;
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