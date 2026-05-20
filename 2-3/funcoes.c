#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "prototipos.h"

Arvore *inicializar(){
    return NULL;
}

Musica *inicializarM(){
    return NULL;
}

void deixarMaiusculo(char *str){
    for(int i=0; str[i]!='\0'; i++) str[i] = toupper((unsigned char) str[i]);
}

Arvore *alocar(TipoDado tipo){
    Arvore *no = (Arvore *)malloc(sizeof(Arvore));
    if(no){
        no->tipo = tipo;
        no->nInfos = 0;
        no->esq = no->cen = no->dir = NULL; 
    }
    return (no);
}

Musica *alocarM(){
    Musica *no = (Musica *)malloc(sizeof(Musica));
    if(no) no->prox = NULL;
    return (no);
}

int ehFolha(Arvore *R){
    int i = 0;
    if(!R->esq) i = 1;
    return (i);
}

void preencheInfo(TipoDado tipo, DadoUnion *info){
    if(tipo == ARTISTA){
        printf("Nome: ");
        scanf(" %[^\n]", info->ARTISTA.nome);
        printf("Estilo musical: ");
        scanf(" %[^\n]", info->ARTISTA.estiloMusical);
        int op = 0;
        while (op < 1 || op > 4) {
            printf("Tipo (1-Cantor, 2-Dupla, 3-Banda, 4-Grupo): ");
            scanf("%d", &op); printf("\n");
        }
        info->ARTISTA.tipo = (TipoArtista) op;
        info->ARTISTA.numeroAlbuns = 0;
        info->ARTISTA.albuns = inicializar(); 
    }else if(tipo == ALBUM){
        printf("Título: ");
        scanf(" %[^\n]", info->ALBUM.nome);
        printf("Ano de lançamento: ");
        scanf("%d", &info->ALBUM.anoLancamento);
        while(info->ALBUM.anoLancamento > 2026){
            printf("[ERRO] Insira um ano válido.\n");
            printf("Ano de lançamento: ");
            scanf("%d", &info->ALBUM.anoLancamento);
        }
        info->ALBUM.numeroMusicas = 0;
        info->ALBUM.musicas = inicializarM();
    }else printf("[ERRO] Tipo inválido.\n");
}

Arvore *criaNo(DadoUnion info, Arvore *Fesq, Arvore *Fcen){
    Arvore *no = (Arvore *)malloc(sizeof(Arvore));
    if(no) {
        no->info1 = info;
        no->esq = Fesq;
        no->cen = Fcen;
        no->dir = NULL;
        no->nInfos = 1;
    }
    return (no);
}

void adicionaInfo(Arvore **no, DadoUnion info, Arvore *filho){
    int cmp;
    if((*no)->tipo == ARTISTA)
        cmp = strcmp(info.ARTISTA.nome, (*no)->info1.ARTISTA.nome);
    else
        cmp = strcmp(info.ALBUM.nome, (*no)->info1.ALBUM.nome);

    if(cmp > 0){
        (*no)->info2 = info;
        (*no)->dir = filho;
    }else{
        (*no)->info2 = (*no)->info1;
        (*no)->dir = (*no)->cen;
        (*no)->info1 = info;
        (*no)->cen = filho;
    }
    (*no)->nInfos = 2;
}

Arvore *quebrarNo(Arvore **no, DadoUnion info, Arvore *filho, DadoUnion *sobe){
    Arvore *maior;
    int cmp1, cmp2;
    if((*no)->tipo == ARTISTA){
        cmp1 = strcmp(info.ARTISTA.nome, (*no)->info1.ARTISTA.nome);
        cmp2 = strcmp(info.ARTISTA.nome, (*no)->info2.ARTISTA.nome);
    }else{
        cmp1 = strcmp(info.ALBUM.nome, (*no)->info1.ALBUM.nome);
        cmp2 = strcmp(info.ALBUM.nome, (*no)->info2.ALBUM.nome);
    }

    if(cmp2 > 0){
        *sobe = (*no)->info2;
        (*no)->nInfos = 1;
        maior = criaNo(info, (*no)->dir, filho);
        (*no)->dir = NULL;
    }else if(cmp1 > 0){
        *sobe = info;
        maior = criaNo((*no)->info2, filho, (*no)->dir);
        (*no)->dir = NULL;
        (*no)->nInfos = 1;
    }else{
        *sobe = (*no)->info1;
        (*no)->info1 = info;
        (*no)->nInfos = 1;
        maior = criaNo((*no)->info2, (*no)->cen, (*no)->dir);
        (*no)->cen = filho;
        (*no)->dir = NULL;
    }

    return (maior);
}

