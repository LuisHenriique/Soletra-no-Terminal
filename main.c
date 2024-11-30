/*
Projeto de ICC2 - Soletra no terminal
Autores:
  Luis Henrique Ponciano dos Santos (Nusp: 15577760)
  Gabriel de Araujo Lima            (Nusp: 14571376)
  Pedro Bizon                       (Nusp: 11812667)
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

/* A função principal coordena os comandos */
int main()
{
  /* Declaração de variáveis auxiliares */
  char comand[10];                // Variável usada para armazenar o comando do usuário
  char valid_letters[TAM_LETTER]; // Vetor para guardar as letras válidas
  char inicio[6];                 // Vetor para guardar o comando inicial ("inicio")

  LISTA *lista = criar_lista(); // Criar a lista

  /* Leitura do primeiro comando: "inicio" */
  scanf(" %s", inicio);

  /* Leitura das letras válidas e sendo letra especial a primeira a ser lida */
  for (int i = 0; i < TAM_LETTER - 2; i++)
  {
    scanf(" %c", &valid_letters[i]);
  }
  valid_letters[8] = '\0'; // Adiciona terminador nulo

  /* Chamar a funcao inicio, que */
  inicio_(valid_letters, lista);

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
    if (strcmp(comand, "progresso") == 0) // Caso o comando seja progresso
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

/* Implementação das funções da lista */
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

/* Funcao para imprimir a lista */
void imprimir_lista(LISTA *lista)
{
  if (lista != NULL)
  {
    // Percorre as palavras de 4 a 8 letras
    quick_sort(lista, 0, lista->fim - 1);
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

/* Funcao para apagar a lista*/
void lista_detruir(LISTA **lista)
{
  if ((*lista) != NULL)
  {
    for (int i = 0; i < (*lista)->fim; i++)
    {
    }
  }
}

/* Implementação da busca na lista */
/* Foi utilizado busca binária */
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

/* Implementação das funções dos comandos e funções auxiliares */
void inicio_(char *valid_letters, LISTA *lista)
{
  /*
  A função inicio inicializa a lista principal, atualizando as informações
  Ela abre o arquivo e verifica as palavras validas, se for valida insere

  Parâmetros:
    valid_letters: vetor com as letras validas
    lista: lista onde sera armazenado as palavras validas
  Return: void
  */

  /* Declaração de variáveis auxiliares */
  char especial_letter = valid_letters[0]; // Variavel que armazena a letra que precisa ter, que está na posição 0 de valid_letters
  char word[50];                           // Armazena a palavra lida
  int contador_auxiliar = 0;               // Variavel para percorrer os indices da lista

  /* Abre o arquivo */
  FILE *fp;
  fp = fopen("valid_words.txt", "r");
  if (fp == NULL) // Verificação se o arquivo foi aberto
  {
    printf("Erro ao abrir o arquivo");
    exit(1);
  }

  // Processa cada palavra do arquivo
  while (fscanf(fp, "%s", word) != EOF)
  {
    // Verifica se a palavra é válida
    if (verifica_palavra(word, valid_letters, especial_letter)) // A função verifica_palavra
    {                                                           // retorna 1 se a palavra e valida e 0 se for invalida
      inserir_lista(lista, word);                               // Caso seja valida, insere na lista

      /* Atualização das informações da palavra: indice, tamanho e se ja foi encontrada */
      lista->vetor_auxiliar[contador_auxiliar].indice = contador_auxiliar; // Indice é o indice da palavra na lista original
      lista->vetor_auxiliar[contador_auxiliar].tam = strlen(word);         // Esse indice é guardado porque caso ordenemos o vetor contendo as palavras
      lista->vetor_auxiliar[contador_auxiliar].encontrado = 0;             // Ele ainda guardará o índice de cada palavra na lista original

      // Atualiza a quantidade de palavras por tamanho

      /*
      Observação: essa matriz é matr[2][5]
      A linha representa a quantidade de palavras ja encontradas, linha 0 encontradas, linha 1 total
      A coluna representa a quantidade de letras, coluna 0 as palavras de 4 letras, coluna 1, palavras de 5, etc.

      Por conta disso, salvamos na linha 1, pois estamos inserindo todas as palavras na lista
      Já a coluna é strlen(word) - 4 pois o índice é sempre o tamanho da palavra subtraido de 4 unidades
      ex.: palavra de 4 letras tem que ser inserida na coluna 0, então coluna = 4 - 4
      ex.: palavra de 5 letras tem que ser inserida na coluan 1, então coluna = 5 - 1
      */
      lista->qtd_palavras_por_tamanho[1][(strlen(word)) - 4]++; // O tamanho é incrementado

      contador_auxiliar++; // Incrementa o contador_auxiliar para que a proxima inserção aconteca no proximo indice
    }
  }
  fclose(fp); // Fecha o arquivo
}

/* A funcao a seguir representa o comando "palavra" do usuario */
void palavra(LISTA *lista, char *words)
{
  /*
  A funcao palavra recebe a lista e uma palavra que o usuario digitou
  Depois, analisa se a palavra recebida é valida, se for atualiza as informações
  das palavras encontradas

  Parâmetros:
    words: palavra recebida para ser analisada
    lista: lista onde sera armazenado as palavras validas

  Return: void
  */

  /* Declaração de variaveis auxiliares*/
  int indice; // Indice representa o indice da palavra na lista original

  if (lista_busca(lista, words, &indice)) // Busca a palavra na lista de palavras validas
  {                                       // lista_busca retorna 1 se encontrou e 0 se nao encontrou
    int existe = 0;                       // Flag auxiliar para analisar se a palavra ja foi escrita pelo usuario

    if (lista->vetor_auxiliar[indice].encontrado == 1) // Verifica no vetor auxiliar se a palavra ja foi encontrada
    {                                                  // Vetor auxiliar é um vetor que contem informações de cada palavra
      existe = 1;                                      // Se verificar que a palavra ja foi encontrada, então marca a flag com 1
    }
    /* Caso em que não existe, ou seja, ainda não foi encontrada */
    if (!existe)                                                 // Como não existe, então realiza a atualização das variaveis que
    {                                                            // armazenam informações das palavras encontradas
      lista->vetor_auxiliar[indice].encontrado = 1;              // Marca a palavra como encontrada
      lista->qtd_palavras_encontradas++;                         // Atualiza a quantidade de palavras encontradas
      lista->qtd_palavras_por_tamanho[0][(strlen(words) - 4)]++; // Atualiza a matriz para informar que a quantidade de palavras
      printf("sucesso + 1\n");                                   // encontradas de tamanho strlen(word) foi aumentada em 1 unidade
    }
    else // Caso ja tenha sido encontrada imprime a informação
    {
      printf("palavra ja encontrada\n");
    }
  }
  else // Caso a palavra nao esteja na lista de palavras validas
  {
    printf("palavra invalida\n");
  }
}

/* Função para verificar se a palavra é valida (retorna 1 - valida, retorna 0 - nao valida) */
int verifica_palavra(char *word, char *valid_letters, char especial_letter)
{
  /*
  A função verifica se a palavra é valida, ou seja, se ela atende a todos os criterios
  de possuir as letras validas e tambem conter a letra especial

  Parâmetros:
    word: palavra a ser analisada
    valid_letters: vetor contendo as letras validas
    especial_letter: letra especial, que deve estar na palavra

  Return:
    1 se a palavra for valida
    0 se a palavra nao for valida
  */

  /* Declaração de variáveis auxiliares */
  size_t i;                     // Variável para percorrer
  int especial = 0;             // Variável para armazenar se a letra especial foi encontrada
  int word_size = strlen(word); // Variável para guardar o tamanho da palavra

  /* Comparação inicial do tamanho da palavra */
  if (word_size < 4 || word_size > TAM_LETTER - 1) // Caso tenha tamanho menor que quatro ou maior que
    return 0;                                      // 8, então a palavra é invalida

  /*
  Loop para verificar se cada letra da palavra é valida, ou seja, se a letra se encontra
  entre as letras fornecidas pelo usuario
  */
  for (i = 0; i < strlen(word); i++)
  {
    char letter = word[i];                     // Variavel para armazenar a letra atual
    if (!verify_letter(letter, valid_letters)) // Verifica se a letra é valida com a função verify_letter (1 - valida, 0 - invalida)
      return 0;                                // Se nao e valida, retorna 0, pois a palavra é invalida

    if (letter == especial_letter) // Analise da letra especial, pois ela deve estar presente
      especial = 1;
  }
  return especial; // Passado todos os casos das letras, retornar especial equivale a retornar se a palavra é valida ou invalida
}

/* Funcao para verificar se a letra é valida */
int verify_letter(char letter, char *letters_ok)
{
  /*
  A função recebe uma letra e verifica se ela se encontra entre as letras validas

  Parâmetros:
    letter: letra a ser analisada
    letters_ok: vetor com as letras validas

  Return:
    1 se for letra valida
    0 se for letra invalida
  */

  /* Loop para verificar se a letra esta entre as letras fornecidas pelo usuario */
  for (int i = 0; i < TAM_LETTER; i++)
  {
    if (letter == letters_ok[i]) // Caso esteja entre elas, retorna 1 - letra valida
      return 1;
  }
  return 0; // Caso passe todos os casos e não tenha achado a letra, é invalida
}

/* Funcao para imprimir a quantidade de letras que o usuario ja encontrou e as restantes por quantidade de letras */
void print_vector(LISTA *lista)
{
  /*
  A funcao imprime as informacoes a respeito das palavras ja encontradas
  e das restantes

  Parâmetros:
    lista: lista onde sera armazenado as palavras validas

  Return: void
  */

  /* Verifica se a lista esta vazia */
  if (lista != NULL)
  {
    /* Loop para percorrer por quantidade de letras */
    /*
    Caso i=0, estamos analisando o caso 4 letras
    Caso i=1, estamos analisando o caso 5 letras
    E assim por diante, estamos analisando a quantidade i+4 letras
    */
    for (int i = 0; i < 5; i++)
    {
      /*
      Variavel auxiliar que armazena a quantidade de palavras restantes
      Como temos a quantidade de já encontradas e a quantidade de total por tamanho
      Então fazemos a diferença entre quantidade total e quantidade de já encontradas
      Assim: quantidade de palavras restantes = qtd total - qtd encontradas

      Essa variavel foi usada para deixar o codigo mais limpo
      */
      int qtd_palavras_restantes = lista->qtd_palavras_por_tamanho[1][i] - lista->qtd_palavras_por_tamanho[0][i];

      /* Imprimir a quantidade de palavras encontradas e as restantes por tamanho da palavra */
      /*
      Observação: essa informação encontra-se armazenada na matriz do vetor_auxiliar
      Essa matriz possibilita um acesso direto e eficiente a informação das palavras
      Lembrando que a linha 0 dessa matriz representa a quantidade de palavras encontradas
      E a linha 1 a quantidade de palavras total
      Já as colunas representam a quantidade de letras
      coluna 0: palavras de 4 letras
      coluna 1: palavras de 5 letras
      ...
      coluna i: palavras de i+4 letras
      */
      printf("(%d letras) %d palavra(s) encontrada(s) / %d palavra(s) faltando\n", i + 4, lista->qtd_palavras_por_tamanho[0][i], qtd_palavras_restantes);
    }
  }
}

/* Funções para ordenar */

/* Funcao auxiliar do quicksort */
int particiona(LISTA *lista, int inicio, int fim)
{
  PALAVRA pivo = lista->vetor_auxiliar[fim]; // Pivô
  int i = inicio - 1;

  for (int j = inicio; j < fim; j++)
  {
    // Ordena por tamanho, se iguais mantém a ordem de inserção (índice)
    if (lista->vetor_auxiliar[j].tam < pivo.tam ||
        (lista->vetor_auxiliar[j].tam == pivo.tam && lista->vetor_auxiliar[j].indice < pivo.indice))
    {
      i++;
      // Troca elementos no vetor auxiliar
      PALAVRA temp = lista->vetor_auxiliar[i];
      lista->vetor_auxiliar[i] = lista->vetor_auxiliar[j];
      lista->vetor_auxiliar[j] = temp;
    }
  }

  // Coloca o pivô na posição correta
  PALAVRA temp = lista->vetor_auxiliar[i + 1];
  lista->vetor_auxiliar[i + 1] = lista->vetor_auxiliar[fim];
  lista->vetor_auxiliar[fim] = temp;

  return i + 1;
}

/* Funcao principal do quicksort*/
void quick_sort(LISTA *lista, int inicio, int fim)
{
  if (inicio < fim)
  {
    int pos = particiona(lista, inicio, fim);
    quick_sort(lista, inicio, pos - 1);
    quick_sort(lista, pos + 1, fim);
  }
}