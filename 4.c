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
    struct lista *l1, *l2;
}arv23Word;

int compara(const char *s1, const char *s2);
void copiar(char *string1, char *string2);
arv23Word *criaVazia();
int ehfolha(arv23Word *no);
arv23Word *alocando(char *valor, arv23Word *esq, arv23Word *cent, arv23Word *dir);
arv23Word *adicionaNo(arv23Word *no, char *valor, lista *l, arv23Word *filho);
arv23Word *criaNo(char *valor, lista *l, arv23Word *esq, arv23Word *cent, arv23Word *dir);
arv23Word *quebraNo(arv23Word **no, char *valor, char *promover, lista **lpromove, arv23Word *subarvore);
void imprime(arv23Word *raiz);
arv23Word *insere(arv23Word **raiz, char *valor, char *promove, lista **lpromove, arv23Word **pai);
arv23Word *busca(arv23Word *arv, char *str); 

void Tratamento(arv23Word **arv, char *var, char *promove, lista **lpromove, arv23Word **pai);
void imprimeLista(lista *l);
void insereLista(lista **l, char *str);
void liberar(lista *l);



int main(){
    
    arv23Word *arvore;
    arvore = criaVazia();
    lista *listapromover = NULL;
    char promove[30];
    Tratamento(&arvore, "bag:saco,meusaco\0", promove, &listapromover, NULL);
    Tratamento(&arvore, "scheme:programa\0", promove, &listapromover, NULL);
    Tratamento(&arvore, "software:programa,aplicativo\0", promove, &listapromover, NULL);
    
    imprime(arvore);
    printf("\n");

    return 0;
}

arv23Word *busca(arv23Word *arv, char *str){
    if(!arv)
        return NULL;
    int comp1, comp2;
    if(arv->nInfo == 1){
        comp1 = compara(str, arv->word1);
        comp2 = 2;
    }else{
        comp1 = compara(str, arv->word1);
        comp2 = compara(str, arv->word2);
    }
    if(!comp1 || !comp2)
        return arv;
    else{
        if(comp1 == -1)
            return busca(arv->esq,str);
        if(comp2 == 1)
            return busca(arv->dir, str);
        return busca(arv->cent,str);
    }
    
}


void Tratamento(arv23Word **arv, char *var, char *promove, lista **lpromove, arv23Word **pai){
    /*arv23Word *aux;
    aux = (arv23Word *)malloc(sizeof(arv23Word));
    aux->l1 = NULL; //Inicializando a lista
    aux->l2 = NULL; //Inicializando a lista*/
    int pos = 0;
    char ing[20];

    while(*var != ':'){
        ing[pos] = *var;
        var++;
        pos++;
    }

    ing[pos]='\0';

    var++;
    
    for(; *var != '\0' && *var!='\n' && *var != '\0' ; var++){
        int cont = 0;
        char pt[20];
        for (int i = 0; i < 20; i++)
            pt[i]='\0';
        
        for(; *var!=',' && *var!='\0'; var++, cont++){
            pt[cont] = *var;
        }
        pt[cont] = '\0';
        arv23Word *aux = busca(*arv,pt);
        if(!aux){
            insere(arv, pt, promove, lpromove, pai);
            aux = busca(*arv,pt);
        }
        if(compara(aux->word1, pt) == 0)
            insereLista(&(aux->l1),ing);
        else
            insereLista(&(aux->l2),ing);
    }


}

void imprimeLista(lista *l){
    if(l == NULL) return;
    else{
        printf("%s ", l->info);
        imprimeLista(l->prox);
    }
}

