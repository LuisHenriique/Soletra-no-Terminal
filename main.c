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
#define TAM_LETTER 9 /* Constante para a quantidade máxima de letras */

/* Definição da estrutura palavra */
struct palavra
{
  /*
  A estrutura palavra é usada para armazenar informações importantes
  de uma palavra válida, como o indice na lista original, o tamanho e se ela
  ja foi encontrada ou nao
  */
  int indice;     // Indice da palavra na lista original
  int tam;        // Tamanho da palavra
  int encontrado; // 1 se foi encontrada e 0 se não
  /* Observação: lista original é a lista contendo todas as palavras válidas */
};
typedef struct palavra PALAVRA;

/* Definição da estrutura lista */
struct lista
{
  /*
  A estrutura lista é utilizada para armazenar a lista de palavras válidas
  Além disso, também há um vetor auxiliar contendo informações importantes sobre cada palavra
  dessa lista
  */
  char lista_palavras[1000][TAM_LETTER]; // Vetor que armazena as palavras válidas
  int tam;                               // Tamanho da lista (quantidade de palavras válidas)
  int inicio;                            // Indice do inicio da lista
  int fim;                               // Indice do final da lista
  PALAVRA palavras_encontradas[1000];    // Vetor auxiliar que armazena as palavras já encontradas
  PALAVRA vetor_auxiliar[1000];          // Vetor auxiliar que em cada elemento representa uma palavra, armazenando se ela ja foi encontrada e seu tamanho
  int qtd_palavras_encontradas;          // Quantidade de palavras encontradas
  int qtd_palavras_por_tamanho[2][5];    // Indice 0 armazena a quantidade total de palavras de 4 letras
                                         // Indice 1 armazena a quantidade total de palavras de 5 letras, e assim por diante, até 8 letras
  /*
  A matriz qtd_palavras_por_tamanho é usada para podermos acessar diretamente
  a quantidade de palavras por cada tamanho (4 letras, 5 letras, ...) de maneira simples
  Isso pode ser feito tanto para as palavras já encontradas (linha 0), ou para todas
  as palavras válidas (linha 1)
  Exemplo da matriz qtd_palavras_por_tamanho
            coluna 0 | coluna 1 | coluna 2 | coluna 3 | coluna 4
  linha 0      4          3         ...         ...       ...
  linha 1      10         5         ...         ...       ...

  Observa-se antes que a coluna 0 é para as palavras de 4 letras, coluna 1 para de 5 letras,
  coluna 2 para 6 letras, assim por diante até 8 letrass.
  Por sua vez, a linha é para indicar se a quantidade de palavras é a quantidade
  que já foi encontrada pelo usuário - linha 0 - (essa quantidade vai variando ao longo da dinâmica)
  ou é a quantidade total de palavras da quantidade de letras da coluna - linha 1

  Nesse sentido, na linha 0 e coluna 0 temos 4, ou seja, temos 4 palavras encontradas
  pelo usuário de 4 letras. Na linha 1 e coluna 0 temos 10, ou seja, existem 10
  palavras no total de 4 letras.

  */
};
typedef struct lista LISTA;

/* Protótipos das funções da lista */
LISTA *criar_lista();
void inserir_lista(LISTA *, char *);
void imprimir_lista(LISTA *);
void lista_detruir(LISTA **);

/* Protótipos de funções das palavras */
int verify_letter(char, char *);
int verifica_palavra(char *, char *, char);

void inicio_(char *, LISTA *);
void print_vector(LISTA *);
void palavra(LISTA *lista, char *words);

/* Protótipos de funções de ordenação */
/* Foi implementado a ordenação quicksort */
int particiona(LISTA *, int, int); // Função auxiliar do quicksort
void quick_sort(LISTA *, int, int);

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
    // Percorre as palavras de 4 a 8 letras
    for (int tamanho = 0; tamanho < 5; tamanho++)
    {                                    // 4 letras até 8 letras
      int tamanho_palavra = tamanho + 4; // Calcula o tamanho das palavras (4, 5, 6, 7, 8)
      printf("(%d letras) ", tamanho_palavra);

      int primeiro = 1; // Flag para controle da vírgula
      for (int j = 0; j < lista->tam; j++)
      {
        // Verifica se a palavra tem o tamanho correto e se não foi encontrada
        if (lista->vetor_auxiliar[j].tam == tamanho_palavra)
        { // Teria o && lista->vetor_auxiliar[j].encontrado == 0 para imprimir as restantes
          if (!primeiro)
          {
            printf(", "); // Adiciona vírgula antes da palavra, exceto para a primeira
          }
          printf("%s", lista->lista_palavras[lista->vetor_auxiliar[j].indice]);
          primeiro = 0; // Depois da primeira palavra, a flag muda
        }
      }
      printf("\n");
    }
    printf("fim!\n");
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

