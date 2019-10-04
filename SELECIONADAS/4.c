#include <stdio.h>
#include <stdlib.h>
#include<time.h>

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

int menu(int *op);
void maiorInfoEsq(arv23Word *raiz, arv23Word **no, arv23Word **paiNo);
void menorInfoDir(arv23Word *raiz, arv23Word **no, arv23Word **paiNo);
void copyList(lista **l, lista *lis);
int arvore23Remover(arv23Word **raiz, char *palavra, arv23Word **pai);
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
void liberarv();

void Tratamento(arv23Word **arv, char *var, char *promove, lista **lpromove, arv23Word **pai);
void imprimeLista(lista *l);
void insereLista(lista **l, char *str);
void liberar(lista **l);
void insercaoListas(arv23Word **arv, char *var);


int main(int argc, char **args){
    arv23Word *arvore;
    arvore = NULL;
    lista *listapromover = NULL;
    char promove[30];
    FILE *arquivo, *arq2;
    char Unidadenome[50];
    char Unidade[50];
    char palavra[50];
    arv23Word *pai = NULL;
    clock_t finish, start;


    int p;
    while (menu(&p)){
        double vettempos[30];
        char palavra[30];
        switch (p){
            case 1:
                printf("Digite o nome do arquivo que contém a unidade >> ");
                scanf("%s", Unidade);
                start = clock();
                arquivo = fopen(Unidade, "r");
                fscanf(arquivo, "%s", Unidadenome);
                while (!feof(arquivo)){
                    char nfstr[100];
                    for (int i = 0; i < 100; i++)
                        nfstr[i] = '\0';
                    fscanf(arquivo, "%s", nfstr);
                    Tratamento(&arvore, nfstr, promove, &listapromover, &pai);
                }
                fclose(arquivo);
                arq2 = fopen(Unidade,"r");
                fscanf(arq2, "%s", Unidadenome);
                while (!feof(arq2)){
                    char nfstr[100];
                    for (int i = 0; i < 100; i++)
                        nfstr[i] = '\0';
                    fscanf(arq2, "%s", nfstr);
                    insercaoListas(&arvore, nfstr);
                }
                fclose(arq2);
                finish = clock();
                break;
            case 2:
                printf("\n");
                imprime(arvore);
                printf("\n");
                break;
            case 3:
                printf("Digite a palavra a ser buscada >> ");
                setbuf(stdin, NULL);
                scanf("%s", palavra);
                arv23Word* aux = busca(arvore, palavra);

                printf("\n");
                if(!aux)
                    printf("Palavra não encontrada!\n");
                else if(!compara(aux->word1, palavra)){
                    printf("<< Lista equivalente >>\n");
                    imprimeLista(aux->l1);
                }
                else{
                    printf("<< Lista equivalente >>\n");
                    imprimeLista(aux->l2);
                }
                printf("\n");
                break;
            case 4:
                printf("Digite a palavra a ser removida >> ");
                setbuf(stdin,NULL);
                scanf("%s", palavra);
                arvore23Remover(&arvore, palavra, &pai);
                printf("\n");
                break;
                
            default:
                break;
        }
    }
    fclose(arquivo);

    printf("\n\n");

    return 0;
}
int menu(int *op){
    printf(" _______________________________ \n");
    printf("| 1 - Ler uma unidade           |\n");
    printf("| 2 - Mostrar todas as palavras |\n");
    printf("| 3 - Buscar uma palavra        |\n");
    printf("| 4 - Remover uma palavra       |\n");
    printf("| 0 - Sair                      |\n");
    printf("|_______________________________|\n");
    printf("Informe uma opcao >> ");
    scanf("%d", op);
    return *op;
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

void insercaoListas(arv23Word **arv, char *var){
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
        if(compara(aux->word1, pt) == 0)
            insereLista(&(aux->l1),ing);
        else
            insereLista(&(aux->l2),ing);
    }
}


void Tratamento(arv23Word **arv, char *var, char *promove, lista **lpromove, arv23Word **pai){
    int pos = 0;
    char ing[20];
    arv23Word* raiz = *arv;
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
        arv23Word* aux = busca(raiz,pt);
        if(aux == NULL)
            insere(arv, pt, promove, lpromove, NULL);
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
            printf("%s ", raiz->word1);
            printf("{"); imprimeLista(raiz->l1); printf("}");
            printf("|| %s",raiz->word2);
            printf("{"); imprimeLista(raiz->l2); printf("}");
            imprime(raiz->esq);
            imprime(raiz->cent);
            imprime(raiz->dir);
        }else{
            printf("%s ",raiz->word1);
            printf("{"); imprimeLista(raiz->l1); printf("}");
            imprime(raiz->esq);
            imprime(raiz->cent);
        }
        printf(">");
    }
}
void liberar(lista **lista){
    if (*lista != NULL){
        liberar(&(*lista)->prox);
        free(*lista);
    }
    *lista = NULL;
}
void menorInfoDir(arv23Word *raiz, arv23Word **no, arv23Word **paiNo)
{
    if (raiz->esq != NULL)
    {
        *paiNo = raiz;
        menorInfoDir(raiz->esq, no, paiNo);
    }
    else
    {
        *no = raiz;
    }
}

