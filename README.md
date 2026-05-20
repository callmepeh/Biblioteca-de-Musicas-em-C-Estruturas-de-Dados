# Music Library DB — Implementação de Árvores 2-3 e Rubro-Negra (C)

[![C Language](https://img.shields.io/badge/Language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Data Structures](https://img.shields.io/badge/Data%20Structures-Trees%20%26%20Lists-orange.svg)]()
[![Algorithms](https://img.shields.io/badge/Algorithms-Self--Balancing-green.svg)]()

Este repositório contém um sistema de banco de dados em memória para catálogo musical desenvolvido em **C**. O grande diferencial técnico deste projeto é o uso prático de duas das estruturas de dados auto-balanceadas mais complexas da computação: **Árvores 2-3** e **Árvores Rubro-Negras (Red-Black Trees)**. 

O projeto foi estruturado para modelar e gerenciar com máxima eficiência um domínio hierárquico complexo: **Artista ➔ Álbuns ➔ Músicas**, permitindo analisar de forma comparativa o desempenho e a complexidade de busca, inserção e remoção em ambas as estruturas.

---

## 🎯 Destaques do Projeto (Showcase para Portfólio)

- **Estruturas Auto-balanceadas na Prática**: Implementação completa de rotações (esquerda/direita), inversão de cores (color flip) na Árvore Rubro-Negra, além de divisão de nós (split), promoção de chaves e redistribuição/fusão complexa (propagação por "onda") na Árvore 2-3.
- **Modelagem Hierárquica Relacional**:
  - **Artistas** representam os nós da árvore principal (ordenados alfabeticamente).
  - Cada artista possui sua própria subárvore auto-balanceada contendo seus **Álbuns**.
  - Cada álbum aponta para uma lista encadeada ordenada de **Músicas**.
- **Polimorfismo em C**: Uso de `union` e `enum` para permitir nós genéricos de árvore que podem armazenar metadados de Artistas ou Álbuns dinamicamente.
- **Análise Comparativa de Busca**: Rastreabilidade do caminho exato percorrido na árvore durante operações de busca (`mostrarCaminhoBusca`), ideal para análise experimental do número de comparações.

---

## 📁 Estrutura do Repositório

O projeto é dividido em dois submódulos principais de implementação:

```text
├── 2-3/                 # Implementação baseada em Árvore 2-3
│   ├── funcoes.c        # Lógica de inserção, remoção ("onda") e busca
│   ├── prototipos.h     # Definição das estruturas e assinaturas de funções
│   └── main.c           # Ponto de entrada / CLI
│
├── VP/                  # Implementação baseada em Árvore Rubro-Negra (Vermelho-Preto)
│   ├── funcoes.c        # Rotações, balanceamento esquerdo/direito e CRUD
│   ├── prototipos.h     # Assinaturas e estruturas do submódulo
│   └── main.c           # Menu interativo CLI completo para cadastro e testes
│
└── README.md            # Documentação do projeto
```

---

## 🏗️ Arquitetura de Dados

A hierarquia de dados é modelada de maneira que a árvore principal (de Artistas) gerencie subárvores locais (de Álbuns), otimizando drasticamente as buscas por escopo.

```mermaid
graph TD
    subgraph Árvore Principal (Artistas)
        ArtA[Artista A] --- ArtB[Artista B]
    end
    subgraph Subárvore de Álbuns (Rubro-Negra / 2-3)
        ArtB --> Alb1[Álbum 1]
        ArtB --> Alb2[Álbum 2]
    end
    subgraph Lista Encadeada (Músicas)
        Alb2 --> M1[Música 1: Título | Minutos]
        M1 --> M2[Música 2: Título | Minutos]
    end

    style ArtB fill:#f9f,stroke:#333,stroke-width:2px
    style Alb2 fill:#bbf,stroke:#333,stroke-width:2px
    style M1 fill:#bfb,stroke:#333,stroke-width:1px
```

---

## 📊 Comparativo Teórico: Árvore 2-3 vs. Árvore Rubro-Negra

Ambas as árvores são isomorfas (uma árvore Rubro-Negra pode ser mapeada diretamente para uma árvore 2-3-4), mantendo garantia de tempo de execução logarítmico para as principais operações:

| Operação | Árvore 2-3 (Melhor / Pior Caso) | Árvore Rubro-Negra (Melhor / Pior Caso) | Explicação |
| :--- | :---: | :---: | :--- |
| **Busca** | $O(\log n)$ / $O(\log n)$ | $O(\log n)$ / $O(\log n)$ | Ambas garantem altura controlada, mas a Rubro-Negra pode ser até $2\log n$ em altura devido ao balanceamento mais relaxado. |
| **Inserção** | $O(\log n)$ / $O(\log n)$ | $O(\log n)$ / $O(\log n)$ | A 2-3 realiza divisões e subidas de nós; a Rubro-Negra resolve conflitos com rotações rápidas e trocas de cores. |
| **Remoção** | $O(\log n)$ / $O(\log n)$ | $O(\log n)$ / $O(\log n)$ | A remoção na 2-3 exige redistribuição complexa entre irmãos; na Rubro-Negra, são feitas correções locais de propriedades de cores. |

---

## 🚀 Como Compilar e Executar

Para rodar a aplicação, você precisará de um compilador C (como o `gcc`).

### Executando a Árvore Rubro-Negra (VP)

O submódulo `VP` possui uma interface interativa via console completa (CLI) que permite testar todas as funcionalidades de cadastro, listagem, busca e remoção.

1. Navegue até a pasta `VP`:
   ```bash
   cd VP
   ```
2. Compile os arquivos fontes:
   ```bash
   gcc main.c funcoes.c -o programa_vp
   ```
3. Execute o programa:
   ```bash
   ./programa_vp
   ```

---

## 🛠️ Detalhes de Implementação

### Estruturas de Dados Básicas (`prototipos.h`)

O projeto faz uso de `Unions` para garantir eficiência de memória no armazenamento dos nós da árvore:

```c
typedef enum {ARTISTA, ALBUM} TipoDado;

typedef struct artista {
    TipoArtista TipArt;
    char nome[50], estiloMusical[70];
    int numeroAlbuns;
    struct arvore *albuns; // Subárvore contendo os álbuns deste artista
} Artista;

typedef struct album {
    char titulo[50];
    int anoLancamento, numeroMusicas;
    struct musica *musicas; // Lista encadeada contendo as músicas deste álbum
} Album;

typedef union {
    struct artista ARTISTA;
    struct album ALBUM;
} DadoUnion;

typedef struct arvore {
    TipoDado tipo;
    DadoUnion dado;
    Cor cor;                // Usado na Árvore Rubro-Negra (VERMELHO/PRETO)
    struct arvore *esq, *dir;
} Arvore;
```

---

## 🤝 Contribuições e Contato

Este projeto foi desenvolvido como um estudo aprofundado para a disciplina de **Estruturas de Dados II**. Feedbacks, revisões de código e discussões sobre otimizações são muito bem-vindos!

Sinta-se à vontade para abrir uma *Issue* ou enviar um *Pull Request*, e não se esqueça de deixar uma ⭐ se este repositório te ajudou a compreender melhor árvores auto-balanceadas!

---
*Criado por [Seu Nome](https://github.com/seu-usuario) — Desenvolvedor de Software.*
