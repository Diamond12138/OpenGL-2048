#ifndef game_hpp
#define game_hpp
#include <GL/freeglut.h>

class Game
{
	public:
		virtual bool initialization() = 0;
		virtual void termination() = 0;
	
	public:
		virtual void display() = 0;
		virtual void keyboard(unsigned char key,int x,int y) = 0;
		virtual void special(int key,int x,int y) = 0;
		virtual void reshape(int width,int height) = 0;
};

template<typename GameClassType>
GameClassType* getGame()
{
	return ((GameClassType*)glutGetWindowData());
}

int getWindowWidth()
{
	return glutGet(GLUT_WINDOW_WIDTH);
}
int getWindowHeight()
{
	return glutGet(GLUT_WINDOW_HEIGHT);
}

#endif