/* A função principal coordena os comandos */
int main()
{
  // int indice;
  char comand[10];
  char valid_letters[TAM_LETTER];
  char inicio[6];
  LISTA *lista = criar_lista();

  scanf(" %s", inicio);
  // Leitura das letras válidas e sendo letra especial a primeira a ser lida
  for (int i = 0; i < TAM_LETTER - 2; i++)
  {
    scanf(" %c", &valid_letters[i]);
  }
  valid_letters[8] = '\0';       // Adiciona terminador nulo
  inicio_(valid_letters, lista); // inicio está fora do loop while, assim é chamado apenas 1 vez

  while (1)
  {
    scanf(" %9s", comand);
    // comando para receber uma palavra válida
    if (strcmp(comand, "palavra") == 0)
    {
      char words[50];
      scanf("%s", words);
      palavra(lista, words); // Chamada da função palavra
      if (lista->qtd_palavras_encontradas == lista->tam)
      {
        printf("parabens! voce encontrou todas as palavras\n");
        break; // Encerra a função, pois todas as palavras foram encontradas
      }
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
  int contador_auxiliar = 0;

  // Verifica o arquivo de palavras
  FILE *fp;
  fp = fopen("valid_words.txt", "r");
  if (fp == NULL)
  {
    printf("Erro ao abrir o arquivo");
    exit(1);
  }

  // Processa cada palavra do arquivo
  while (fscanf(fp, "%s", word) != EOF)
  {
    // Verifica se a palavra é válida
    if (verifica_palavra(word, valid_letters, especial_letter))
    {
      inserir_lista(lista, word);
      lista->vetor_auxiliar[contador_auxiliar].indice = contador_auxiliar;
      lista->vetor_auxiliar[contador_auxiliar].tam = strlen(word);
      lista->vetor_auxiliar[contador_auxiliar].encontrado = 0;

      // Atualiza a quantidade de palavras por tamanho
      lista->qtd_palavras_por_tamanho[1][(strlen(word)) - 4]++;
      contador_auxiliar++;
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
  if (word_size < 4 || word_size > TAM_LETTER)
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

    for (int i = 0; i < 5; i++)
    {
      int qtd_palavras_restantes = lista->qtd_palavras_por_tamanho[1][i] - lista->qtd_palavras_por_tamanho[0][i];

      printf("(%d letras) %d palavra(s) encontrada(s) / %d palavra(s) faltando\n", i + 4, lista->qtd_palavras_por_tamanho[0][i], qtd_palavras_restantes);
    }
  }
}

int particiona(LISTA *lista, int beggin, int end)
{
  int middle = (beggin + end) / 2;
  int pivo = (lista->vetor_auxiliar[beggin].tam + lista->vetor_auxiliar[end].tam + lista->vetor_auxiliar[middle].tam) / 3; // 1º passo calcula a mediana
  while (beggin < end)
  {
    // 2°passo verifica o vetor da esquerda para direita procurando um maior que o pivô
    while (beggin < end && lista->vetor_auxiliar[beggin].tam <= pivo)
      beggin++;

    // 3°passo verifica o vetor da direita para esquerda procurando um menor que o pivô
    while (beggin < end && lista->vetor_auxiliar[end].tam > pivo)
      end--;

    // 4° passo trocas do i(beggin) com j(end)
    int aux = lista->vetor_auxiliar[beggin].tam;
    lista->vetor_auxiliar[beggin].tam = lista->vetor_auxiliar[end].tam;
    lista->vetor_auxiliar[end].tam = aux;
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

void palavra(LISTA *lista, char *words)
{
  int indice;

  if (lista_busca(lista, words, &indice))
  {
    int existe = 0;

    if (lista->vetor_auxiliar[indice].encontrado == 1)
    {
      existe = 1;
    }

    if (!existe)
    {
      lista->vetor_auxiliar[indice].encontrado = 1;
      lista->qtd_palavras_encontradas++;
      lista->qtd_palavras_por_tamanho[0][(strlen(words) - 4)]++;

      printf("sucesso + 1\n");
    }
    else
    {
      printf("palavra ja encontrada\n");
    }
  }
  else
  {
    printf("palavra invalida\n");
  }
}