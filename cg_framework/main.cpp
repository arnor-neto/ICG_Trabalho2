#include "main.h"
#include <pipeline.h>

//-----------------------------------------------------------------------------
void MyGlDraw(void)
{
	pipeline();

	struct pixel p1 = {0, 0, 0, 0, 0, 255};
	struct pixel p2 = {0, 511, 0, 0, 0, 255};

	for (int i = 0; i < 512; i++)
	{	
		p1.X = i;
		p2.X = i;

		drawLine(p1, p2);
	}

	pipeline();
}

//-----------------------------------------------------------------------------
int main(int argc, char **argv)
{
	// Inicializações.
	InitOpenGL(&argc, argv);
	InitCallBacks();
	InitDataStructures();

	// Ajusta a função que chama as funções do mygl.h
	DrawFunc = MyGlDraw;

	// Framebuffer scan loop.
	glutMainLoop();

	return 0;
}