void insereLista(lista **l, char *str){
    if(*l == NULL){
        *l = (lista*)malloc(sizeof(lista));
        copiar(str, (*l)->info);
        (*l)->prox = NULL;
    }else
        insereLista(&((*l)->prox), str);
}







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
arv23Word *adicionaNo(arv23Word *no, char *valor, lista *l, arv23Word *filho){

    if (compara(valor, no->word1) == 1){
        copiar(valor, no->word2);
        no->l2 = l;
        no->dir = filho;
    }
    
    else{
        copiar((*no).word1, no->word2);
        no->l2 = no->l1;
        no->l1 = l;
        copiar(valor, no->word1);
        no->dir = no->cent;
        no->cent = filho;
    }

    no->nInfo = 2;
    return no;
}
arv23Word *criaNo(char *valor, lista *l, arv23Word *esq, arv23Word *cent, arv23Word *dir){

    arv23Word *no = malloc(sizeof(arv23Word));
    copiar(valor, no->word1);
    no->l1 = l;
    no->l2 = NULL;
    no->dir = dir;
    no->cent = cent;
    no->esq = esq;
    no->nInfo = 1;

    return no;
}
arv23Word *quebraNo(arv23Word **no, char *valor, char *promover, lista **lpromove, arv23Word *subarvore){
    arv23Word *paux;

    if (compara(valor, (*no)->word2) == 1) {        //word2 está no meio e será promovido
       copiar((*no)->word2, promover);
       *lpromove = (*no)->l2;
       paux = (*no)->dir;
       (*no)->dir = NULL;  
       (*no)->nInfo = 1;     
       return criaNo(valor, NULL, paux, subarvore, NULL);
    }

    else if (compara(valor, (*no)->word1) == 1){    //valor está no meio e será promovido
        copiar(valor, promover);
        *lpromove = NULL;      
        paux = (*no)->dir;
        (*no)->nInfo = 1;
        return criaNo((*no)->word2, (*no)->l2,subarvore, paux, NULL);
    }

    else{                                           //word1 está no meio e será promovido
        arv23Word *centro, *dir;
        copiar((*no)->word1, promover);
        *lpromove = (*no)->l1;
        paux = criaNo((*no)->word2, (*no)->l2, (*no)->cent, (*no)->dir, NULL); 
        copiar(valor, (*no)->word1);
        (*no)->l1 = NULL;
        (*no)->nInfo = 1;
        (*no)->dir = NULL;
        (*no)->cent = subarvore;
        return paux;
    }
}
arv23Word *insere(arv23Word **raiz, char *valor, char *promove, lista **lpromove, arv23Word **pai){
    arv23Word *paux;
    if(*raiz == NULL){                   // Árvore Vazia
        *raiz = criaNo(valor, NULL, NULL, NULL, NULL);
        paux = NULL;
    }
    else{
        if(ehfolha(*raiz)){
            if((**raiz).nInfo == 1){        //Folha com 1 informação
                *raiz = adicionaNo(*raiz, valor, NULL, NULL);
                paux = NULL;
            }else                           // Folha com 2 infos
                paux = quebraNo(raiz, valor, promove, lpromove, NULL);
        }

        else{
            if(compara(valor, (**raiz).word1)== -1)                                    // Não folha para esquerda
                paux = insere(&((*raiz)->esq), valor, promove, lpromove, raiz);
            else if(((*raiz)->nInfo == 1) || (compara(valor, (*raiz)->word2)== -1))    //Não folha para o centro
                paux = insere(&((*raiz)->cent), valor, promove, lpromove, raiz);
            else paux = insere(&((*raiz)->dir), valor, promove, lpromove, raiz);                 // não folha para a direita
        }
    }
    if(paux != NULL){                                           //Foi inserido com resquício(nó quebrado)
        if(pai == NULL){                                        //Se não existe pai acima
            *raiz = criaNo(promove, *lpromove, *raiz, paux, NULL);
            paux = NULL;
        }
        else if((*pai)->nInfo == 1){                           //Se existe um pai e ele tem espaço
            *pai = adicionaNo(*pai, promove, *lpromove, paux);
            paux = NULL;
        }
        else{
            char guardar[30];
            copiar(promove, guardar);
            paux = quebraNo(pai, guardar, promove, lpromove, paux);     // Se existe um pai mas não tem espaço
        }     
    }
    return paux;
}
void imprime(arv23Word *raiz){
    if(raiz){
        printf("<");
        if(raiz->nInfo == 2){
            printf("%s ",raiz->word1);
            printf("{"); imprimeLista(raiz->l1); printf("}");
            printf("|| %s",raiz->word2);
            printf("{"); imprimeLista(raiz->l2); printf("}");
        }else{
            printf("%s ",raiz->word1);
            printf("{"); imprimeLista(raiz->l1); printf("}");
        }
        imprime(raiz->esq);
        imprime(raiz->cent);
        imprime(raiz->dir);
        printf(">");
    }
}


