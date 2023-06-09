/*
다양한 좌표 변환이 필요한 이유 : ndc 공간에서 작업하기 불편하기 때문에, 작업 공간마다 좌표계를 다르게 하여 작업한다.
clip space : 동차좌표계에서 w로 나눈 coordinate / view volume에 투영된 space
OpenGL은 NDC만 쓴다 -> 나머지 좌표계는 쓰지 않음
*/

#include <sb7.h>
#include <vmath.h>
#include <shader.h>

class tmp : public sb7::application
{
private:
	GLuint RenderingProgramWings;		// shader program
	GLuint RenderingProgramStick;
	GLuint VertexArrayObject;			// vertex array object(VAO)

public:
	tmp() : RenderingProgramWings(0), RenderingProgramStick(0), VertexArrayObject(0) {}

	GLuint CompileShaderWings()
	{
		GLuint vertexShader;
		GLuint fragmentShader;

		vertexShader = sb7::shader::load("./pinWheel_vs.glsl", GL_VERTEX_SHADER);
		fragmentShader = sb7::shader::load("./pinWheel_ps.glsl", GL_FRAGMENT_SHADER);

		GLuint shaderProgram;
		shaderProgram = glCreateProgram();

		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);

		glLinkProgram(shaderProgram);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		return shaderProgram;
	}
	GLuint CompileShaderStick()
	{
		GLuint vertexShader;
		GLuint fragmentShader;

		vertexShader = sb7::shader::load("./stick_vs.glsl", GL_VERTEX_SHADER);
		fragmentShader = sb7::shader::load("./stick_ps.glsl", GL_FRAGMENT_SHADER);

		GLuint shaderProgram;
		shaderProgram = glCreateProgram();

		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);

		glLinkProgram(shaderProgram);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		return shaderProgram;
	}

	// application initialize
	virtual void startup()
	{
		RenderingProgramWings = CompileShaderWings();
		RenderingProgramStick = CompileShaderStick();

		glGenVertexArrays(1, &VertexArrayObject);
		glBindVertexArray(VertexArrayObject);
	}

	// rendering loop
	virtual void render(double currentTime)
	{
		vmath::vec4 v1;
		vmath::vec4 v2;
		vmath::vec4 v3;

		const GLfloat background[] = { 0.f,		// R
									   0.f,		// G
									   0.f,		// B
									   1.f		// A
		};
		glClearBufferfv(GL_COLOR, 0, background);

		// ---------- draw stick ---------- //
		// define stick's color
		const GLfloat stickColor[] = { 1.f, 1.f, 0.f, 1.f };

		// draw stick : top
		v1 = vmath::vec4(0.01f, 0.f, 0.5f, 1.f);
		v2 = vmath::vec4(-0.01f, 0.f, 0.5f, 1.f);
		v3 = vmath::vec4(-0.01f, -0.8f, 0.5f, 1.f);

		glVertexAttrib4fv(0, stickColor);
		glVertexAttrib1f(1, (float)currentTime);
		glVertexAttrib4fv(2, v1);
		glVertexAttrib4fv(3, v2);
		glVertexAttrib4fv(4, v3);
		glUseProgram(RenderingProgramStick);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// draw stick : down
		v1 = vmath::vec4(0.01f, 0.f, 0.5f, 1.f);
		v2 = vmath::vec4(-0.01f, -0.8f, 0.5f, 1.f);
		v3 = vmath::vec4(0.01f, -0.8f, 0.5f, 1.f);

		glVertexAttrib4fv(0, stickColor);
		glVertexAttrib1f(1, (float)currentTime);
		glVertexAttrib4fv(2, v1);
		glVertexAttrib4fv(3, v2);
		glVertexAttrib4fv(4, v3);
		glUseProgram(RenderingProgramStick);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// deactive shader program //
		glUseProgram(0);

		// ---------- draw wings ---------- //
		// define wing's color //
		const GLfloat wingsColor[] = {
								  (float)sin(currentTime) * (float)cos(currentTime) * 0.5f + 0.5,		// R
								  (float)sin(currentTime) * 0.5f + 0.5f,								// G
								  (float)cos(currentTime) * 0.5f + 0.5f,								// B
								  1.f																	// A
		};

		// draw wing : top //
		v1 = { 0.f, 0.f, 0.5f, 1.f };
		v2 = { 0.f, 0.25f, 0.5f, 1.f };
		v3 = { -0.25f, 0.25f, 0.5f, 1.f };

		glVertexAttrib4fv(0, wingsColor);
		glVertexAttrib1f(1, (float)currentTime);
		glVertexAttrib4fv(2, v1);
		glVertexAttrib4fv(3, v2);
		glVertexAttrib4fv(4, v3);
		glUseProgram(RenderingProgramWings);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// draw wing : left //
		v1 = { 0.f, 0.f, 0.5f, 1.f };
		v2 = { -0.25f, 0.f, 0.5f, 1.f };
		v3 = { -0.25f, -0.25f, 0.5f, 1.f };

		glVertexAttrib4fv(0, wingsColor);
		glVertexAttrib1f(1, (float)currentTime);
		glVertexAttrib4fv(2, v1);
		glVertexAttrib4fv(3, v2);
		glVertexAttrib4fv(4, v3);
		glUseProgram(RenderingProgramWings);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// draw wing : down //
		v1 = { 0.f, 0.f, 0.5f, 1.f };
		v2 = { 0.f, -0.25f, 0.5f, 1.f };
		v3 = { 0.25f, -0.25f, 0.5f, 1.f };

		glVertexAttrib4fv(0, wingsColor);
		glVertexAttrib1f(1, (float)currentTime);
		glVertexAttrib4fv(2, v1);
		glVertexAttrib4fv(3, v2);
		glVertexAttrib4fv(4, v3);
		glUseProgram(RenderingProgramWings);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// draw wing : right //
		v1 = { 0.f, 0.f, 0.5f, 1.f };
		v2 = { 0.25f, 0.f, 0.5f, 1.f };
		v3 = { 0.25f, 0.25f, 0.5f, 1.f };

		glVertexAttrib4fv(0, wingsColor);
		glVertexAttrib1f(1, (float)currentTime);
		glVertexAttrib4fv(2, v1);
		glVertexAttrib4fv(3, v2);
		glVertexAttrib4fv(4, v3);
		glUseProgram(RenderingProgramWings);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// deactive shader program //
		glUseProgram(0);
	}

	virtual void shutdown()
	{
		glDeleteVertexArrays(1, &VertexArrayObject);
		glDeleteProgram(RenderingProgramWings);
		glDeleteProgram(RenderingProgramStick);
	}
};

DECLARE_MAIN(tmp)