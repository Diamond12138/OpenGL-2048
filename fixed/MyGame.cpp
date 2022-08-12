#ifndef mygame_cpp
#define mygame_cpp

#include <iostream>
#include <string>
#include <sstream>
#include <array>
#include <cstdlib>
#include <ctime>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Game.hpp"
#include "main.cpp"

class MyGame : public Game
{
public:
	const unsigned int SCORE_NONE = 0;
	const int NOT_FOUND_NEW_POSITION = -1;

	const float total_clearance = 0.2f;
	const float text_height = 0.1f;

	const float total_length_of_rects_x = 2.0f - total_clearance;
	const float width_per_rect = total_length_of_rects_x / 4.0f;
	const float width_per_clearance = total_clearance / 5.0f;

	const float total_length_of_rects_y = 2.0f - text_height - total_clearance;
	const float height_per_rect = total_length_of_rects_y / 4.0f;
	const float height_per_clearance = total_clearance / 5.0f;

	const glm::vec2 rect_size = glm::vec2(width_per_rect, height_per_rect);

	const float width_span = width_per_clearance + width_per_rect;
	const float height_span = height_per_clearance + height_per_rect;

private:
	void *m_font;
	unsigned int m_score;
	std::array<unsigned int, 16> m_data;

public:
	MyGame()
	{
		m_font = GLUT_STROKE_ROMAN;
		m_score = 0;
		m_data = std::array<unsigned int, 16>();
		for (unsigned i = 0; i < m_data.size(); i++)
		{
			m_data.at(i) = SCORE_NONE;
		}
		srand(time(nullptr));
		random_new();
		if (!initialization())
			quit();
	}

public:
	void quit(int code = EXIT_SUCCESS)
	{
		glutLeaveMainLoop();
		termination();
		exit(code);
	}

	void updateProjectionMatrix()
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		/*float aspect = float(getWindowWidth()) / float(getWindowHeight());
		gluPerspective(60.0f,aspect,0.1f,100.0f);*/
		gluOrtho2D(-1, 1, -1, 1);
		glMatrixMode(GL_MODELVIEW);
	}

	void updateViewMatrix()
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		// gluLookAt(0,0,2,0,0,0,0,1,0);
	}

	void draw_layout_game()
	{
		updateViewMatrix();

		glColor3f(0.9f, 0.9f, 0.9f);
		draw_rect(glm::vec2(-1.0f, 1.0f), glm::vec2(2.0f, 2.0f));

		glColor3f(0.1f, 0.1f, 0.1f);
		draw_text(glm::vec2(0.0f, 1.0f), toString("current score:", m_score));

		int index = 0;
		for (float y = 1.0f - text_height - height_per_clearance; y >= -1.0f + height_per_clearance + height_per_rect; y -= height_span)
		{
			for (float x = -1.0f + width_per_clearance; x <= 1.0f - width_per_clearance - width_per_rect; x += width_span)
			{
				draw_rect_game(glm::vec2(x, y), rect_size, index);
				index++;
			}
		}

		draw_axis();
	}

	void draw_text(glm::vec2 position, std::string text, float size = 5.0f)
	{
		glm::vec2 text_size = measure_text_size(text);
		float ratio = size / 10000.0f;
		text_size *= ratio;

		glPushMatrix();
		{
			glTranslatef(position.x, position.y - text_size.y, 0.0f);
			glScalef(ratio, ratio, 0.0f);
			glutStrokeString(m_font, (const unsigned char *)text.data());
		}
		glPopMatrix();
	}

	void draw_rect(glm::vec2 position, glm::vec2 size)
	{
		glBegin(GL_TRIANGLE_FAN);
		{
			glNormal3f(0.0f, 0.0f, 1.0f);
			glVertex2f(position.x, position.y);
			glVertex2f(position.x, position.y - size.y);
			glVertex2f(position.x + size.x, position.y - size.y);
			glVertex2f(position.x + size.x, position.y);
		}
		glEnd();
	}

	glm::vec2 measure_text_size(std::string text)
	{
		glm::vec2 result = glm::vec2(0.0f, 0.0f);
		for (unsigned int i = 0; i < text.length(); i++)
		{
			float s = glutStrokeWidthf(m_font, text.at(i));
			// std::cout << s << std::endl;
			result.x += s;
		}
		result.y = glutStrokeHeight(m_font);
		return result;
	}

	void draw_axis()
	{
		draw_line(glm::vec2(-1.0f, 0.0f), glm::vec2(1.0f, 0.0f));
		draw_line(glm::vec2(0.0f, -1.0f), glm::vec2(0.0f, 1.0f));
	}

	void draw_grid(float span = 0.1f)
	{
		for (float i = -1.0f; i < 1.0f; i += span)
		{
			draw_line(glm::vec2(i, -1.0f), glm::vec2(i, 1.0f));
		}
		for (float i = -1.0f; i < 1.0f; i += span)
		{
			draw_line(glm::vec2(-1.0f, i), glm::vec2(1.0f, i));
		}
	}

	void draw_line(glm::vec2 v1, glm::vec2 v2)
	{
		glBegin(GL_LINES);
		{
			glVertex2f(v1.x, v1.y);
			glVertex2f(v2.x, v2.y);
		}
		glEnd();
	}

	template <typename T>
	std::string toString(T arg)
	{
		std::stringstream sstr;
		sstr << arg;
		return sstr.str();
	}

	std::string toString()
	{
		return std::string();
	}

	template <typename T, typename... Ts>
	std::string toString(T arg, Ts... args)
	{
		return toString(arg) + toString(args...);
	}

	void draw_rect_game(glm::vec2 position, glm::vec2 size, unsigned int index)
	{
		glColor3f(0.0f, 1.0f, 1.0f);
		draw_rect(position, size);
		int score = m_data.at(index);
		if (score != SCORE_NONE)
		{
			glColor3f(0.1f, 0.1f, 0.1f);
			if (score >= 1024)
			{
				draw_text(position, toString(score), 15.0f);
			}
			else
			{
				draw_text(position, toString(score), 20.0f);
			}
		}
	}

	int find_none_pos()
	{
		for (unsigned int i = 0; i < 16; i++)
		{
			if (m_data.at(i) != SCORE_NONE)
				return i;
		}
		return NOT_FOUND_NEW_POSITION;
	}

	int random_new_pos()
	{
		int result;
		int num = 0;
		do
		{
			result = rand() % 16;
			num++;
		} while (m_data.at(result) != SCORE_NONE && num < 16);
		if (num >= 16)
		{
			return find_none_pos();
		}
		return result;
	}

	void random_new()
	{
		int a = random_new_pos();
		if (a != NOT_FOUND_NEW_POSITION)
		{
			m_data.at(a) = 2;

			int b = random_new_pos();
			if (b != NOT_FOUND_NEW_POSITION)
			{
				m_data.at(b) = 2;
			}

			glutPostRedisplay();
			std::cout << a << std::endl;
		}
	}

public:
	virtual bool initialization()
	{
		updateProjectionMatrix();
		updateViewMatrix();
		// glEnable(GL_DEPTH_TEST);
		return true;
	}

	virtual void termination()
	{
	}

public:
	virtual void display()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		draw_layout_game();

		glutSwapBuffers();
	}

	virtual void keyboard(unsigned char key, int x, int y)
	{
		switch (key)
		{
		case 27:
			quit();
			break;
		default:
			break;
		}
	}

	virtual void special(int key, int x, int y)
	{
	}

	virtual void reshape(int width, int height)
	{
		glViewport(0, 0, width, height);
	}
};

#endif