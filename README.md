# Introdução à Computação Gráfica

## Trabalho 2: Implementação de um pipeline gráfico

O objetivo deste trabalho é implementar um pipeline gráfico em c++ usando as matrizes de transformação para manipular vértices, que permitam a formação de modelos na tela com a sensação de distorção perspectiva.
Para tal implementação, utilizamos o framework fornecido pelo professor Christian Azambuja Pagot e a biblioteca Assimp.

Trabalho desenvolvido pelos alunos:

Arnôr Martiniano da Silva Neto - Matrícula: 20170011130

Dandara Estrela Braga - Matrícula: 20170000870

### Introdução: O pipeline gráfico
O pipeline gráfico consiste em uma série de tranformações aplicadas à vértices de um objeto tridimensional com o objetivo de representá-los em uma tela bidimensional. O pipeline se divide em etapas e cada uma delas aplica matrizes para transformar os vértices entre sistemas de coordenadas. Nosso pipeline contém as seguintes tranformações:

![45761005-c28fe700-bc01-11e8-9f75-7de5e3565d01-crop](https://user-images.githubusercontent.com/42072854/45788157-3dcbba00-bc4f-11e8-9772-1a03c5555128.jpeg)

### 1 - Do Espaço do Objeto para o Espaço do Universo
Tendo as coordenadas dos vértices do objeto, é preciso posicionar o objeto no universo. Para isso, usamos a matriz Model. Essa matriz é composta por um produto de matrizes de escala, translação e rotação.

Matriz de Escala:
Altera o tamanho do objeto no espaço do universo.

![image](https://user-images.githubusercontent.com/42072854/45788116-03fab380-bc4f-11e8-8986-dbd78c0dac5f.png)

Matriz de Translação:
Move um objeto no espaço do universo.

![image](https://user-images.githubusercontent.com/42072854/45788127-14129300-bc4f-11e8-9369-1d408a6142e2.png)

Matriz de Rotação:
Rotaciona um objeto no espaço do universo. Para nossos testes, usamos a rotação no eixo Y.

![image](https://user-images.githubusercontent.com/42072854/45788146-255b9f80-bc4f-11e8-8556-4ec765cef6ef.png)

No código, definimos as matrizes usando a biblioteca GLM, que recebe colunas no formato de linha:
```
//Matriz Model

    mat4x4 mTranslation = mat4x4(1, 0, 0, 0,
                                 0, 1, 0, 0,
                                 0, 0, 1, 0,
                                tx, ty, tz, 1); //tx, ty e tz são os valores de translação

    mat4x4 mScale = mat4x4(sx, 0, 0,  0,
                           0, sy, 0,  0,
                           0,  0, sz, 0,
                           0,  0, 0,  1); //sx, sy e sz são os valores de escala

    mat4x4 mRotate = mat4x4(cos(angulo), 0, -sin(angulo), 0,
                                 0,      1,       0,      0,
                            sin(angulo), 0, cos(angulo),  0,
                                 0       0,       0,      1); //rotação no eixo Y, em relação ao ângulo

    mat4x4 mModel = mScale * mTranslation * mRotate; //Construção da matriz Model

```
### 2 - Do Espaço do Universo para o Espaço da Câmera
Depois de os objetos estarem devidamente posicionados no Espaço do Universo, é preciso definir uma câmera que funcionará como ponto de vista do espaço montado. Para construção da câmera, são necessárias sua posição, o vetor up (que define sua orientação) e o vetor look at, que indica o ponto para qual a câmera está direcionada. Com esses parâmetros, é possível construir um sistema de eixos e, posteriormente, uma matriz para o espaço da câmera.

![image](https://user-images.githubusercontent.com/42072854/45788229-98fdac80-bc4f-11e8-9a94-6023cb55d944.png)

![image](https://user-images.githubusercontent.com/42072854/45770412-b3686380-bc18-11e8-9b29-8ea0477033d9.png)

Na implementação, são recebidos os parâmetros e é gerado o vetor distância com base no look at e na posição para o cálculo do sistema de coordenadas da câmera.  
```
  //Matriz View

    vec3 lookatCam = vec3(0, 0, 0);
    vec3 posCam = vec3(3, 2, 2);
    vec3 upCam = vec3(0, 1, 0);

    vec3 dirCam = lookatCam - posCam;

    vec3 zCam = -normalize(dirCam);
    vec3 xCam = normalize(cross(upCam, zCam));
    vec3 yCam = cross(zCam, xCam);

    mat4x4 Bt = mat4x4(xCam[0], yCam[0], zCam[0], 0,
                       xCam[1], yCam[1], zCam[1], 0,
                       xCam[2], yCam[2], zCam[2], 0,
                       0, 0, 0, 1);

    mat4x4 T = mat4x4(1, 0, 0, 0,
                      0, 1, 0, 0,
                      0, 0, 1, 0,
                      -dirCam[0], -dirCam[1], -dirCam[2], 1);

    mat4x4 mView = Bt * T;
``` 
### 3 - Do Espaço da Câmera para o Espaço de Recorte
O espaço de recorte é responsável por transformar os vértices do objeto permitindo a aplicação da distorção perspectiva. A semelhança de triângulos foi usada para o cálculo da matriz de projeção que é combinada à uma matriz de translação que corrige a distância da câmera ao plano de visão.

![image](https://user-images.githubusercontent.com/42072854/45771040-6be2d700-bc1a-11e8-9c8d-cdd88af1c7d9.png)

No pipeline, o valor d representa a distância focal da câmera que observa a cena.
```
//Matriz projeção

    int d = 1;

    mat4x4 mProjection = mat4x4(1, 0, 0,     0   ,
                                0, 1, 0,     0   ,
                                0, 0, 1, -(1 / d),
                                0, 0, d,     0  );

```
### 4 - Do Espaço de Recorte para o Espaço Canônico
Quando a transformação para o espaço de recorte é aplicada, o valor da coordenada homogênea (w) dos vértices se torna um valor provavelmente diferente de 1, é preciso transformar os vértices de volta para o espaço euclidiano para que possam ser exibidos em uma tela bidimensional com a distorção perspectiva.

![image](https://user-images.githubusercontent.com/42072854/45771687-28896800-bc1c-11e8-835c-5cc40a89b8e2.png)

Também é possível efetuar a homogenização dos vértices ao fim do pipeline, tornando possível a união de todas as matrizes do pipeline por meio de produto de matrizes, isso foi feito no pipeline implementado.

### 5 - Do Espaço Canônico para o Espaço de Tela
A última etapa do pipeline transforma os vértices para o espaço de tela, permitindo que sejam rasterizados com o algoritmo de Bresenham implementado no Trabalho I. Essa transformação consiste em uma matriz que inverte o eixo Y, uma que adapta o tamanho do objeto ao lado positivo do plano euclidiano e uma matriz que transforma os vértices para que possam ser rasterizados nas coordenadas de tela.

![image](https://user-images.githubusercontent.com/42072854/45772460-29bb9480-bc1e-11e8-823d-4a3ed6e135db.png)

```
//Matriz viewport

    int w = 512;
    int h = 512;

    mat4x4 mViewTranslate = mat4x4(1, 0, 0, 0,
                                   0, 1, 0, 0,
                                   0, 0, 1, 0,
                                   (w - 1) / 2, (h - 1) / 2, 0, 1);

    mat4x4 mViewScale = mat4x4(w / 2, 0, 0, 0,
                               0, h / 2, 0, 0,
                               0, 0, 1, 0,
                               0, 0, 0, 1);

    mat4x4 mViewInvert = mat4x4(1, 0, 0, 0,
                                0, -1, 0, 0,
                                0, 0, 1, 0,
                                0, 0, 0, 1);

    mat4x4 mViewport = mViewTranslate * mViewScale * mViewInvert;
```

### Resultados
Com o uso da biblioteca Assimp, carregamos os vértices do modelo e desenhamos suas primitivas com o função drawTriangle desenvolvida no trabalho 1.

```
//Carregando o modelo e desenhando

    vector<glm::vec4> vertices = loadModel("monkey.obj");

    for (int i = 0; i < vertices.size(); i++)
    {
        vertices[i] = mPipeline * vertices[i];
        vertices[i] = vertices[i] / vertices[i].w;
    }

    for (int i = 0; i < vertices.size(); i += 3)
    {

        struct pixel p1 = {(int)vertices[i].x, (int)vertices[i].y, 255, 255, 255, 255};

        struct pixel p2 = {(int)vertices[i + 1].x, (int)vertices[i + 1].y, 255, 255, 255, 255};

        struct pixel p3 = {(int)vertices[i + 2].x, (int)vertices[i + 2].y, 255, 255, 255, 255};

        drawTriangle(p1, p2, p3);
    }
```

![image](https://user-images.githubusercontent.com/42072854/45791655-f0584880-bc60-11e8-9e75-b178baac61a7.png)

![image](https://user-images.githubusercontent.com/42072854/45791709-36ada780-bc61-11e8-8c53-77811ed68fc2.png)

### Dificuldades Encontradas
A maior dificuldade encontrada foi no fim do desenvolvimento, não conseguimos dominar os conhecimentos do loader e dos parâmetros avançados da câmera para fazer a comparação do modelo com o OpenGL no tempo limite de entrega do trabalho.

## Referências

 - Slides de aula do professor Christian Azambuja Pagot

 - http://matheuspraxedescg.blogspot.com/