void maiorInfoEsq(arv23Word *raiz, arv23Word **no, arv23Word **paiNo)
{
    if (raiz->dir != NULL)
    {
        *paiNo = raiz;
        maiorInfoEsq(raiz->dir, no, paiNo);
    }
    else
    {
        *no = raiz;
    }
}

void copyList(lista **l, lista *lis){
    if (lis != NULL)
    {
        insereLista(l, lis->info);
        copyList(l, lis->prox);
    }
}

int arvore23Remover(arv23Word **raiz, char *palavra, arv23Word **pai){
    int removeu = 0;
    arv23Word *no = NULL, *no1, *paiNo = NULL, *paiNo1 = NULL, **aux;
    aux = (arv23Word **)malloc(sizeof(arv23Word *));
    no1 = (arv23Word *)malloc(sizeof(arv23Word));
    if (*raiz != NULL){
        if (ehfolha(*raiz))
        {
            if ((*raiz)->nInfo == 2)
            {
                if (compara(palavra, (*raiz)->word2) == 0){
                    copiar(" ", (*raiz)->word2);
                    liberar(&(*raiz)->l2);
                    (*raiz)->nInfo = 1;
                    removeu = 1;
                }
                else if (compara(palavra, (*raiz)->word1) == 0)
                {
                    copiar((*raiz)->word2, (*raiz)->word1);
                    liberar(&(*raiz)->l1);
                    copyList(&(*raiz)->l1, (*raiz)->l2);
                    liberar(&(*raiz)->l2);
                    copiar(" ", (*raiz)->word2);
                    (*raiz)->nInfo = 1;
                    removeu = 1;
                }
            }
            else if (compara(palavra, (*raiz)->word1) == 0)
            {
                if (*pai == NULL)
                {
                    free(*raiz);
                    *raiz = NULL;
                    removeu = 1;
                }
                else if (*raiz == (*pai)->esq)
                {
                    copiar((*pai)->word1, (*raiz)->word1);
                    liberar(&(*raiz)->l1);
                    copyList(&(*raiz)->l1, (*pai)->l1);
                    paiNo = *pai;
                    menorInfoDir((*pai)->cent, &no, &paiNo);
                    copiar(no->word1, (*pai)->word1);
                    liberar(&(*pai)->l1);
                    copyList(&(*pai)->l1, no->l1);
                    removeu = 1;
                    if (no->nInfo == 2)
                    {
                        copiar(no->word2, no->word1);
                        liberar(&no->l1);
                        copyList(&no->l1, no->l2);
                        copiar(" ", no->word2);
                        liberar(&no->l2);
                        no->nInfo = 1;
                    }
                    else
                    {
                        if (paiNo->nInfo == 1)
                        {
                            copiar(no->word1, (*raiz)->word2);
                            liberar(&(*raiz)->l2);
                            copyList(&(*raiz)->l2, no->l1);
                            (*raiz)->nInfo = 2;
                            liberar(&no->l1);
                            free(no);
                            *pai = *raiz;
                        }
                        else
                        {
                            copiar(paiNo->word2, no->word1);
                            liberar(&no->l1);
                            copyList(&no->l1, paiNo->l2);
                            paiNo1 = paiNo;
                            menorInfoDir(paiNo->dir, &no1, &paiNo1);
                            copiar(no1->word1, paiNo->word2);
                            liberar(&paiNo->l2);
                            copyList(&paiNo->l2, no1->l1);
                            if (no1->nInfo == 2)
                            {
                                copiar(no1->word2, no1->word1);
                                liberar(&no1->l1);
                                copyList(&no1->l1, no1->l2);
                                copiar(" ", no1->word2);
                                liberar(&no1->l2);
                                no1->nInfo = 1;
                            }
                            else
                            {
                                copiar(paiNo->word2, no->word2);
                                liberar(&no->l2);
                                copyList(&no->l2, paiNo->l2);
                                no->nInfo = 2;
                                copiar(" ", paiNo->word2);
                                liberar(&paiNo->l2);
                                paiNo->nInfo = 1;
                                liberar(&no1->l1);
                                liberar(&no1->l2);
                                free(no1);
                                paiNo1->dir = NULL;
                            }
                        }
                    }
                }
                else if (*raiz == (*pai)->cent)
                {
                    removeu = 1;
                    if ((*pai)->nInfo == 1)
                    {
                        if (((*pai)->esq)->nInfo == 2)
                        {
                            copiar((*pai)->word1, (*raiz)->word1);
                            liberar(&(*raiz)->l1);
                            copyList(&(*raiz)->l1, (*pai)->l1);
                            copiar(((*pai)->esq)->word2, (*pai)->word1);
                            liberar(&(*pai)->l1);
                            copyList(&(*pai)->l1, ((*pai)->esq)->l2);
                            copiar(" ", ((*pai)->esq)->word2);
                            liberar(&((*pai)->esq)->l2);
                            ((*pai)->esq)->nInfo = 1;
                        }
                        else
                        {
                            copiar((*pai)->word1, ((*pai)->esq)->word2);
                            liberar(&((*pai)->esq)->l2);
                            copyList(&((*pai)->esq)->l2, (*pai)->l1);
                            liberar(&(*raiz)->l1);
                            liberar(&(*raiz)->l2);
                            free(*raiz);
                            ((*pai)->esq)->nInfo = 2;
                            *aux = (*pai)->esq;
                            liberar(&(*pai)->l1);
                            liberar(&(*pai)->l1);
                            free(*pai);
                            *pai = *aux;
                        }
                    }
                    else
                    {
                        copiar((*pai)->word2, (*raiz)->word1);
                        liberar(&(*raiz)->l1);
                        copyList(&(*raiz)->l1, (*pai)->l2);
                        paiNo = *pai;
                        menorInfoDir((*pai)->dir, &no, &paiNo);
                        copiar(no->word1, (*pai)->word2);
                        liberar(&(*pai)->l2);
                        copyList(&(*pai)->l2, no->l1);
                        if (no->nInfo == 2)
                        {
                            copiar(no->word2, no->word1);
                            liberar(&no->l1);
                            copyList(&no->l1, no->l2);
                            copiar(" ", no->word2);
                            liberar(&no->l2);
                            no->nInfo = 1;
                        }
                        else
                        {
                            (*raiz)->nInfo = 2;
                            copiar((*pai)->word2, (*raiz)->word2);
                            liberar(&(*raiz)->l2);
                            copyList(&(*raiz)->l2, (*pai)->l2);
                            copiar(" ", (*pai)->word2);
                            liberar(&(*pai)->l2);
                            (*pai)->nInfo = 1;
                            free(no);
                            (*pai)->dir = NULL;
                        }
                    }
                }
                else
                {
                    removeu = 1;
                    paiNo = *pai;
                    maiorInfoEsq((*pai)->cent, &no, &paiNo);
                    if (no->nInfo == 1)
                    {
                        copiar((*pai)->word2, no->word2);
                        liberar(&no->l2);
                        copyList(&no->l2, (*pai)->l2);
                        copiar(" ", (*pai)->word2);
                        liberar(&(*pai)->l2);
                        (*pai)->nInfo = 1;
                        no->nInfo = 2;
                        free(*raiz);
                        *raiz = NULL;
                    }
                    else
                    {
                        copiar((*pai)->word2, (*raiz)->word1);
                        liberar(&(*raiz)->l1);
                        copyList(&(*raiz)->l1, (*pai)->l2);
                        copiar(no->word2, (*pai)->word2);
                        liberar(&(*pai)->l2);
                        copyList(&(*pai)->l2, no->l2);
                        copiar(" ", no->word2);
                        liberar(&no->l2);
                        no->nInfo = 1;
                    }
                }
            }
        }
        else
        {
            if (compara(palavra, (*raiz)->word1) == -1)
            {
                removeu = arvore23Remover(&(*raiz)->esq, palavra, raiz);
            }
            else if (compara(palavra, (*raiz)->word1) == 0)
            {
                paiNo = *raiz;
                menorInfoDir((*raiz)->cent, &no, &paiNo);
                copiar(no->word1, (*raiz)->word1);
                liberar(&(*raiz)->l1);
                copyList(&(*raiz)->l1, no->l1);
                imprime(*raiz);
                printf("\n");
                arvore23Remover(&(*raiz)->cent, (*raiz)->word1, raiz);
                printf("\n");
                imprime(*raiz);
                removeu = 1;
            }
            else if ((*raiz)->nInfo== 1 || compara(palavra, (*raiz)->word2) == -1)
            {
                removeu = arvore23Remover(&(*raiz)->cent, palavra, raiz);
            }
            else if (compara(palavra, (*raiz)->word2)==0)
            {
                paiNo = *pai;
                menorInfoDir((*pai)->dir, &no, &paiNo);
                copiar(no->word1, (*raiz)->word2);
                liberar(&(*raiz)->l2);
                copyList(&(*raiz)->l2, no->l1);
                arvore23Remover(&(*raiz)->dir, (*raiz)->word2, raiz);
                removeu = 1;
            }
            else
            {
                removeu = arvore23Remover(&(*raiz)->dir, palavra, raiz);
            }
        }
    }
    return removeu;
}
