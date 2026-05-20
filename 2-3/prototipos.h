#ifndef PROTOTIPOS_H
#define PROTOTIPOS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct artista;
struct album;
struct musica;
struct arvore;

typedef enum {ARTISTA, ALBUM} TipoDado;
typedef enum {CANTOR = 1, DUPLA, BANDA, GRUPO} TipoArtista;
typedef enum {VERMELHO, PRETO} Cor;

typedef struct artista{
    TipoArtista tipo;
    char nome[50], estiloMusical[40];
    int numeroAlbuns;
    struct arvore *albuns;
}Artista;

typedef struct album{
    char nome[50];
    int anoLancamento, numeroMusicas;
    struct musica *musicas;
}Album;

typedef struct musica{
    char titulo[50];
    int minutos;
    struct musica *prox;
}Musica;

typedef union{
    struct artista ARTISTA;
    struct album ALBUM;
}DadoUnion;

typedef struct arvore{
    TipoDado tipo;
    DadoUnion info1;
    DadoUnion info2;
    int nInfos;
    struct arvore *esq, *cen, *dir;
}Arvore;

Arvore *inicializar();
Musica *inicializarM();
void deixarMaiusculo(char *str);

Arvore *alocar(TipoDado tipo);
Musica *alocarM();
int ehFolha(Arvore *R);
void preencheInfo(TipoDado tipo, DadoUnion *info);

Arvore *criaNo(DadoUnion info, Arvore *Fesq, Arvore *Fcen);
void adicionaInfo(Arvore **no, DadoUnion info, Arvore *filho);
Arvore *inserirNo(Arvore **R, DadoUnion info, Arvore *Pai, DadoUnion *sobe);
Arvore *quebrarNo(Arvore **no, DadoUnion info, Arvore *filho, DadoUnion *sobe);
void preencherMusica(Musica *musica);
int inserirMusica(Musica **lista, Musica *novaMusica);

int buscarNaArvore23(Arvore *raiz, char *nome, Arvore **busca);
Musica *buscarMusica(Musica *lista, char *titulo);
void mostrarCaminhoBusca(Arvore *raiz, char *nome, int *comparacoes);
Arvore *buscarAlbumDeArtista(DadoUnion *artista, char *tituloAlbum, int *nInfoBuscada);
Musica *buscarMusicaDeAlbum(DadoUnion *album, char *tituloAlbum, char *tituloMusica);
void buscarMusicaEmAlbuns(Arvore *albuns, Musica **busca, char *tituloMusica);
Musica *buscarMusicaDeArtista(Arvore *artistas, char *nomeArtista, char *tituloMusica);

void mostrarArtistasPorEstilo(Arvore *artistas, char *estilo);
void mostraArtista(Arvore *raiz);
void mostraAlbum(Arvore *raiz);
void imprimirArvore23(Arvore *raiz);
void imprimirMusicas(Musica *lista);
void mostrarAlbunsDeArtista(Arvore *artistas, char *nomeArtista);
void mostrarMusicasDeAlbum(DadoUnion *album);
void imprimirAlbunsRec(Arvore *n);
void mostrarMusicasDeArtista(Arvore *artistas, char *nomeArtista);

void liberarMusicas(Musica *lista);
void liberarArvore(Arvore *raiz);

// REMOVER 
int ehVazio(Arvore *no);
void deQuemSouFilho(Arvore *R, Arvore *pai, char *posicao);

void onda_CentroParaDireita_CentroDuasInfos(Arvore **R, Arvore **pai);
void onda_CentroParaDireita_CentroUmaInfo(Arvore **R, Arvore **pai);
void onda_EsquerdaParaCentro_EsquerdaDuasInfos(Arvore **R, Arvore **pai);
void onda_EsquerdaParaCentro_EsquerdaEmaInfo_PaiDuasInfos(Arvore **R, Arvore **pai);
void onda_EsquerdaParaCentro_EsquerdaEmaInfo_PaiUmaInfo(Arvore **R, Arvore **pai);
void onda_CentroParaEsquerda_CentroDuasInfos(Arvore **R, Arvore **pai);
void onda_CentroParaEsquerda_CentroUmaInfo_PaiDuasInfos(Arvore **R, Arvore **pai);
void onda_CentroParaEsquerda_CentroUmaInfo_PaiUmaInfo(Arvore **R, Arvore **pai);

void balanceia(Arvore **R, Arvore **pai);
void maiorFilhoEsq(Arvore **R, Arvore **maiorFilho, Arvore **paiM, int posicaoInfoQueVaiSerRemovida);
void removerArvore23(Arvore **R, Arvore **pai, DadoUnion *info);

#endif