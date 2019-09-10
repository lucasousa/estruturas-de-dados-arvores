#include <stdio.h>
#include <stdlib.h>


/* Lista para armazenar as palavras em inglês*/
typedef struct lista{
    char info[20];
    struct lista *prox;
}lista;


typedef struct arvBinWord{
    char word[20];
    struct arvBinWord *esq;
    struct arvBinWord *dir;
    struct lista *lista;
}arvBinWord;

arvBinWord *criaVazia();
int esqDir(char *add, char *comp);
void insereLista(lista **l, char *str);
arvBinWord *Tratamento(arvBinWord **arv, char *var);
void imprimeLista(lista *l);
void imprime(arvBinWord *arvore);
void insereArvore(arvBinWord **arv, arvBinWord *no);
int compara(const char *s1, const char *s2); 
arvBinWord* busca(arvBinWord *arv, char *valor);
void copiar(char *string1, char *string2);

arvBinWord *buscaNo(arvBinWord *raiz, char *ch, arvBinWord **pai);
arvBinWord *removerNo(arvBinWord *raiz, char *ch);
void liberar(lista *l);

int main(int argc, char **args){
    arvBinWord *arvore;
    arvore = criaVazia();
    FILE *arquivo;
    arquivo = fopen(args[1],"r");
    char Unidade[50];
    fscanf(arquivo,"%s",Unidade);
    //printf("%s\n",Unidade);
    
    while(!feof(arquivo)){
        char nfstr[100];
        for(int i=0;i<100;i++)
            nfstr[i]='\0';
        fscanf(arquivo, "%s" , nfstr);
        //printf("%s\n",nfstr);
        Tratamento(&arvore,nfstr);
    }
    printf("Procurando a palavra PROGRAMA e imprimindo sua lista correspondente \n");
    imprimeLista(busca(arvore,"programa")->lista);
    printf("\n");
    printf("Removendo a palavra PROGRAMA e imprimindo a arvore novamente \n");
    arvore = removerNo(arvore, "programa");
    fclose(arquivo);
    imprime(arvore);
    printf("\n\n");

    return 0;
}



arvBinWord *criaVazia(){
    return NULL;
}

int esqDir(char *add, char *comp){
    while((*add == *comp) && (*add!='\0' && *comp!='\0')){
        add++;
        comp++;
    }
    if(*add == '\0' || *add < *comp) return 0;
    else return 1;
}

void copiar(char *string1, char *string2){
    while (*string1 != '\0'){
        *string2 = *string1;
        string1++;
        string2++;
    }
}

arvBinWord* criaNo(char* valor,char* ing){
    arvBinWord *no = malloc(sizeof(arvBinWord));
    no->lista = NULL;
    copiar(valor,no->word);
    insereLista(&(no->lista),ing);
    no->esq = NULL;
    no->dir = NULL;
    return no;
}

arvBinWord *Tratamento(arvBinWord **arv, char *var){
    arvBinWord *aux;
    aux = (arvBinWord *)malloc(sizeof(arvBinWord));
    aux->lista = NULL; //Inicializando a lista
    int pos=0;
    char ing[20];

    while(*var != ':'){
        ing[pos] = *var;
        var++;
        pos++;
    }

    ing[pos]='\0';

    var++;
    
    for(; *var != '\0' && *var!='\n' ; var++){
        int cont = 0;
        char pt[20];
        for (int i = 0; i < 20; i++)
            pt[i]='\0';
        
        for(; *var!=',' && *var!='\0'; var++, cont++){
            pt[cont] = *var;
        }
        pt[cont] = '\0';
        arvBinWord *aux = busca(*arv,pt);
        if(!aux)
            insereArvore(arv,criaNo(pt, ing));
        else
            insereLista(&(aux->lista),ing);
    }


}

void insereArvore(arvBinWord **arv, arvBinWord *no){
    if(*arv == NULL)
        *arv = no;
    else if(compara(no->word, (*arv)->word) == 1){
        insereArvore(&((*arv)->dir), no);
    }
    else if (compara(no->word, (*arv)->word) == -1){
        insereArvore(&((*arv)->esq), no);
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

void imprime(arvBinWord *arvore){
    if(arvore){
        printf("<");
        printf("%s ", arvore->word);
        printf("(");
        imprimeLista(arvore->lista);
        printf(")");
        imprime(arvore->esq);
        imprime(arvore->dir);
        printf(">");
    }
}

void imprimeLista(lista *l){
    if(l == NULL) return;
    else{
        printf("%s ", l->info);
        imprimeLista(l->prox);
    }
    
}

arvBinWord* busca(arvBinWord *arv, char *valor){
    if(!arv){
        return NULL;
    }
    if(compara(arv->word, valor)==0){
        return arv;
    }
    else if(compara(arv->word, valor) == 1){
        busca(arv->esq, valor);
    }else{
        busca(arv->dir, valor);
    }
}

int compara(const char *s1, const char *s2){
    int i=0;
    while ( (s1[i] == s2[i]) && (s1[i] != '\0') && (s2[i] != '\0')){
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

void liberar(lista *l){
    if(l == NULL)
        return;

    liberar(l->prox);
    free(l);
}

arvBinWord *buscaNo(arvBinWord *raiz, char *ch, arvBinWord **pai){
    arvBinWord *atual = raiz;
    *pai = NULL;
    while(atual){
        if(compara(atual->word, ch)==0)
            return atual;
        *pai = atual;
        if(compara(atual->word, ch)==1)
            atual = atual->esq;
        else
            atual = atual->dir;
    }
    return NULL;
}

arvBinWord *removerNo(arvBinWord *raiz, char *ch){
    arvBinWord *pai, *no, *p, *q;
    no = buscaNo(raiz, ch, &pai);
    
    //No não encontrado
    if( no == NULL)
        return raiz;

    //Verifica se tem os dois descendentes
    if( !no->esq || !no->dir ){
        if( !no->esq)
            q = no->dir;
        else
            q = no->esq;
    }else{
        p = no;
        q = no->esq;
        //Busca o mais a direta
        while(q->dir){
            p = q;
            q = q->dir;
        }
        if( p != no){
            p->dir = q->esq;
            q->esq = no->esq;
        }
        q->dir = no->dir;
    }
    if(!pai){
        liberar(no->lista);
        free(no);
        return q;
    }
    if(compara((pai)->word, ch)==1)
        (pai)->esq = q;
    else
        (pai)->dir = q;

    liberar(no->lista);
    free(no);
    return raiz;
}