Arvore *inserirNo(Arvore **R, DadoUnion info, Arvore *Pai, DadoUnion *sobe){
    Arvore *maior = inicializar();

    if(!(*R)) 
        *R = criaNo(info, NULL, NULL);
    else{
        if(ehFolha(*R)){
            if((*R)->nInfos == 1) adicionaInfo(R, info, NULL);
            else{
                maior = quebrarNo(R, info, NULL, sobe);
                if(!Pai){
                    *R = criaNo(*sobe, *R, maior);
                    maior = NULL;
                }
            }
        }else{
            int cmp1 = 0, cmp2 = 0;
            if((*R)->tipo == ARTISTA){
                cmp1 = strcmp(info.ARTISTA.nome, (*R)->info1.ARTISTA.nome);
                if((*R)->nInfos == 2)
                    cmp2 = strcmp(info.ARTISTA.nome, (*R)->info2.ARTISTA.nome);
            }else{
                cmp1 = strcmp(info.ALBUM.nome, (*R)->info1.ALBUM.nome);
                if((*R)->nInfos == 2)
                    cmp2 = strcmp(info.ALBUM.nome, (*R)->info2.ALBUM.nome);
            }

            if(cmp1 < 0)
                maior = inserirNo(&((*R)->esq), info, *R, sobe);
            else if((*R)->nInfos == 1 || cmp2 < 0)
                maior = inserirNo(&((*R)->cen), info, *R, sobe);
            else 
                maior = inserirNo(&((*R)->dir), info, *R, sobe);
        }

        if(maior){
            if((*R)->nInfos == 1){
                adicionaInfo(R, *sobe, maior);
                maior = NULL;
            }else{
                maior = quebrarNo(R, *sobe, maior, sobe);
                if(!Pai){
                    *R = criaNo(*sobe, *R, maior);
                    maior = NULL;
                }
            }
        }
    }

    return (maior);
}

void preencherMusica(Musica *musica) {
    if(musica != NULL){
        printf("\n=== Cadastro de Música ===\n");
        printf("Título da música: ");
        scanf(" %[^\n]", musica->titulo);
        deixarMaiusculo(musica->titulo);
        printf("Duração (em minutos): ");
        scanf("%d", &musica->minutos);
    }
}

int inserirMusica(Musica **lista, Musica *novaMusica){
    int inseriu = 1;
    if(*lista == NULL){
        *lista = novaMusica;
    } else {
        Musica *atual = *lista;
        Musica *ant = inicializarM();
        while(atual && strcmp(novaMusica->titulo, atual->titulo) >= 0){ 
            if(strcmp(atual->titulo, novaMusica->titulo) == 0) inseriu = 0;
            ant = atual;
            atual = atual->prox;
        }
        if(inseriu){
            if(!atual) ant->prox = novaMusica;
            else if(strcmp(atual->titulo, (*lista)->titulo) == 0){
                novaMusica->prox = *lista;
                *lista = novaMusica;
            }else{
                ant->prox = novaMusica;
                novaMusica->prox = atual;
            }
        }
    }
    return inseriu;
}

int buscarNaArvore23(Arvore *raiz, char *nome, Arvore **busca){
    int nInfoBusca = 0;
    if(raiz){
        int compInfo1 = 2, compInfo2 = 2;

        if(raiz->tipo == ARTISTA){
            compInfo1 = strcmp(nome, raiz->info1.ARTISTA.nome);
            if(raiz->nInfos == 2)
                compInfo2 = strcmp(nome, raiz->info2.ARTISTA.nome);
        }else{
            compInfo1 = strcmp(nome, raiz->info1.ALBUM.nome);
            if(raiz->nInfos == 2)
                compInfo2 = strcmp(nome, raiz->info2.ALBUM.nome);
        }

        if(compInfo1 == 0){
            *busca = raiz;
            nInfoBusca = 1;
        }else if(raiz->nInfos == 2 && compInfo2 == 0){
            *busca = raiz;
            nInfoBusca = 2;
        }else{
            if(compInfo1 < 0)
                nInfoBusca = buscarNaArvore23(raiz->esq, nome, busca);
            else if(raiz->nInfos == 1 || compInfo2 < 0)
                nInfoBusca = buscarNaArvore23(raiz->cen, nome, busca);
            else 
                nInfoBusca = buscarNaArvore23(raiz->dir, nome, busca);
        }
    }

    return nInfoBusca;
}

