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
#define TAM_LETTER 7

/* Definição da estrutura lista sequencial */
struct lista
{
  int tam;
  int inicio;
  int fim;
  char valid_word[5000][TAM_LETTER];
};
typedef struct lista LISTA;

/* Protótipos das funções da lista */
LISTA *criar_lista();
void inserir_lista(LISTA *lista, char *word);
void imprimir_lista(LISTA *lista);
void lista_detruir(LISTA **lista);

LISTA *criar_lista()
{
  LISTA *lista = (LISTA *)malloc(sizeof(LISTA));
  if (lista != NULL)
  {
    lista->tam = 0;
    lista->fim = 0;
    lista->inicio = 0;
    return lista;
  }

  return NULL;
}

void inserir_lista(LISTA *lista, char *word)
{
  if (lista != NULL && lista->fim < 5000)
  {
    strcpy(lista->valid_word[lista->fim], word);
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
      printf("%s", lista->valid_word[j]);
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
/* Protótipos de funções das palavras */
int verify_letter(char, char *);
int valid_word(char *, char *, char);

/* A função principal coordena os comandos */
int main()
{

  char comand[10];
  char valid_letters[TAM_LETTER];
  char word[50];
  char especial_letter;

  LISTA *lista = criar_lista();

  // Iniciando o jogo, com o comando inicio.
  char inicio[6];
  scanf(" %s", inicio);
  // Leitura das letras válidas e sendo letra especial a primeira a ser lida
  for (int i = 0; i < TAM_LETTER; i++)
  {
    scanf(" %c", &valid_letters[i]);
  }
  especial_letter = valid_letters[0];
  //  Verificando o arquivo
  FILE *
      fp;
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

    printf("OPq");
    if (valid_word(word, valid_letters, especial_letter))
    {
      printf("Caiu aqui");
      inserir_lista(lista, word);
    }
  }

  fclose(fp);

  imprimir_lista(lista);

  while (1)
  {

    scanf(" %s", comand);

    // comando para receber uma palavra válida
    if (strcmp(comand, "palavra") == 0)
    {
      char *palavra = (char *)malloc(sizeof(char) * 7);
      if (palavra != NULL)
        scanf(" %s", palavra);
    }
    if (strcmp(comand, "progresso") == 0)
    {
    }
    // Comando para finalizar a tarefa
    if (strcmp(comand, "solucao") == 0)
    {
      printf("fim\n");
      imprimir_lista(lista);
      break;
    }
  }

  // lista_destruir();
  return 0;
}

/* Função que verifica se a palavra lida é válida */
int valid_word(char *word, char *valid_letters, char especial_letter)
{
  /*
  A função valid_word é usada para saber se uma palavra pode ou não entrar na lista
  de palavras válidas do jogo, ou seja, aquelas que contêm somente as letras especificadas, além da especial

  Parâmetros:
    word: palavra a ser analisada
    valid_letters: string que contém as letras válidas
    especial_letter: letra especial que deve aparecer na palavra
  */
  size_t i;
  int especial = 0;
  int word_size = strlen(word);

  int verify = 1;
  if ((word_size < 4) || (word_size > 7))
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