#include <stdio.h>
#include <stdlib.h>

typedef struct lista{
    char info[20];
    struct lista *prox;
}lista;

typedef struct arv23Word{
    char word1[20], word2[20];
    int nInfo;
    struct arv23Word *esq;
    struct arv23Word *cent;
    struct arv23Word *dir;
    struct lista *lista;
}arv23Word;

void copiar(char *string1, char *string2){ //copia string1 para string2
    while (*string1 != '\0'){
        *string2 = *string1;
        string1++;
        string2++;
    }
    *string2 = '\0';
}

int compara(const char *s1, const char *s2){ 
    int i = 0;
    while ((s1[i] == s2[i]) && (s1[i] != '\0') && (s2[i] != '\0'))
        i++;
    if (s1[i] == '\0' && s2[i] == '\0')
        return 0;
    else if (s1[i] != s2[i]){
        if (s1[i] > s2[i])
            return 1;           //S1 fica depois de S2
        else
            return -1;          //S1 fica antes de S2
    }
}
arv23Word *criaVazia(){
    return NULL;
}

int ehfolha(arv23Word *no){
    if( ( no->esq == NULL) && ( (*no).cent == NULL) && ((*no).dir == NULL) )
        return 1;
    else return 0;
}

arv23Word *alocando(char *valor, arv23Word *esq, arv23Word *cent, arv23Word *dir ){

    arv23Word *no;
    no = (arv23Word*)malloc(sizeof(arv23Word));

    copiar(valor, no->word1);
    no->nInfo = 1;
    no->esq = esq;
    no->cent = cent;
    no->dir = dir;

    return no;
}

arv23Word *adicionaNo(arv23Word *no, char *valor, arv23Word *filho){

    if (compara(valor, no->word1) == 1){
        copiar(valor, no->word2);
        no->dir = filho;
    }
    
    else{
        copiar((*no).word1, no->word2);
        copiar(valor, no->word1);
        no->dir = no->cent;
        no->cent = filho;
    }

    no->nInfo = 2;
    return no;
}

arv23Word *criaNo(char *valor, arv23Word *esq, arv23Word *cent, arv23Word *dir){

    arv23Word *no = malloc(sizeof(arv23Word));
    copiar(valor, no->word1);

    no->dir = dir;
    no->cent = cent;
    no->esq = esq;
    no->nInfo = 1;

    return no;
}



arv23Word *quebraNo(arv23Word **no, char *valor, char *promover, arv23Word *subarvore){
    arv23Word *paux;

    if (compara(valor, (*no)->word2) == 1) {        //word2 está no meio e será promovido
       copiar((*no)->word2, promover);
       paux = (*no)->dir;
       (*no)->dir = NULL;  
       (*no)->nInfo = 1;     
       return criaNo(valor, paux, subarvore, NULL);
    }

    else if (compara(valor, (*no)->word1) == 1){    //valor está no meio e será promovido
        copiar(valor, promover);      
        paux = (*no)->dir;
        (*no)->nInfo = 1;
        return criaNo((*no)->word2,subarvore,  paux, NULL);
    }

    else{                                           //word1 está no meio e será promovido
        arv23Word *centro, *dir;
        copiar((*no)->word1, promover);
        paux = criaNo((*no)->word2, (*no)->cent, (*no)->dir, NULL); 
        copiar(valor, (*no)->word1);
        (*no)->nInfo = 1;
        (*no)->dir = NULL;
        (*no)->cent = subarvore;
        return paux;
    }
}




arv23Word *insere(arv23Word **raiz, char *valor, char *promove, arv23Word **pai){
    arv23Word *paux;
    if(*raiz == NULL){                   // Árvore Vazia
        *raiz = criaNo(valor, NULL, NULL, NULL);
        paux = NULL;
    }

    else{

        if(ehfolha(*raiz)){
            if((**raiz).nInfo == 1){        //Folha com 1 informação
                *raiz = adicionaNo(*raiz, valor, NULL);
                paux = NULL;
            }
            else{                           // Folha com 2 infos
                paux = quebraNo(raiz, valor, promove, NULL);
            }
        }

        else{

            if(compara(valor, (**raiz).word1)== -1){                                    // Não folha para esquerda
                paux = insere(&((*raiz)->esq), valor, promove, raiz);
            }

            else if(((*raiz)->nInfo == 1) || (compara(valor, (*raiz)->word2)== -1)){    //Não folha para o centro
                paux = insere(&((*raiz)->cent), valor, promove, raiz);
            }
            else paux = insere(&((*raiz)->dir), valor, promove, raiz);                 // não folha para a direita
        }

    }


    if(paux != NULL){                                           //Foi inserido com resquício(nó quebrado)

        if(pai == NULL){                                        //Se não existe pai acima
            *raiz = criaNo(promove, *raiz, paux, NULL);
            paux = NULL;
        }

        else if((*pai)->nInfo == 1){                           //Se existe um pai e ele tem espaço
            *pai = adicionaNo(*pai, promove, paux);
            paux = NULL;
        }

        else{
            char guardar[30];
            copiar(promove,guardar);
            paux = quebraNo(pai, guardar, promove, paux);     // Se existe um pai mas não tem espaço
        }
            
    }

    return paux;
}


void imprime(arv23Word *raiz){
    if(raiz){
        printf("<");
        if(raiz->nInfo == 2)
            printf("%s || %s",raiz->word1,raiz->word2);
        else
            printf("%s",raiz->word1);
        imprime(raiz->esq);
        imprime(raiz->cent);
        imprime(raiz->dir);
        printf(">");
    }
}

int main(){
    
    arv23Word *arvore;
    arvore = criaVazia();
    char promove[30];

    insere(&arvore, "aa", promove, NULL);
    insere(&arvore, "bb", promove, NULL);
    insere(&arvore, "cc", promove, NULL);
    insere(&arvore, "dd", promove, NULL);
    insere(&arvore, "ee", promove, NULL);
    insere(&arvore, "ff", promove, NULL);
    insere(&arvore, "gg", promove, NULL);
    insere(&arvore, "hh", promove, NULL);
    insere(&arvore, "ii", promove, NULL);
    insere(&arvore, "jj", promove, NULL);
    insere(&arvore, "kk", promove, NULL);
    insere(&arvore, "ll", promove, NULL);
    insere(&arvore, "mm", promove, NULL);
    insere(&arvore, "nn", promove, NULL);
    insere(&arvore, "oo", promove, NULL);
    imprime(arvore);
    printf("\n");

    return 0;
}