Musica *buscarMusica(Musica *lista, char *titulo){
    Musica *aux = lista, *busca = NULL;
    while(aux){
        if(strcmp(titulo, aux->titulo) == 0) busca = aux;
        aux = aux->prox;
    }
    return busca;
}

void mostrarCaminhoBusca(Arvore *raiz, char *nome, int *comparacoes){
    if(raiz){
        int compInfo1 = 2, compInfo2 = 2;

        if(raiz->tipo == ARTISTA){
            compInfo1 = strcmp(nome, raiz->info1.ARTISTA.nome);
            printf(" -> [%s]", raiz->info1.ARTISTA.nome);
            (*comparacoes)++;
            if(raiz->nInfos == 2){
                compInfo2 = strcmp(nome, raiz->info2.ARTISTA.nome);
                printf(" -> [%s]", raiz->info2.ARTISTA.nome);
                (*comparacoes)++;
            }
        }else{
            compInfo1 = strcmp(nome, raiz->info1.ALBUM.nome);
            printf(" -> [%s]", raiz->info1.ALBUM.nome);
            (*comparacoes)++;
            if(raiz->nInfos == 2){
                compInfo2 = strcmp(nome, raiz->info2.ALBUM.nome);
                printf(" -> [%s]", raiz->info2.ALBUM.nome);
                (*comparacoes)++;
            }
        }

        if(compInfo1 == 0 || (raiz->nInfos == 2 && compInfo2 == 0)){
            printf("  [ENCONTRADO]\n");
        }else{
            if(compInfo1 < 0)
                mostrarCaminhoBusca(raiz->esq, nome, comparacoes);
            else if(raiz->nInfos == 1 || compInfo2 < 0)
                mostrarCaminhoBusca(raiz->cen, nome, comparacoes);
            else 
                mostrarCaminhoBusca(raiz->dir, nome, comparacoes);
        }
    } else {
        printf(" (NAO ESTA NA ARVORE) \n");
    }
}

Arvore *buscarAlbumDeArtista(DadoUnion *artista, char *tituloAlbum, int *nInfoBuscada) {
    Arvore *raizAlbuns = artista->ARTISTA.albuns;
    Arvore *busca = inicializar();
    if(raizAlbuns) {
        *nInfoBuscada = buscarNaArvore23(raizAlbuns, tituloAlbum, &busca);
    } 
    return busca;
}

Musica *buscarMusicaDeAlbum(DadoUnion *album, char *tituloAlbum, char *tituloMusica){
    Musica *musica = NULL;
    if(album) 
        musica = buscarMusica(album->ALBUM.musicas, tituloMusica);
    return musica;
}

void buscarMusicaEmAlbuns(Arvore *albuns, Musica **busca, char *tituloMusica) {
    if(albuns && !(*busca)){
        *busca = buscarMusicaDeAlbum(&(albuns->info1), albuns->info1.ALBUM.nome, tituloMusica);
        if(albuns->nInfos == 2 && !(*busca))
            *busca = buscarMusicaDeAlbum(&(albuns->info2), albuns->info2.ALBUM.nome, tituloMusica);
        buscarMusicaEmAlbuns(albuns->esq, busca, tituloMusica);
        buscarMusicaEmAlbuns(albuns->cen, busca, tituloMusica);
        if(albuns->nInfos == 2)
            buscarMusicaEmAlbuns(albuns->dir, busca, tituloMusica);
    }
}

Musica *buscarMusicaDeArtista(Arvore *artistas, char *nomeArtista, char *tituloMusica) {
    Musica *musica = inicializarM();
    Arvore *artista = inicializar();
    int nInfoBuscada = buscarNaArvore23(artistas, nomeArtista, &artista);
    if (artista){
        if(nInfoBuscada == 1)
            buscarMusicaEmAlbuns(artista->info1.ARTISTA.albuns, &musica, tituloMusica);
        else 
            buscarMusicaEmAlbuns(artista->info2.ARTISTA.albuns, &musica, tituloMusica);
    }
    return (musica);
}

