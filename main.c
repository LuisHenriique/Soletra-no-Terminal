/*
Projeto de ICC2 - Soletra no terminal
Autores:
  Luis Henrique Ponciano dos Santos (Nusp: 15577760)
  Gabriel de Araujo Lima            (Nusp: 14571376)
  Pedro Bizon                       (Nusp: )
*/

/* Bibliotecas e módulos */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TAM_LETTER 8

/* Definição da estrutura lista sequencial */
struct palavra
{
  int indice;
  int tam;
};
typedef struct palavra PALAVRA;

struct lista
{
  char lista_palavras[1000][TAM_LETTER];
  int tam; // Tamanho da lista (quantidade de palavras válidas)
  int inicio;
  int fim;
  PALAVRA palavras_encontradas[1000]; // Vetor auxiliar que armazena as palavras já encontradas
  int qtd_palavras_encontradas;       // Quantidade de palavras encontradas
  int qtd_palavras_por_tamanho[2][5]; // Indice 0 armazena a quantidade total de palavras de 4 letras
                                      // Indice 1 armazena a quantidade total de palavras de 5 letras, e assim por diante, até 8 letras
};
typedef struct lista LISTA;

/* Protótipos das funções da lista */
LISTA *criar_lista();
void inserir_lista(LISTA *, char *);
void imprimir_lista(LISTA *);
void lista_detruir(LISTA **);

LISTA *criar_lista()
{
  LISTA *lista = (LISTA *)malloc(sizeof(LISTA));
  if (lista != NULL)
  {
    lista->tam = 0;
    lista->fim = 0;
    lista->qtd_palavras_encontradas = 0;
    lista->inicio = 0;
    for (int i = 0; i < 2; i++) // Duas linhas
    {
      for (int j = 0; j < 5; j++) // Cinco colunas por linha
      {
        lista->qtd_palavras_por_tamanho[i][j] = 0;
      }
    }
    return lista;
  }

  return NULL;
}

void inserir_lista(LISTA *lista, char *word)
{
  if (lista != NULL && lista->fim < 1000)
  {
    strcpy(lista->lista_palavras[lista->fim], word);
    lista->fim++;
    lista->tam++;
  }
}

void imprimir_lista(LISTA *lista)
{
  if (lista != NULL)
  {
    for (int j = 0; j < lista->fim; j++)
    {
      printf("%s, ", lista->lista_palavras[j]);
    }
  }
}
void lista_detruir(LISTA **lista)
{
  if ((*lista) != NULL)
  {
    for (int i = 0; i < (*lista)->fim; i++)
    {
    }
  }
}

int lista_busca(LISTA *lista, char *chave, int *indice)
{
  if (lista != NULL)
  {

    int sup, inf, middle;

    inf = lista->inicio;
    sup = lista->fim - 1;

    while (inf <= sup)
    {
      middle = (inf + sup) / 2;
      if (strcmp(lista->lista_palavras[middle], chave) == 0)
      {
        *indice = middle;
        return 1;
      }
      else if (strcmp(lista->lista_palavras[middle], chave) > 0)
        sup = middle - 1;
      else
        inf = middle + 1;
    }
    return 0;
  }
}
/* Protótipos de funções das palavras */
int verify_letter(char, char *);
int verifica_palavra(char *, char *, char);
void inicio_(char *, LISTA *);
void print_vector(LISTA *);
int particiona(LISTA *, int, int);
void quick_sort(LISTA *, int, int);

