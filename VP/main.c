#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "prototipos.h"

void menu(){
    printf("\n========== MENU PRINCIPAL ==========\n");
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
    printf("15 - Remover musica de um artista\n");
    printf("-----------------------------------------\n");
    printf("0  - Sair\n");
    printf("====================================\n");
    printf("Escolha uma opcao: ");
}

int main(){
    setvbuf(stdout, NULL, _IONBF, 0);
    Arvore *biblioteca = inicializar();
    int opcao;

    do {
        menu();
        scanf("%d", &opcao);

        switch(opcao){
            case 1: {
                Arvore *novoArtista = alocarTree(ARTISTA);
                if(novoArtista){
                    preencherNo(novoArtista);
                    if(inserirArvore(&biblioteca, novoArtista)){
                        atualizaCorRaiz(&biblioteca);
                        printf("\n Artista cadastrado com sucesso!\n");
                    }else printf("\n Erro ao cadastrar artista.\n");
                }
                break;
            }
            case 2: {
                char nomeArtista[100];
                printf("Digite o nome do artista: ");
                scanf(" %[^\n]", nomeArtista);
                deixarMaiusculo(nomeArtista);

                Arvore *artista = buscarArvRubroNegra(biblioteca, nomeArtista);
                if(artista){
                    Arvore *novoAlbum = alocarTree(ALBUM);
                    if(novoAlbum != NULL){
                        preencherNo(novoAlbum);
                        if(inserirArvore(&artista->dado.ARTISTA.albuns, novoAlbum)){
                            artista->dado.ARTISTA.numeroAlbuns++;
                            atualizaCorRaiz(&artista->dado.ARTISTA.albuns);
                            printf("\n Album cadastrado com sucesso!\n");
                        } else {
                            printf("\n Erro ao cadastrar album.\n");
                        }
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

                Arvore *artista = buscarArvRubroNegra(biblioteca, nomeArtista);
                if (artista) {
                    printf("Digite o titulo do album: ");
                    scanf(" %[^\n]", tituloAlbum);

                    deixarMaiusculo(tituloAlbum);

                    Arvore *album = inicializar();
                    buscarAlbumDeArtista(artista, tituloAlbum, &album);

                    if (album) {
                        Musica *novaMusica = alocarMusica();
                        if (novaMusica != NULL) {
                            preencherMusica(novaMusica);
                            if (inserirMusica(&album->dado.ALBUM.musicas, novaMusica)) {
                                album->dado.ALBUM.numeroMusicas++;
                                printf("\n Musica cadastrada com sucesso!\n");
                            } else {
                                printf("\n Erro ao cadastrar musica.\n");
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
                imprimirArvRubroNegra(biblioteca);
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
                Arvore *artista = buscarArvRubroNegra(biblioteca, nome);
                if (artista){
                    if (artista){
                        printf("Digite o titulo do album: ");
                        scanf(" %[^\n]", nalbum);
                        deixarMaiusculo(nalbum);
                        Arvore *album;
                        buscarAlbumDeArtista(artista, nalbum, &album);
                        if(album)
                            mostrarMusicasDeAlbum(album);
                        else printf("Album nao encontrado");
                    } else printf("Artista nao encontnrado");
                }
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

                Arvore *artista = buscarArvRubroNegra(biblioteca, nomeArtista);
                if(artista){
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

                Arvore *artista = buscarArvRubroNegra(biblioteca, nomeArtista);
                if(artista){
                    printf("Digite o titulo do album: ");
                    scanf(" %[^\n]", tituloAlbum);
                    deixarMaiusculo(tituloAlbum);

                    Arvore *album;
                    buscarAlbumDeArtista(artista, tituloAlbum, &album);
                    if(album){
                        printf("Digite o titulo da musica: ");
                        scanf(" %[^\n]", tituloMusica);
                        deixarMaiusculo(tituloMusica);

                        Musica *musica = buscarMusicaDeAlbum(album, tituloAlbum, tituloMusica);
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

                Arvore *artista = buscarArvRubroNegra(biblioteca, nomeArtista);
                if(artista){
                    printf("Digite o titulo do album: ");
                    scanf(" %[^\n]", tituloAlbum);
                    deixarMaiusculo(tituloAlbum);

                    Arvore *album;
                    buscarAlbumDeArtista(artista, tituloAlbum, &album);

                    if(album){
                        printf("Digite o titulo da musica a remover: ");
                        scanf(" %[^\n]", tituloMusica);
                        deixarMaiusculo(tituloMusica);

                        if(removerMusica(&album->dado.ALBUM.musicas, tituloMusica)){
                            album->dado.ALBUM.numeroMusicas--;
                            printf("\n Musica removida com sucesso!\n");
                        }else printf("\n Musica nao encontrada.\n");
                    } else printf("\n Album nao encontrado.\n");   
                } else printf("\n Artista nao encontrado.\n");

                break;
            }
            case 13: {
                char nomeAlbum[100], nomeArtista[100];
                printf("Digite o nome do artista: ");
                scanf(" %[^\n]", nomeArtista);
                deixarMaiusculo(nomeArtista);

                Arvore *artista = buscarArvRubroNegra(biblioteca, nomeArtista);
                if(artista){
                    printf("Digite o nome do album a ser removido: ");
                    scanf(" %[^\n]", nomeAlbum);

                    deixarMaiusculo(nomeAlbum);
                    if(removerArvRubroNegra(&artista->dado.ARTISTA.albuns, nomeAlbum))
                        printf("Album %s removido com sucesso!\n", nomeAlbum);
                    else printf("Album nao encontrado.\n");
                } else printf("Artista nao encontrado.\n");
                
                break;
            }
            case 14:{
                char nomeArtista[100];
                printf("Digite o nome do Artista que deseja apagar: ");
                scanf(" %[^\n]", nomeArtista);
    
                deixarMaiusculo(nomeArtista);
                if(removerArvRubroNegra(&biblioteca, nomeArtista))
                    printf("Artista %s removido com sucesso!\n", nomeArtista);
                else printf("Artista nao encontrado.\n");
            }
            case 0:
                printf("\n Encerrando o programa...\n");
                break;
            default:
                printf("\n Opcao invalida!\n");
        }

    } while(opcao != 0);

    liberarArvore(biblioteca);
    return 0;
}