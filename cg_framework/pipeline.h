#include <iostream>
#include <glm/glm.hpp>
#include <mygl.h>
#include <math.h>
#include <loader.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <bits/stdc++.h>

using namespace glm;
using namespace std;

float angulo = 180.0;

void pipeline()
{
    //Matriz Model

    mat4x4 mTranslation = mat4x4(1, 0, 0, 0,
                                 0, 1, 0, 0,
                                 0, 0, 1, 0,
                                 0, 0, 0, 1);

    mat4x4 mScale = mat4x4(1, 0, 0, 0,
                           0, -1, 0, 0,
                           0, 0, 1, 0,
                           0, 0, 0, 1);

    mat4x4 mRotate = mat4x4(cos(angulo), 0, -sin(angulo), 0,
                            0, 1, 0, 0,
                            sin(angulo), 0, cos(angulo), 0,
                            0, 0, 0, 1);

    angulo = angulo + 0.00;

    mat4x4 mModel = mScale * mTranslation * mRotate;

    //Matriz View

    vec3 lookatCam = vec3(0, 0, 0);
    vec3 posCam = vec3(0, 0, 3);
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

    //Matriz projeção

    int d = 1;

    mat4x4 mProjection = mat4x4(1, 0, 0, 0,
                                0, 1, 0, 0,
                                0, 0, 1, -(1 / d),
                                0, 0, d, 0);

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

    mat4x4 mPipeline = mViewport * mProjection * mView * mModel;

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
}