/* A função principal coordena os comandos */
int main()
{
  int indice;
  char comand[10];
  char valid_letters[TAM_LETTER];
  char inicio[6];
  LISTA *lista = criar_lista();

  scanf(" %s", inicio);
  // Leitura das letras válidas e sendo letra especial a primeira a ser lida
  for (int i = 0; i < TAM_LETTER; i++)
  {
    scanf(" %c", &valid_letters[i]);
  }
  valid_letters[8] = '\0'; // Adiciona terminador nulo
  inicio_(valid_letters, lista);

  while (1)
  {

    scanf(" %s", comand);

    // comando para receber uma palavra válida
    if (strcmp(comand, "palavra") == 0)
    {
      char words[50];
      scanf("%s", words);

      if (lista_busca(lista, words, &indice))
      {
        int existe = 0;

        for (int i = 0; i < lista->tam; i++)
        {
          if (lista->palavras_encontradas[i].indice == indice)
          {
            existe = 1;
            break;
          }
        }

        if (!existe)
        {

          lista->palavras_encontradas[lista->qtd_palavras_encontradas].indice = indice;
          lista->palavras_encontradas[lista->qtd_palavras_encontradas].tam = strlen(words);
          lista->qtd_palavras_encontradas++;

          lista->qtd_palavras_por_tamanho[0][(strlen(words) - 4)]++;

          printf("sucesso + 1\n");
          if (lista->qtd_palavras_encontradas == lista->tam)
          {
            printf("parabens! voce encontrou todas as palavras” deve ser mostrada e o programa é encerrado");
            break;
          }
        }
      }
      else
        printf("palavra invalida\n");
    }
    if (strcmp(comand, "progresso") == 0)
    {
      printf("progresso atual:\n");
      print_vector(lista);
    }
    // Comando para finalizar a tarefa
    if (strcmp(comand, "solucao") == 0)
    {
      printf("para encerrar o jogo estavam faltando as palavras:\n");
      imprimir_lista(lista);
      for (int j = 0; j < lista->qtd_palavras_encontradas; j++)
      {
        printf(" \nindice[%d] ", lista->palavras_encontradas[j].indice);
        printf(" \ntam[%d] ", lista->palavras_encontradas[j].tam);
      }
      break;
    }
  }

  // lista_destruir();
  return 0;
}

void inicio_(char *valid_letters, LISTA *lista)
{
  char especial_letter = valid_letters[0];
  char word[50];
  //  Verificando o arquivo

  FILE *fp;
  fp = fopen("/home/luishenrique/Desktop/Projects/ICC2 - Project 2/valid_words.txt", "r");
  if (fp == NULL)
  {
    printf("Erro abrir o arquivo");
    exit(1);
  }

  // Processa a palavra do arquivo
  while (fscanf(fp, "%s", word) != EOF)
  {
    // verificar se palavra é válida, se for, insere no array.

    if (verifica_palavra(word, valid_letters, especial_letter))
    {
      inserir_lista(lista, word);
      lista->qtd_palavras_por_tamanho[1][(strlen(word)) - 4]++;
    }
  }
  fclose(fp);
}
// Função que verifica se a palavra lida é válida
int verifica_palavra(char *word, char *valid_letters, char especial_letter)
{
  size_t i;
  int especial = 0;
  int word_size = strlen(word);
  if (word_size < 4 || word_size > 7)
    return 0;

  for (i = 0; i < strlen(word); i++)
  {
    char letter = word[i];
    if (!verify_letter(letter, valid_letters))
      return 0;

    if (letter == especial_letter)
      especial = 1;
  }
  return especial;
}

int verify_letter(char letter, char *letters_ok)
{

  for (int i = 0; i < TAM_LETTER; i++)
  {
    if (letter == letters_ok[i])
      return 1;
  }
  return 0;
}

void print_vector(LISTA *lista)
{
  if (lista != NULL)
  {
    quick_sort(lista, 0, lista->qtd_palavras_encontradas - 1);
  }
  for (int i = 0; i < 5; i++)
  {
    int qtd_palavras_restantes = lista->qtd_palavras_por_tamanho[1][i] - lista->qtd_palavras_por_tamanho[0][i];

    printf("(%d letras) %d palavra(s) encontrada(s) / %d palavra(s) faltando\n", i + 4, lista->qtd_palavras_por_tamanho[0][i], qtd_palavras_restantes);
  }
}

int particiona(LISTA *lista, int beggin, int end)
{
  int middle = (beggin + end) / 2;
  int pivo = (lista->palavras_encontradas[beggin].tam + lista->palavras_encontradas[end].tam + lista->palavras_encontradas[middle].tam) / 3; // 1º passo calcula a mediana
  while (beggin < end)
  {
    // 2°passo verifica o vetor da esquerda para direita procurando um maior que o pivô
    while (beggin < end && lista->palavras_encontradas[beggin].tam <= pivo)
      beggin++;

    // 3°passo verifica o vetor da direita para esquerda procurando um menor que o pivô
    while (beggin < end && lista->palavras_encontradas[end].tam > pivo)
      end--;

    // 4° passo trocas do i(beggin) com j(end)
    int aux = lista->palavras_encontradas[beggin].tam;
    lista->palavras_encontradas[beggin].tam = lista->palavras_encontradas[end].tam;
    lista->palavras_encontradas[end].tam = aux;
  }
  return beggin; // Aqui pode ser tanto o início quanto o fim, pois ambos tem o mesmo índice beggin == end
}

void quick_sort(LISTA *lista, int beggin, int end)
{
  if (beggin < end)
  {
    int pos = particiona(lista, beggin, end);
    quick_sort(lista, beggin, pos - 1);
    quick_sort(lista, pos, end);
  }
}