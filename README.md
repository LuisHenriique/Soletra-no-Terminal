# Projeto de ICC2 - Soletra no Terminal

## Autores

- Luis Henrique Ponciano dos Santos (NUSP: 15577760)
- Gabriel de Araujo Lima (NUSP: 14571376)
- Pedro Bizon (NUSP:11812667)

---

## Resumo do Projeto

Este projeto implementa uma aplicação interativa no terminal que auxilia os jogadores a soletrar palavras válidas utilizando um conjunto de letras fornecido e incluido com uma letra especial. As palavras válidas são organizadas, gerenciadas e analisadas para fornecer feedback durante o jogo. As principais funcionalidades incluem gerenciamento de listas de palavras, busca eficiente(utilizando busca binária) e contagem de ocorrências com base em critérios predefinidos, conforme o usuário for listando as palavras válidas o programa fornece o feedback da palavra encontrada por ele, caso o usuário liste todas as palavras é impresso um feedback final e encerra o jogo.

---

## Estruturas de Dados Implementadas

### 1. **Estrutura PALAVRA**

- **Descrição**: Representa uma palavra válida com informações adicionais sobre seu estado e posição na lista.
- **Campos**:
  - `indice`: Posição da palavra na lista original.
  - `tam`: Tamanho da palavra.
  - `encontrado`: Indica se a palavra já foi encontrada (1) ou não (0).
- **Motivação**: Permite associar os dados de cada palavra, facilitando o controle do progresso do jogo.

### 2. **Estrutura LISTA**

- **Descrição**: Armazena a lista de palavras válidas e informações auxiliares para facilitar operações de busca, ordenação.
- **Campos**:
  - `lista_palavras`: Vetor de strings para armazenar palavras válidas. 
  - `tam`: Número total de palavras válidas na lista.
  - `inicio` e `fim`: Controlam os limites da lista.
  - `palavras_encontradas`: Vetor auxiliar com as palavras já encontradas.
  - `vetor_auxiliar`: Vetor auxiliar com informações como tamanho e estado das palavras.
  - `qtd_palavras_por_tamanho`: Matriz para contagem de palavras com base no tamanho e estado (encontradas/não encontradas).
- **Motivação**: Estrutura central para manipulação e análise de palavras, otimizando buscas e o funcionamento do jogo.

---

## Algoritmos Implementados

### 1. **Busca Binária**

- **Descrição**: Realiza a busca eficiente de palavras na lista ordenada.
- **Motivação**: A busca binária tem complexidade \(O(\log n)\), permitindo localizar palavras rapidamente mesmo em listas extensas.

### 2. **Ordenação com Quicksort**

- **Descrição**: Implementação do algoritmo Quicksort para organizar a lista de palavras em ordem alfabética.
- **Motivação**: O Quicksort é eficiente, com complexidade média \(O(n \log n)\), garantindo uma estrutura ordenada para busca binária e operações subsequentes.

---

## Motivações das Escolhas

1. **Simplicidade e Eficiência**:
   - As estruturas de dados escolhidas são de implementar e possuí eficiência nas operações realizadas.

---
