# Introdução à Computação Gráfica

## Trabalho 2: Implementação de um pipeline gráfico

O objetivo deste trabalho é implementar um pipeline gráfico em c++ usando as matrizes de transformação para manipular vértices, que permitam a formação de modelos na tela com a sensação de distorção perspectiva.
Para tal implementação, utilizamos o framework fornecido pelo professor Christian Azambuja Pagot.

Trabalho desenvolvido pelos alunos:

Arnôr Martiniano da Silva Neto - Matrícula: 20170011130

Dandara Estrela Braga - Matrícula: 20170000870

### Introdução: O pipeline gráfico
O pipeline gráfico consiste em uma série de tranformações aplicadas à vértices de um objeto tridimensional com o objetivo de representá-los em uma tela bidimensional. O pipeline se divide em etapas e cada uma delas aplica matrizes para transformar os vértices entre sistemas de coordenadas. Nosso pipeline contém as seguintes tranformações:

![untitleddocument](https://user-images.githubusercontent.com/42072854/45761005-c28fe700-bc01-11e8-9f75-7de5e3565d01.jpeg)

### 1 - Do Epaço do Objeto para o Espaço do Universo
Tendo as coordenadas dos vértices do objeto, é preciso posicionar o objeto no universo.