void mostrarArtistasPorEstilo(Arvore *artistas, char *estilo) {
    if (artistas){
        mostrarArtistasPorEstilo(artistas->esq, estilo);
        if(strcmp(artistas->info1.ARTISTA.estiloMusical, estilo) == 0)
            printf("Nome: %s\n", artistas->info1.ARTISTA.nome);
        mostrarArtistasPorEstilo(artistas->cen, estilo);
        if(artistas->nInfos == 2){
            if(strcmp(artistas->info2.ARTISTA.estiloMusical, estilo) == 0)
                printf("Nome: %s\n", artistas->info2.ARTISTA.nome);
            mostrarArtistasPorEstilo(artistas->dir, estilo);
        }
    }
}

void mostraArtista(Arvore *raiz){
    printf("\n=== Artista ===\n");
    printf("Nome: %s\n", raiz->info1.ARTISTA.nome);
    printf("Estilo musical: %s\n", raiz->info1.ARTISTA.estiloMusical);
    printf("Tipo: ");
    switch(raiz->info1.ARTISTA.tipo) {
        case CANTOR: printf("Cantor\n"); break;
        case DUPLA: printf("Dupla\n"); break;
        case BANDA: printf("Banda\n"); break;
        case GRUPO: printf("Grupo\n"); break;
        default: printf("Desconhecido\n"); break;
    }
    printf("Número de álbuns: %d\n", raiz->info1.ARTISTA.numeroAlbuns);

    if(raiz->nInfos == 2){
        printf("\n=== Artista ===\n");
        printf("Nome: %s\n", raiz->info2.ARTISTA.nome);
        printf("Estilo musical: %s\n", raiz->info2.ARTISTA.estiloMusical);
        printf("Tipo: ");
        switch(raiz->info2.ARTISTA.tipo) {
            case CANTOR: printf("Cantor\n"); break;
            case DUPLA: printf("Dupla\n"); break;
            case BANDA: printf("Banda\n"); break;
            case GRUPO: printf("Grupo\n"); break;
            default: printf("Desconhecido\n"); break;
        }
        printf("Número de álbuns: %d\n", raiz->info2.ARTISTA.numeroAlbuns);
    }
}

void mostraAlbum(Arvore *raiz){
    printf("\n=== Álbum ===\n");
    printf("Título: %s\n", raiz->info1.ALBUM.nome);
    printf("Ano de lançamento: %d\n", raiz->info1.ALBUM.anoLancamento);
    printf("Número de músicas: %d\n", raiz->info1.ALBUM.numeroMusicas);
    if(raiz->nInfos == 2){
        printf("\n=== Álbum ===\n");
        printf("Título: %s\n", raiz->info2.ALBUM.nome);
        printf("Ano de lançamento: %d\n", raiz->info2.ALBUM.anoLancamento);
        printf("Número de músicas: %d\n", raiz->info2.ALBUM.numeroMusicas);
    }
}

void imprimirArvore23(Arvore *raiz){
    if(raiz){
        imprimirArvore23(raiz->esq);
        imprimirArvore23(raiz->cen);

        if(raiz->tipo == ARTISTA)
            mostraArtista(raiz);
        else if(raiz->tipo == ALBUM)
            mostraAlbum(raiz);

        if(raiz->nInfos == 2)
            imprimirArvore23(raiz->dir);
    }
}

void imprimirMusicas(Musica *lista){
    if(lista){
        Musica *aux = lista;
        printf("\n=== Músicas ===\n");
        while(aux){
            printf("Título: %s, Duração: %d minutos\n", aux->titulo, aux->minutos);
            aux = aux->prox;
        }
    }else printf("Nenhuma música cadastrada.\n");
}

void mostrarAlbunsDeArtista(Arvore *artistas, char *nomeArtista){
    Arvore *artista = inicializar();
    int nInfoBuscada = buscarNaArvore23(artistas, nomeArtista, &artista);
    if(artista){
        if(nInfoBuscada == 1){
            printf("\n=== Álbuns de %s ===\n", artista->info1.ARTISTA.nome);
            imprimirArvore23(artista->info1.ARTISTA.albuns);
        }else{
            printf("\n=== Álbuns de %s ===\n", artista->info2.ARTISTA.nome);
            imprimirArvore23(artista->info2.ARTISTA.albuns);
        }
    }else printf("Artista não encontrado.\n");
}

