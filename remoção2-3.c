int remover(arv23Word **raiz, char *valor, arv23Word **pai){
    int removeu=0;
    
    if(*raiz != NULL){
        if(ehfolha(*raiz)){
            if((*raiz)->info == 2 ){
                if(compara((*raiz)->word2, valor) == 0){
                    (*raiz)->word2 = 0;
                    (*raiz)->nInfo = --;
                    removeu = 1;
                }
                else if(compara((*raiz)->word1, valor) == 0){
                    (*raiz)->word1 = (*raiz)->word2;
                    (*raiz)->word2 = 0;
                    (*raiz)->nInfo --;
                    removeu = 1;
                }
            }
        }
    }

    else{
        if(compara((*raiz)->word1, valor) == 0){
            if(*pai == NULL){
                free(*raiz);
                *raiz = NULL;
                removido = 1;
            }
            else if(compara(*raiz, (*pai)->esq) == 0){
                copiar((*pai)->word1, (*raiz)->word1);
                (*pai)->word1 = menorInfoDir((*pai)->cent, &no, &paiNo);
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
            if(compara(valor, (*raiz)->word1) == -1){
                removeu = remover( &((*raiz)->esq), valor, raiz);
            }
            else if(compara(valor, (*raiz)->word1) == 0){
                (*raiz)->word1 = menorInfoDir((*raiz)->cent, &no, &paiNo);
                remover(&((*raiz)->cent), (*raiz)->word1, raiz);
                removeu = 1;
            }
            else{
                if((*raiz)->nInfo == 1 || compara(valor, (*raiz)->word2)) == -1){
                    removeu = remover(&((*raiz)->cent), valor, raiz);
                }
                else if(compara(valor, (*raiz)->word2) == 0){
                    (*raiz)->word2 = menorInfoDir((*raiz)->dir, &no, &paiNo);
                    remover(&((*raiz)->dir), (*raiz)->word2, raiz);
                    removeu = 1;
                }
                else{
                    removeu = remover(&((*raiz)->dir), valor, raiz);
                }
            }
        }
    }
}