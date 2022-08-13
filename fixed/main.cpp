#ifndef main_cpp
#define main_cpp

#include <iostream>
#include <memory>
#include <GL/freeglut.h>
#include "MyGame.cpp"

void display();
void keyboard(unsigned char key,int x,int y);
void special(int key,int x,int y);
void reshape(int width,int height);

int main(int argc,char* argv[])
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
	
	glutCreateWindow("2048");
	glutFullScreen();
	
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutReshapeFunc(reshape);
	
	//shared_ptr will release memory automatically
	std::shared_ptr<MyGame> game = std::make_shared<MyGame>();
	glutSetWindowData(game.get());
	
	glutMainLoop();
	
	return 0;
}

void display()
{
	getGame<MyGame>()->display();
}

void keyboard(unsigned char key,int x,int y)
{
	getGame<MyGame>()->keyboard(key,x,y);
}

void special(int key,int x,int y)
{
	getGame<MyGame>()->special(key,x,y);
}

void reshape(int width,int height)
{
	getGame<MyGame>()->reshape(width,height);
}

#endif