void mostrarMusicasDeAlbum(DadoUnion *album){
    if(album){
        printf("\n=== Músicas do Álbum %s ===\n", album->ALBUM.nome);
        imprimirMusicas(album->ALBUM.musicas);
    }
}

void imprimirAlbunsRec(Arvore *n) {
    if(n){
        imprimirAlbunsRec(n->esq);
        imprimirAlbunsRec(n->cen);

        printf("\n--- Álbum: %s ---\n", n->info1.ALBUM.nome);
        imprimirMusicas(n->info1.ALBUM.musicas);

        if(n->nInfos == 2){
            printf("\n--- Álbum: %s ---\n", n->info2.ALBUM.nome);
            imprimirMusicas(n->info2.ALBUM.musicas);
            imprimirAlbunsRec(n->dir);
        }
    }
}

void mostrarMusicasDeArtista(Arvore *artistas, char *nomeArtista){
    Arvore *artista = inicializar();
    int nInfoBuscada = buscarNaArvore23(artistas, nomeArtista, &artista);
    if(artista){
        Arvore *aux = inicializar();
        if(nInfoBuscada == 1)
            aux = artista->info1.ARTISTA.albuns;
        else aux = artista->info2.ARTISTA.albuns;

        if(!aux) printf("Nenhum álbum cadastrado para este artista.\n");
        else imprimirAlbunsRec(aux);
    }else printf("Artista não encontrado.\n");
}

void liberarMusicas(Musica *lista){
    Musica *aux;
    while(lista){
        aux = lista;
        lista = lista->prox;
        free(aux);
    }
}

void liberarArvore(Arvore *raiz){
    if(raiz){
        liberarArvore(raiz->esq);
        liberarArvore(raiz->dir);
        liberarArvore(raiz->cen);
        free(raiz);
    }
}

// ================= REMOVER ======================

int ehVazio(Arvore *no){
    int vazio = 0;
    if(no->nInfos == 0) vazio = 1;
    return (vazio);
}

void deQuemSouFilho(Arvore *R, Arvore *pai, char *posicao) {
    if (R == pai->esq) strcpy(posicao, "ESQUERDA");
    else if (R == pai->cen) strcpy(posicao, "CENTRO");
    else if (R == pai->dir) strcpy(posicao, "DIREITA");
}

void onda_CentroParaDireita_CentroDuasInfos(Arvore **R, Arvore **pai){
    (*R)->info1 = (*pai)->info2;
    (*R)->nInfos = 1;
    (*pai)->info2 = (*pai)->cen->info2;
    (*pai)->cen->nInfos = 1;
    (*R)->esq = (*pai)->cen->dir;
    (*pai)->cen->dir = NULL;
}

void onda_CentroParaDireita_CentroUmaInfo(Arvore **R, Arvore **pai){
    (*R)->info2 = (*pai)->info2;
    (*R)->dir = (*R)->cen;

    (*R)->info1 = (*pai)->cen->info1;
    (*R)->nInfos = 2;
    (*pai)->nInfos = 1;

    (*R)->cen = (*pai)->cen->cen;
    (*R)->esq = (*pai)->cen->esq;

    free((*pai)->cen);

    (*pai)->cen = *R;
    (*pai)->dir = NULL;
}

void onda_EsquerdaParaCentro_EsquerdaDuasInfos(Arvore **R, Arvore **pai){
    (*R)->info1 = (*pai)->info1;
    (*R)->nInfos = 1;

    (*pai)->info1 = (*pai)->esq->info2;
    (*pai)->esq->nInfos = 1;
    
    (*R)->esq = (*pai)->esq->dir;
    (*pai)->esq->dir = NULL;
}

void onda_EsquerdaParaCentro_EsquerdaEmaInfo_PaiDuasInfos(Arvore **R, Arvore **pai){
    (*R)->info2 = (*pai)->info1;
    (*R)->info1 = (*pai)->esq->info1;
    (*R)->nInfos = 2;

    (*R)->dir = (*R)->cen;
    (*R)->cen = (*pai)->esq->cen;
    (*R)->esq = (*pai)->esq->esq;

    (*pai)->info1 = (*pai)->info2;
    (*pai)->nInfos = 1;

    free((*pai)->esq);
    (*pai)->esq = *R;
    (*pai)->cen = (*pai)->dir;
    (*pai)->dir = NULL;
}

