#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prototipos.h"

void menu(){
    printf("\n========== MENU PRINCIPAL (ÁRVORE 2-3) ==========\n");
    printf("1  - Cadastrar artista\n");
    printf("2  - Cadastrar album\n");
    printf("3  - Cadastrar musica\n");
    printf("---------------------------------------\n");
    printf("4  - Listar todos os artistas\n");
    printf("5  - Mostrar albuns de um artista\n");
    printf("6  - Mostrar musicas de um album\n");
    printf("7  - Mostrar musicas de um artista\n");
    printf("---------------------------------------\n");
    printf("8  - Buscar artista (mostrar caminho)\n");
    printf("9  - Buscar musica de artista\n");
    printf("10 - Buscar musica em um album\n");
    printf("---------------------------------------\n");
    printf("11 - Mostrar artistas por estilo musical\n");
    printf("12 - Remover musica de um album\n");
    printf("13 - Remover album de um artista\n");
    printf("14 - Remover artista\n");
    printf("-----------------------------------------\n");
    printf("0  - Sair\n");
    printf("====================================\n");
    printf("Escolha uma opcao: ");
}

int main(){
    // Disable buffering on stdout to ensure reliable communication with GUIs/Pipes
    setvbuf(stdout, NULL, _IONBF, 0);

    Arvore *biblioteca = inicializar();
    DadoUnion sobe;
    int opcao;

    do {
        menu();
        if(scanf("%d", &opcao) != 1) {
            printf("\nOpcao invalida!\n");
            // Clear input buffer
            while(getchar() != '\n');
            opcao = -1;
            continue;
        }

        switch(opcao){
            case 1: { // CADASTRAR ARTISTA
                DadoUnion info;
                preencheInfo(ARTISTA, &info);
                Arvore *busca = inicializar();
                if(buscarNaArvore23(biblioteca, info.ARTISTA.nome, &busca) == 0){
                    inserirNo(&biblioteca, info, NULL, &sobe);
                    printf("\n Artista cadastrado com sucesso!\n");
                }else printf("\n Artista ja cadastrado!\n");
                break;
            }
            case 2: { // CADASTRAR ALBUM
                char nomeArtista[100];
                printf("Digite o nome do artista: ");
                scanf(" %[^\n]", nomeArtista);
                deixarMaiusculo(nomeArtista);

                Arvore *artista = inicializar();
                int nInfo = buscarNaArvore23(biblioteca, nomeArtista, &artista);
                if(artista){
                    DadoUnion info;
                    preencheInfo(ALBUM, &info);
                    
                    if (nInfo == 1) {
                        Arvore *busca = inicializar();
                        if(buscarNaArvore23(artista->info1.ARTISTA.albuns, info.ALBUM.nome, &busca) == 0){
                            DadoUnion sobeAlb;
                            inserirNo(&(artista->info1.ARTISTA.albuns), info, NULL, &sobeAlb);
                            artista->info1.ARTISTA.numeroAlbuns++;
                            printf("\n Album cadastrado com sucesso!\n");
                        } else printf("\n Album ja cadastrado para este artista.\n");
                    } else {
                        Arvore *busca = inicializar();
                        if(buscarNaArvore23(artista->info2.ARTISTA.albuns, info.ALBUM.nome, &busca) == 0){
                            DadoUnion sobeAlb;
                            inserirNo(&(artista->info2.ARTISTA.albuns), info, NULL, &sobeAlb);
                            artista->info2.ARTISTA.numeroAlbuns++;
                            printf("\n Album cadastrado com sucesso!\n");
                        } else printf("\n Album ja cadastrado para este artista.\n");
                    }
                } else {
                    printf("\n Artista nao encontrado!\n");
                }
                break;
            }
            case 3: { // CADASTRAR MÚSICA
                char nomeArtista[100], tituloAlbum[100];
                printf("Digite o nome do artista: ");
                scanf(" %[^\n]", nomeArtista);
                deixarMaiusculo(nomeArtista);

                Arvore *artista = inicializar();
                int nInfoArt = buscarNaArvore23(biblioteca, nomeArtista, &artista);
                if (artista) {
                    printf("Digite o titulo do album: ");
                    scanf(" %[^\n]", tituloAlbum);
                    deixarMaiusculo(tituloAlbum);

                    int nInfoAlb = 0;
                    Arvore *album = inicializar();
                    if(nInfoArt == 1) {
                        album = buscarAlbumDeArtista(&(artista->info1), tituloAlbum, &nInfoAlb);
                    } else {
                        album = buscarAlbumDeArtista(&(artista->info2), tituloAlbum, &nInfoAlb);
                    }

                    if (album) {
                        Musica *novaMusica = alocarM();
                        if (novaMusica != NULL) {
                            preencherMusica(novaMusica);
                            DadoUnion *dadoAlb = (nInfoAlb == 1) ? &(album->info1) : &(album->info2);
                            if (inserirMusica(&(dadoAlb->ALBUM.musicas), novaMusica)) {
                                dadoAlb->ALBUM.numeroMusicas++;
                                printf("\n Musica cadastrada com sucesso!\n");
                            } else {
                                printf("\n Erro ao cadastrar musica (ja existe).\n");
                                free(novaMusica);
                            }
                        }
                    } else {
                        printf("\n Album nao encontrado.\n");
                    }
                } else {
                    printf("\n Artista nao encontrado.\n");
                }
                break;
            }
            case 4: { // LISTAR ARTISTAS
                if(!biblioteca) printf("Nenhum artista cadastrado.\n");
                else imprimirArvore23(biblioteca);
                break;
            }
            case 5: { // MOSTRAR ÁLBUNS DE ARTISTA
                char nome[100];
                printf("Digite o nome do artista: ");
                scanf(" %[^\n]", nome);
                deixarMaiusculo(nome);
                mostrarAlbunsDeArtista(biblioteca, nome);
                break;
            }
            case 6: { // MOSTRAR MÚSICAS DE UM ÁLBUM
                char nome[100], nalbum[100];
                printf("Digite o nome do artista: ");
                scanf(" %[^\n]", nome);
                deixarMaiusculo(nome);
                Arvore *artista = inicializar();
                int nInfoArt = buscarNaArvore23(biblioteca, nome, &artista);
                if (artista){
                    printf("Digite o titulo do album: ");
                    scanf(" %[^\n]", nalbum);
                    deixarMaiusculo(nalbum);
                    
                    int nInfoAlb = 0;
                    Arvore *album = inicializar();
                    if(nInfoArt == 1)
                        album = buscarAlbumDeArtista(&(artista->info1), nalbum, &nInfoAlb);
                    else
                        album = buscarAlbumDeArtista(&(artista->info2), nalbum, &nInfoAlb);

                    if(album) {
                        DadoUnion *dadoAlb = (nInfoAlb == 1) ? &(album->info1) : &(album->info2);
                        mostrarMusicasDeAlbum(dadoAlb);
                    } else printf("Album nao encontrado.\n");
                } else printf("Artista nao encontrado.\n");
                break;
            }
            case 7: { // MOSTRAR MÚSICAS DE UM ARTISTA
                char nome[100];
                printf("Digite o nome do artista: ");
                scanf(" %[^\n]", nome);
                deixarMaiusculo(nome);
                mostrarMusicasDeArtista(biblioteca, nome);
                break;
            }
            case 8: { // BUSCAR ARTISTA (CAMINHO)
                char nome[100];
                printf("Digite o nome do artista: ");
                scanf(" %[^\n]", nome);
                deixarMaiusculo(nome);
                int comparacoes = 0;
                printf("\n Caminho da busca:\n");
                mostrarCaminhoBusca(biblioteca, nome, &comparacoes);
                printf(" Comparacoes realizadas: %d\n", comparacoes);
                break;
            }
            case 9: { // BUSCAR MÚSICA DE ARTISTA
                char nomeArtista[100], tituloMusica[100];
                printf("Digite o nome do artista: ");
                scanf(" %[^\n]", nomeArtista);
                deixarMaiusculo(nomeArtista);

                Arvore *artista = inicializar();
                if(buscarNaArvore23(biblioteca, nomeArtista, &artista)){
                    printf("Digite o titulo da musica: ");
                    scanf(" %[^\n]", tituloMusica); 
                    deixarMaiusculo(tituloMusica);
                    
                    Musica *musica = buscarMusicaDeArtista(biblioteca, nomeArtista, tituloMusica);

                    if(musica) printf("\n Musica encontrada: %s (%d min)\n", musica->titulo, musica->minutos);
                    else printf("\n Musica nao encontrada.\n");
                } else printf("\n Artista nao encontrado.\n");                
                break;
            }
            case 10: { // BUSCAR MÚSICA EM UM ÁLBUM
                char nomeArtista[100], tituloAlbum[100], tituloMusica[100];
                printf("Digite o nome do artista: ");
                scanf(" %[^\n]", nomeArtista);
                deixarMaiusculo(nomeArtista);

                Arvore *artista = inicializar();
                int nInfoArt = buscarNaArvore23(biblioteca, nomeArtista, &artista);
                if(artista){
                    printf("Digite o titulo do album: ");
                    scanf(" %[^\n]", tituloAlbum);
                    deixarMaiusculo(tituloAlbum);

                    int nInfoAlb = 0;
                    Arvore *album = inicializar();
                    if(nInfoArt == 1)
                        album = buscarAlbumDeArtista(&(artista->info1), tituloAlbum, &nInfoAlb);
                    else
                        album = buscarAlbumDeArtista(&(artista->info2), tituloAlbum, &nInfoAlb);

                    if(album){
                        printf("Digite o titulo da musica: ");
                        scanf(" %[^\n]", tituloMusica);
                        deixarMaiusculo(tituloMusica);

                        DadoUnion *dadoAlb = (nInfoAlb == 1) ? &(album->info1) : &(album->info2);
                        Musica *musica = buscarMusicaDeAlbum(dadoAlb, tituloAlbum, tituloMusica);
                        if(musica) printf("\n Musica encontrada: %s (%d min)\n", musica->titulo, musica->minutos);
                        else printf("\n Musica nao encontrada.\n");
                    } else printf("\n Album nao encontrado.\n");
                } else printf("\n Artista nao encontrado.\n");
                break;
            }
            case 11: { // MOSTRAR ARTISTAS POR ESTILO
                char estilo[50];
                printf("Digite o estilo musical: ");
                scanf(" %[^\n]", estilo);
                deixarMaiusculo(estilo);
                mostrarArtistasPorEstilo(biblioteca, estilo);
                break;
            }
            case 12: { // REMOVER MÚSICA
                char nomeArtista[100], tituloAlbum[100], tituloMusica[100];
                printf("Digite o nome do artista: ");
                scanf(" %[^\n]", nomeArtista);
                deixarMaiusculo(nomeArtista);

                Arvore *artista = inicializar();
                int nInfoArt = buscarNaArvore23(biblioteca, nomeArtista, &artista);
                if(artista){
                    printf("Digite o titulo do album: ");
                    scanf(" %[^\n]", tituloAlbum);
                    deixarMaiusculo(tituloAlbum);

                    int nInfoAlb = 0;
                    Arvore *album = inicializar();
                    if(nInfoArt == 1)
                        album = buscarAlbumDeArtista(&(artista->info1), tituloAlbum, &nInfoAlb);
                    else
                        album = buscarAlbumDeArtista(&(artista->info2), tituloAlbum, &nInfoAlb);

                    if(album){
                        printf("Digite o titulo da musica a remover: ");
                        scanf(" %[^\n]", tituloMusica);
                        deixarMaiusculo(tituloMusica);

                        DadoUnion *dadoAlb = (nInfoAlb == 1) ? &(album->info1) : &(album->info2);
                        // Using a simple linked list remove since music is a linked list
                        Musica *ant = NULL;
                        Musica *curr = dadoAlb->ALBUM.musicas;
                        int removeu = 0;
                        while(curr) {
                            if(strcmp(curr->titulo, tituloMusica) == 0) {
                                if(ant == NULL) dadoAlb->ALBUM.musicas = curr->prox;
                                else ant->prox = curr->prox;
                                free(curr);
                                removeu = 1;
                                break;
                            }
                            ant = curr;
                            curr = curr->prox;
                        }

                        if(removeu){
                            dadoAlb->ALBUM.numeroMusicas--;
                            printf("\n Musica removida com sucesso!\n");
                        }else printf("\n Musica nao encontrada.\n");
                    } else printf("\n Album nao encontrado.\n");   
                } else printf("\n Artista nao encontrado.\n");
                break;
            }
            case 13: { // REMOVER ALBUM
                char nomeAlbum[100], nomeArtista[100];
                printf("Digite o nome do artista: ");
                scanf(" %[^\n]", nomeArtista);
                deixarMaiusculo(nomeArtista);

                Arvore *artista = inicializar();
                int nInfoArt = buscarNaArvore23(biblioteca, nomeArtista, &artista);
                if(artista){
                    printf("Digite o nome do album a ser removido: ");
                    scanf(" %[^\n]", nomeAlbum);
                    deixarMaiusculo(nomeAlbum);
                    
                    DadoUnion infoRem;
                    strcpy(infoRem.ALBUM.nome, nomeAlbum);

                    if(nInfoArt == 1) {
                        removerArvore23(&(artista->info1.ARTISTA.albuns), NULL, &infoRem);
                        artista->info1.ARTISTA.numeroAlbuns--;
                    } else {
                        removerArvore23(&(artista->info2.ARTISTA.albuns), NULL, &infoRem);
                        artista->info2.ARTISTA.numeroAlbuns--;
                    }
                    printf("Processo de remocao de album executado.\n");
                } else printf("Artista nao encontrado.\n");
                break;
            }
            case 14: { // REMOVER ARTISTA
                char nomeArtista[100];
                printf("Digite o nome do Artista que deseja apagar: ");
                scanf(" %[^\n]", nomeArtista);
                deixarMaiusculo(nomeArtista);
                
                DadoUnion infoRem;
                strcpy(infoRem.ARTISTA.nome, nomeArtista);

                removerArvore23(&biblioteca, NULL, &infoRem);
                printf("Processo de remocao de artista executado.\n");
                break;
            }
            case 0:
                printf("\n Encerrando o programa...\n");
                break;
            default:
                if(opcao != -1) printf("\n Opcao invalida!\n");
        }

    } while(opcao != 0);

    liberarArvore(biblioteca);
    return 0;
}