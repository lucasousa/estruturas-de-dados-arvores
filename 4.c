#include <stdio.h>
#include <stdlib.h>

typedef struct lista{
    char info[20];
    struct lista *prox;
} lista;

typedef struct arv23Word{
    char word1[20], word2[20];
    int nInfo;
    struct arv23Word *esq;
    struct arv23Word *cent;
    struct arv23Word *dir;
    struct lista *lista;
} arv23Word;


void copiar(char *string1, char *string2)
{
    while (*string1 != '\0')
    {
        *string2 = *string1;
        string1++;
        string2++;
    }
}

int compara(const char *s1, const char *s2){
    int i = 0;
    while ((s1[i] == s2[i]) && (s1[i] != '\0') && (s2[i] != '\0')){
        i++;
    }
    if (s1[i] == '\0' && s2[i] == '\0'){
        return 0;
    }
    else if (s1[i] != s2[i]){
        if (s1[i] > s2[i]){
            return 1;
        }
        else{
            return -1;
        }
    }
}

arv23Word *criaVazia(){
    return NULL;
}

int ehfolha(arv23Word *no){
    if (((*no).esq == NULL) && ((*no).cent == NULL) && ((*no).dir == NULL))
        return 1;
    else
        return 0;
}

arv23Word *alocando(char *valor, arv23Word *esq, arv23Word *cent, arv23Word *dir){
    arv23Word *no;
    no = (arv23Word *)malloc(sizeof(arv23Word));
    copiar(valor, no->word1);
    no->nInfo = 1;
    no->esq = esq;
    no->cent = cent;
    no->dir = dir;
    return no;
}

arv23Word *adicionaNo(arv23Word *no, char *valor, arv23Word *filho){
    if (compara(valor, (*no).word1) == 1){
        copiar(valor, (*no).word2);
        (*no).dir = filho;
    }
    else{
        copiar((*no).word1,(*no).word2);
        copiar(valor, no->word1);
        no->dir = no->cent;
        no->cent = filho;
    }
    no->nInfo = 2;
    return no;
}

/* arv23Word *quebraNo(arv23Word **no, char *valor, char *promove, arv23Word *subarvore){
    arv23Word *paux;
    if(compara(valor, (**no).word1)==-1){
        copiar((*no)->word1, promove);
        paux = (**no).dir;
        (**no).nInfo = 1;
        return criaNo((*no)->word1, paux, subarvore, NULL);
    }
    else{
        if(compara(valor, (**no).word2) == 0 || compara(valor, (**no).word2) == 1 ){
            copiar(valor, promove);
            paux = (**no).dir;
            (**no).nInfo = 1;
            return criaNo((**no).word2, subarvore, paux, NULL);
        }
        else{
            arv23Word *centro, *dir;
            *promove = (**no).word1;
            copiar((**no).word2, paux);
            centro = (**no).cent;
            dir = (**no).dir;
            copiar(valor, (**no).word1);
            (**no).nInfo = 1;
            (**no).dir = NULL;
            (**no).cent = subarvore;
            return criaNo(paux, centro, dir, NULL);
        }
    }
}
 */

arv23Word *quebraNo(arv23Word *no, char *valor, char *promover, arv23Word *subarvore){
    arv23Word *paux;

    if (compara(valor, (*no).word2)== 1){  // val esta mais a direita
       *promover = no->word2;   // promove a antiga maior
       paux = no->dir;
       no->dir = NULL;   // elimina o terceiro filho
       no->nInfo = 1;      // atualiza o número de chaves
       return criaNo(valor, paux, subarvore, NULL);
    } else if (compara(valor, (*no).word1) == 1){ // val esta no meio
       copiar(valor, promover);        // continua sendo promovido
       paux = no->dir;
       no->dir = NULL;
       no->nInfo = 1;
       return criaNo(no->word2, subarvore, paux, NULL);
    } else {              // val esta a mais a esquerda
       copiar(no->word1, promover);   // primeiro cria o n� a direita
       paux = criaNo(no->word2, no->cent, no->dir, NULL);
       copiar(valor, no->word1);   // em seguida arruma o n� a esquerda
       no->nInfo = 1;
       no->dir = NULL;
       no->cent = subarvore;
       return paux;
    }
}

arv23Word *criaNo(char* valor, arv23Word *esq, arv23Word *cent, arv23Word *dir){
    arv23Word *no = malloc(sizeof(arv23Word));
    copiar(valor, no->word1);
    no->dir = dir;
    no->cent = cent;
    no->esq = esq;
    no->nInfo = 1;
    return no;
}


arv23Word *insere(arv23Word **raiz, char *valor, char *promove, arv23Word *pai){
    arv23Word *paux;
    if(*raiz == NULL){
        *raiz = criaNo(valor, NULL, NULL, NULL);
        paux = NULL;
    }else{
        if(ehfolha(*raiz)){
            if((**raiz).nInfo == 1){
                *raiz = adicionaNo(*raiz, valor, NULL);
                paux = NULL;
            }
            else{
                paux = quebraNo(raiz, valor, promove, NULL);
            }
        }
        else{
            if(compara(valor, (**raiz).word1)==1){
                paux = insere(&((*raiz)->esq), valor, promove, *raiz);
            }else if(((*raiz)->nInfo == 1) || (compara(valor, (*raiz)->word2)== -1)){
                paux = insere(&((*raiz)->cent), valor, promove, *raiz);
            }
            else paux = insere(&((*raiz)->dir), valor, promove, *raiz);
        }
    }
    if(paux != NULL){
        if(pai != NULL){
            *raiz = criaNo(*promove, *raiz, paux, NULL);
            paux = NULL;
        }
        else if((*raiz)->nInfo == 1){
            *raiz = adicionaNo(*raiz, promove, paux);
            paux = NULL;
        }
        else paux = quebraNo(raiz, *promove, promove, paux);
    }
    return paux;
}

void imprime(arv23Word *raiz){
    if(raiz){
        printf("<");
        if(raiz->nInfo == 2)
            printf("%s || %s",raiz->word1,raiz->word2);
        else
            printf("%s",raiz->word1,raiz->word2);
        imprime(raiz->esq);
        imprime(raiz->cent);
        imprime(raiz->dir);
        printf(">");
    }
}

int main(){
    arv23Word *arvore;
    arvore = criaVazia();
    insere(&arvore, "Samuel", NULL, NULL);
    imprime(arvore);






    return 0;
}