void onda_EsquerdaParaCentro_EsquerdaEmaInfo_PaiUmaInfo(Arvore **R, Arvore **pai){
    (*R)->info2 = (*pai)->info1;
    (*R)->info1 = (*pai)->esq->info1;
    (*R)->nInfos = 2;

    (*pai)->nInfos = 0;

    (*R)->dir = (*R)->cen;
    (*R)->cen = (*pai)->esq->cen;
    (*R)->esq = (*pai)->esq->esq;

    free((*pai)->esq);
    (*pai)->esq = NULL;
}

void onda_CentroParaEsquerda_CentroDuasInfos(Arvore **R, Arvore **pai){
    (*R)->info1 = (*pai)->info1;
    (*R)->nInfos = 1;

    (*pai)->info1 = (*pai)->cen->info1;
    (*pai)->cen->info1 = (*pai)->cen->info2;
    (*pai)->cen->nInfos = 1;

    (*R)->esq = (*R)->cen;
    (*R)->cen = (*pai)->cen->esq;

    (*pai)->cen->esq = (*pai)->cen->cen;
    (*pai)->cen->cen = (*pai)->cen->dir;
    (*pai)->cen->dir = NULL;
}

void onda_CentroParaEsquerda_CentroUmaInfo_PaiDuasInfos(Arvore **R, Arvore **pai){
    (*R)->info1 = (*pai)->info1;
    (*R)->info2 = (*pai)->cen->info1;
    (*R)->nInfos = 2;

    (*R)->esq = (*R)->cen;
    (*R)->cen = (*pai)->cen->esq;
    (*R)->dir = (*pai)->cen->cen;

    (*pai)->info1 = (*pai)->info2;
    (*pai)->nInfos = 1;

    free((*pai)->cen);
    (*pai)->cen = (*pai)->dir;
    (*pai)->dir = NULL;
}

void onda_CentroParaEsquerda_CentroUmaInfo_PaiUmaInfo(Arvore **R, Arvore **pai){
    (*R)->info1 = (*pai)->info1;
    (*R)->info2 = (*pai)->cen->info1;
    (*R)->nInfos = 2;

    (*R)->esq = (*R)->cen;
    (*R)->cen = (*pai)->cen->esq;
    (*R)->dir = (*pai)->cen->cen;

    (*pai)->nInfos = 0;

    free((*pai)->cen);
    (*pai)->cen = (*R);
    (*pai)->esq = NULL;
}

void balanceia(Arvore **R, Arvore **pai){
    if(*R){
        if(ehVazio(*R)){
            if(*pai){
                char posicao[20];
                deQuemSouFilho(*R, *pai, posicao);

                if(strcmp(posicao, "DIREITA") == 0){
                    if((*pai)->cen->nInfos == 2)
                        onda_CentroParaDireita_CentroDuasInfos(R, pai);
                    else if((*pai)->cen->nInfos == 1)
                        onda_CentroParaDireita_CentroUmaInfo(R, pai);
                }
                else if(strcmp(posicao, "CENTRO") == 0){
                    if((*pai)->esq->nInfos == 2)
                        onda_EsquerdaParaCentro_EsquerdaDuasInfos(R, pai);
                    else if((*pai)->esq->nInfos == 1){
                        if((*pai)->nInfos == 2)
                            onda_EsquerdaParaCentro_EsquerdaEmaInfo_PaiDuasInfos(R, pai);
                        else if((*pai)->nInfos == 1)
                            onda_EsquerdaParaCentro_EsquerdaEmaInfo_PaiUmaInfo(R, pai);
                    }
                }
                else if(strcmp(posicao, "ESQUERDA") == 0){
                    if((*pai)->cen->nInfos == 2)
                        onda_CentroParaEsquerda_CentroDuasInfos(R, pai);
                    else if((*pai)->cen->nInfos == 1){
                        if((*pai)->nInfos == 2)
                            onda_CentroParaEsquerda_CentroUmaInfo_PaiDuasInfos(R, pai);
                        else if((*pai)->nInfos == 1)
                            onda_CentroParaEsquerda_CentroUmaInfo_PaiUmaInfo(R, pai);
                    }
                }
            }
            else {
                Arvore *aux = *R;
                *R = (*R)->cen;
                free(aux);
            }
        }
    }
}