/* 
char* menorinfodir(arv23Word *raiz){
    if(ehfolha(raiz)) return raiz->word1;
    else return menorinfodir(raiz->esq);
}
char* maiorinfoesq(arv23Word* raiz){
    if(ehfolha(raiz)){
        if (raiz->nInfo == 2) return raiz->word2;
        else return raiz->word1;
    }else{
        if(raiz->nInfo == 2) return maiorinfoesq(raiz->dir);
        else return maiorinfoesq(raiz->cent);
    }
}

int remover(arv23Word **raiz, char *valor, arv23Word **pai){
    int removeu=0;
    if(*raiz == NULL) // árvore vazia
        return 0;
    
    if(ehfolha(*raiz)){ //nó atual é uma folha(caso bosta)
        if((*raiz)->nInfo == 2 ){   // folha com 2 infos
            if(compara((*raiz)->word2, valor) == 0){
                copiar("",(*raiz)->word2);
                (*raiz)->nInfo--;
                removeu = 1;
            }
            else if(compara((*raiz)->word1, valor) == 0){
                copiar((*raiz)->word2,(*raiz)->word1);
                copiar("",(*raiz)->word2);
                (*raiz)->nInfo--;
                removeu = 1;
            }
        }else{  //folha com 1 info
            if(compara((*raiz)->word1, valor) == 0){ // valor encontrado
                if(*pai == NULL){ // sem pai(raiz da arvore)
                    free(*raiz);
                    *raiz = NULL;
                    removeu = 1;
                }else if(*raiz == (*pai)->esq){ //estou no filho da esquerda?
                    copiar((*pai)->word1, (*raiz)->word1);
                    copiar(menorInfoDir((*pai)->cent, &no, &paiNo),(*pai)->word1);
                    removeu = 1;
                    if(no->nInfo == 2){
                        copiar(no->word2, no->word1);
                        no->word2 =  0;
                        no->nInfo == 1;
                    }
                    else{
                        if(paiNo->nInfo == 1){
                            copiar(no->word1, (*raiz)->word2 );
                            (*raiz)->nInfo = 2;
                            free(no);
                            *pai = *raiz;
                            removeu = 1;
                        }
                    }
                }
            }

        else{
            copiar(no->word1, (*pai)->word1);
            copiar(no->word1, (*pai)->word2);
            paiNo->word2 = menorInfoDir((*paiNo->dir, &no1, &paiNo));

            if (no1->nInfo == 2){
                copiar(no1->word1, no1->word2);
                no1->nInfo = 1;
            }
            else{
                copiar(paiNo->word2, no->word2);
                paiNo->word2 = 0;
                free(no);
            }
        }

        else if(*raiz ==  (*pai)->cent){
            removeu = 1;
            if((*pai)->nInfo == 1){
                if( ((*pai)->esq)->nInfo == 2){
                    copiar((*pai)->word2, (*raiz)->word1);
                    copiar((*pai)->esq->word2, (*pai)->word1);
                    (*pai)->esq->word2 = 0;
                    (*pai)->esq->nInfo = 1;
                }
                else{
                    copiar((*pai)->word1, ((*pai)->esq)->word2);
                    free(*raiz);
                    ((*pai)->esq)->nInfo = 2;
                    *pai = (*pai)->esq;
                }
            }
            
            else{
                copiar((*pai)->word2, (*raiz)->word1);
                (*pai)->word2 = menorInfoDir((*pai)->dir, &no, &paiNo);
                if(no->nInfo == 2){
                    copiar(no->word2, no->word1);
                    no->word2 = 0;
                    no->nInfo = 1;
                }
                else{
                    copiar((*pai)->word2, (*raiz)->word2);
                    (*pai)->word2 = 0;
                    (*pai)->nInfo =1;
                    free(no);
                    (*pai)->dir = NULL;
                }
            }
        }
        else{
            removeu = 1;
            info = maiorInfoEsq((*pai)->cent, &no, &paiNo);
            if(no->nInfo == 1){
                copiar((*pai)->word2, no->word2);
                *pai)->word2 = 0;
                *pai)->nInfo = 1;
                no->nInfo = 2;
                free(*raiz);
                *raiz = NULL;
            }
            else{
                copiar((*pai)->word2, (*raiz)->word1);
                copiar(info, (*pai)->word2);
                no->word2 = 0;
                no->nInfo = 1;
            }
        }

        else{
            if(compara(valor, (*raiz)->word1) == -1)
                removeu = remover( &((*raiz)->esq), valor, raiz);
            else if(compara(valor, (*raiz)->word1) == 0){
                (*raiz)->word1 = menorInfoDir((*raiz)->cent, &no, &paiNo);
                remover(&((*raiz)->cent), (*raiz)->word1, raiz);
                removeu = 1;
            }
            else{
                if((*raiz)->nInfo == 1 || compara(valor, (*raiz)->word2)) == -1)
                    removeu = remover(&((*raiz)->cent), valor, raiz);
                else if(compara(valor, (*raiz)->word2) == 0){
                    (*raiz)->word2 = menorInfoDir((*raiz)->dir, &no, &paiNo);
                    remover(&((*raiz)->dir), (*raiz)->word2, raiz);
                    removeu = 1;
                }
                else
                    removeu = remover(&((*raiz)->dir), valor, raiz);
                }
            }
        }
    }
} */

