#include <stdio.h>
#include <stdlib.h>
#include<time.h>


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
void Tratamento(arvBinWord **arv, char *var);
void imprimeLista(lista *l);
void imprime(arvBinWord *arvore);
void insereArvore(arvBinWord **arv, arvBinWord *no);
int compara(const char *s1, const char *s2); 
arvBinWord* busca(arvBinWord *arv, char *valor);
void copiar(char *string1, char *string2);
arvBinWord *buscaNo(arvBinWord *raiz, char *ch, arvBinWord **pai);
arvBinWord *removerNo(arvBinWord *raiz, char *ch);
void liberar(lista *l);
void liberarArv(arvBinWord *arv);
int menu(int *op);


int main(int argc, char **args){
    arvBinWord *arvore;
    arvore = criaVazia();
    FILE *arquivo;
    char Unidade[50];
    clock_t finish, start;
    int p;
    while(menu(&p)){
        char palavra[30];
        switch(p){
            case 1:
                printf("Digite o nome do arquivo que contém a unidade >> ");
                setbuf(stdin, NULL);
                scanf("%s",Unidade);
                arquivo = fopen(Unidade, "r");
                start = clock();
                fscanf(arquivo,"%s",Unidade);
                while(!feof(arquivo)){
                    char nfstr[100];
                    for(int i=0;i<100;i++)
                        nfstr[i]='\0';
                    fscanf(arquivo, "%s" , nfstr);
                    Tratamento(&arvore,nfstr);
                }
                finish = clock();
                printf("\nTempo de Inserção da Árvore: %lf \n",(double)(finish-start)*1000/CLOCKS_PER_SEC);
            break;
            case 2:
                imprime(arvore);
                printf("\n\n");
            break;
            case 3:
                printf("Digite a palavra a ser buscada >> ");
                setbuf(stdin, NULL);
                scanf("%s",palavra);
                arvBinWord *aux = busca(arvore, palavra);
                printf("\n");
                if(aux){
                    printf("<< Lista Equivalente >> \n");
                    imprimeLista(aux->lista);
                }
                else{
                    printf("Palavra nao encontrada \n");
                }
                printf("\n");
                break;
            case 4:
                printf("Digite a palavra a ser removida >> "); 
                setbuf(stdin, NULL);
                scanf("%s",palavra);
                arvore = removerNo(arvore, palavra);
                printf("\n");
            break;
            case 5:
                printf("árvore limpa com sucesso\n");
                liberarArv(arvore);
                arvore = NULL;
            break;
            default:
            break;
        }
    }
    arvore = removerNo(arvore, "programa");
    fclose(arquivo);
   
    printf("\n\n");

    return 0;
}
void liberarArv(arvBinWord *arv){
    if(!arv) return;
    liberarArv(arv->esq);
    liberarArv(arv->dir);
    free(arv);
}

int menu(int *op){
    printf(" _______________________________ \n");
    printf("| 1 - Ler uma unidade           |\n");
    printf("| 2 - Mostrar todas as palavras |\n");
    printf("| 3 - Buscar uma palavra        |\n");
    printf("| 4 - Remover uma palavra       |\n");
    printf("| 5 - Limpar a árvore           |\n");
    printf("| 0 - Sair                      |\n");
    printf("|_______________________________|\n");
    printf("\nDigite uma opcao >> ");
    scanf("%d",op);
    printf("\n");
    return *op;
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

void Tratamento(arvBinWord **arv, char *var){
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
        imprime(arvore->esq);
        printf("%s ", arvore->word);
        printf("(");
        imprimeLista(arvore->lista);
        printf(")");
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