void maiorFilhoEsq(Arvore **R, Arvore **maiorFilho, Arvore **paiM, int posicaoInfoQueVaiSerRemovida){
    if(*maiorFilho){
        if(ehFolha(*maiorFilho)){
            DadoUnion infoQueVaiSerRemovida;
            if(posicaoInfoQueVaiSerRemovida == 1){
                infoQueVaiSerRemovida = (*R)->info1;
                if((*maiorFilho)->nInfos == 2){
                    (*R)->info1 = (*maiorFilho)->info2;
                    (*maiorFilho)->info2 = infoQueVaiSerRemovida;
                }else{ 
                    (*R)->info1 = (*maiorFilho)->info1;
                    (*maiorFilho)->info1 = infoQueVaiSerRemovida;
                }
            }else if(posicaoInfoQueVaiSerRemovida == 2){ 
                infoQueVaiSerRemovida = (*R)->info2;
                if((*maiorFilho)->nInfos == 2){
                    (*R)->info2 = (*maiorFilho)->info2;
                    (*maiorFilho)->info2 = infoQueVaiSerRemovida;
                }else{ 
                    (*R)->info2 = (*maiorFilho)->info1;
                    (*maiorFilho)->info1 = infoQueVaiSerRemovida;
                }
            }
            removerArvore23(maiorFilho, paiM, &infoQueVaiSerRemovida);
        }else{
            if((*maiorFilho)->nInfos == 2){
                maiorFilhoEsq(R, &(*maiorFilho)->dir, paiM, posicaoInfoQueVaiSerRemovida);
            }else if((*maiorFilho)->nInfos == 1){
                maiorFilhoEsq(R, &(*maiorFilho)->cen, paiM, posicaoInfoQueVaiSerRemovida);
            }
        }
    }
    balanceia(maiorFilho, paiM);
}

void removerArvore23(Arvore **R, Arvore **pai, DadoUnion *info){
    int compInfo1 = 2, compInfo2 = 2;
    if(*R){
        int match1 = 0, match2 = 0;
        if ((*R)->tipo == ARTISTA) {
            match1 = (strcmp(info->ARTISTA.nome, (*R)->info1.ARTISTA.nome) == 0);
            if((*R)->nInfos == 2)
                match2 = (strcmp(info->ARTISTA.nome, (*R)->info2.ARTISTA.nome) == 0);
        } else {
            match1 = (strcmp(info->ALBUM.nome, (*R)->info1.ALBUM.nome) == 0);
            if((*R)->nInfos == 2)
                match2 = (strcmp(info->ALBUM.nome, (*R)->info2.ALBUM.nome) == 0);
        }

        if(match1){
            if(ehFolha(*R)){
                if((*R)->nInfos == 2){
                    (*R)->info1 = (*R)->info2;
                    (*R)->nInfos = 1;
                }else (*R)->nInfos = 0;
            }else{
                Arvore **maiorFilho = &((*R)->esq);
                Arvore **paiM = R;
                maiorFilhoEsq(R, maiorFilho, paiM, 1);
            }
        }else if(match2){
            if(ehFolha(*R)) (*R)->nInfos = 1;
            else{
                Arvore **maiorFilho = &((*R)->cen);
                Arvore **paiM = R;
                maiorFilhoEsq(R, maiorFilho, paiM, 2);
            }
        }else{
            if ((*R)->tipo == ARTISTA) {
                compInfo1 = strcmp(info->ARTISTA.nome, (*R)->info1.ARTISTA.nome);
                if((*R)->nInfos == 2)
                    compInfo2 = strcmp(info->ARTISTA.nome, (*R)->info2.ARTISTA.nome);
            } else {
                compInfo1 = strcmp(info->ALBUM.nome, (*R)->info1.ALBUM.nome);
                if((*R)->nInfos == 2)
                    compInfo2 = strcmp(info->ALBUM.nome, (*R)->info2.ALBUM.nome);
            }

            if(compInfo1 < 0)
                removerArvore23(&((*R)->esq), R, info);
            else if((*R)->nInfos == 1 || compInfo2 < 0)
                removerArvore23(&((*R)->cen), R, info);
            else 
                removerArvore23(&((*R)->dir), R, info);
        }    
    }

    balanceia(R, pai);
}