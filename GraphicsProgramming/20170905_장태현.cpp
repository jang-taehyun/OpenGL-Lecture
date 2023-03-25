#include <sb7.h>

class tmp : public sb7::application
{
private:
	GLuint RenderingProgramWings;		// shader program
	GLuint RenderingProgramStick;
	GLuint VertexArrayObject;		// vertex array object(VAO)
public:
	GLuint CompileShaderWings()
	{
		// shader object ID
		GLuint vertexShader;
		GLuint fragmentShader;

		// 1. shader object 생성
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		fprintf(stdout, "\n1. create shader object\n");

		// 2. shader source code를 shader object에 전달 : shader source code는 각 vertex마다 호출됨(vertex의 개수만큼 호출된다!)
		const GLchar* vertexShaderSource[] =
		{
			// CCW 방식
			"#version 430 core												\n"
			"																\n"
			"layout (location = 0) in vec4 color;							\n"
			"																\n"
			"out vec4 vs_color;												\n"
			"																\n"
			"void main()													\n"
			"{																\n"
			"	const vec4 vertices[12] = vec4[12](							\n"
											// top
			"								vec4(0.f, 0.f, 0.5f, 1.f), \n"
			"								vec4(0.f,  0.5f, 0.5f, 1.f), \n"
			"								vec4(-0.5f, 0.5f, 0.5f, 1.f), \n"
											// left
			"								vec4(0.f, 0.f, 0.5f, 1.f), \n"
			"								vec4(-0.5f, 0.f, 0.5f, 1.f),\n"
			"								vec4(-0.5f, -0.5f, 0.5f, 1.f),  \n"
											// down
			"								vec4(0.f, 0.f, 0.5f, 1.f), \n"
			"								vec4(0.f, -0.5f, 0.5f, 1.f),\n"
			"								vec4(0.5f, -0.5f, 0.5f, 1.f),  \n"
											// right
			"								vec4(0.f, 0.f, 0.5f, 1.f), \n"
			"								vec4(0.5f, 0.f, 0.5f, 1.f),\n"
			"								vec4(0.5f, 0.5f, 0.5f, 1.f)  \n"
			"								);								\n"
			"	gl_Position = vertices[gl_VertexID];						\n"
			"	vec4 input[12] = vec4[12](									\n"
			"							vec4(color[0], 0.f, 0.f, 1.f),		\n"
			"							vec4(0.f, color[1], 0.f, 1.f),		\n"
			"							vec4(0.f, 0.f, color[2], 1.f),		\n"

			"							vec4(color[0], 0.f, 0.f, 1.f),		\n"
			"							vec4(0.f, color[1], 0.f, 1.f),		\n"
			"							vec4(0.f, 0.f, color[2], 1.f),		\n"

			"							vec4(color[0], 0.f, 0.f, 1.f),		\n"
			"							vec4(0.f, color[1], 0.f, 1.f),		\n"
			"							vec4(0.f, 0.f, color[2], 1.f),		\n"

			"							vec4(color[0], 0.f, 0.f, 1.f),		\n"
			"							vec4(0.f, color[1], 0.f, 1.f),		\n"
			"							vec4(0.f, 0.f, color[2], 1.f));		\n"
			"	vs_color = input[gl_VertexID];								\n"
			"}																\n"

		};
		glShaderSource(vertexShader, 1, vertexShaderSource, NULL);
		fprintf(stdout, "2-1. vertex shader source code attach to vertex shader object\n");

		const GLchar* fragmentShaderSource[]
		{
			"#version 430 core						\n"
			"										\n"
			"in vec4 vs_color;						\n"
			"out vec4 color;						\n"
			"										\n"
			"void main()							\n"
			"{										\n"
			"	color = vs_color;					\n"
			"}										\n"
		};
		glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);
		fprintf(stdout, "2-2. fragment shader source code attach to fragment shader object\n");

		// 3. shader sourse code compile
		glCompileShader(vertexShader);
		glCompileShader(fragmentShader);
		fprintf(stdout, "3. shader compile\n");

		// 4. shader program object 생성
		GLuint shaderProgram;
		shaderProgram = glCreateProgram();

		// 5. shader program object에 shader object attach
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		fprintf(stdout, "4. create shader program & attach shader object ");

		// 6. shader program에 attach된 모든 shader object를 link ==> OpenGL과 shader program을 link
		glLinkProgram(shaderProgram);
		fprintf(stdout, "5. Link to OpenGL and shader program\n");

		// 7. shader object 반환
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		fprintf(stdout, "6. return shader object\n");

		return shaderProgram;
	}
	GLuint CompileShaderStick()
	{
		// shader object ID
		GLuint vertexShader;
		GLuint fragmentShader;

		// 1. shader object 생성
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		fprintf(stdout, "\n1. create shader object\n");

		// 2. shader source code를 shader object에 전달 : shader source code는 각 vertex마다 호출됨(vertex의 개수만큼 호출된다!)
		const GLchar* vertexShaderSource[] =
		{
			// CCW 방식
			"#version 430 core												\n"
			"																\n"
			"layout (location = 0) in vec4 color;							\n"
			"																\n"
			"out vec4 vs_color;												\n"
			"																\n"
			"void main()													\n"
			"{																\n"
			"	const vec4 vertices[6] = vec4[6](							\n"
											// top
			"								vec4(0.01f, 0.f, 0.5f, 1.f), \n"
			"								vec4(-0.01f,  0.f, 0.5f, 1.f), \n"
			"								vec4(-0.01f, -0.8f, 0.5f, 1.f), \n"
											// down
			"								vec4(0.01f, 0.f, 0.5f, 1.f), \n"
			"								vec4(-0.01f, -0.8f, 0.5f, 1.f),\n"
			"								vec4(0.01f, -0.8f, 0.5f, 1.f)  \n"
			"								);								\n"
			"	gl_Position = vertices[gl_VertexID];						\n"
			"	vs_color = color;											\n"
			"}																\n"
		};
		glShaderSource(vertexShader, 1, vertexShaderSource, NULL);
		fprintf(stdout, "2-1. vertex shader source code attach to vertex shader object\n");

		const GLchar* fragmentShaderSource[]
		{
			"#version 430 core						\n"
			"										\n"
			"in vec4 vs_color;						\n"
			"out vec4 color;						\n"
			"										\n"
			"void main()							\n"
			"{										\n"
			"	color = vec4(1.f, 1.f, 0.f, 1.f);					\n"
			"}										\n"
		};
		glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);
		fprintf(stdout, "2-2. fragment shader source code attach to fragment shader object\n");

		// 3. shader sourse code compile
		glCompileShader(vertexShader);
		glCompileShader(fragmentShader);
		fprintf(stdout, "3. shader compile\n");

		// 4. shader program object 생성
		GLuint shaderProgram;
		shaderProgram = glCreateProgram();

		// 5. shader program object에 shader object attach
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		fprintf(stdout, "4. create shader program & attach shader object ");

		// 6. shader program에 attach된 모든 shader object를 link ==> OpenGL과 shader program을 link
		glLinkProgram(shaderProgram);
		fprintf(stdout, "5. Link to OpenGL and shader program\n");

		// 7. shader object 반환
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		fprintf(stdout, "6. return shader object\n");

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
		const GLfloat red[] = { 0.f,		// R
								0.f,		// G
								0.f,		// B
								1.f											// A
		};

		glClearBufferfv(GL_COLOR, 0, red);

		const GLfloat stickCor[] = { 1.f, 0.f, 0.f, 1.f };
		glVertexAttrib4fv(0, stickCor);
		glUseProgram(RenderingProgramStick);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUseProgram(0);

		const GLfloat wingsColor[] = {
								  (float)sin(currentTime) * (float)cos(currentTime) * 0.5f + 0.5,		// B
								  (float)sin(currentTime) * 0.5f + 0.5f,		// R
								  (float)cos(currentTime) * 0.5f + 0.5f,		// G
								  1.f											// A
		};
		glVertexAttrib4fv(0, wingsColor);
		glUseProgram(RenderingProgramWings);
		glDrawArrays(GL_TRIANGLES, 0, 12);
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

/*
			"#version 430 core												\n"
			"																\n"
			"void main()													\n"
			"{																\n"
			"	const vec4 vertices[12] = vec4[12](							\n"
											// top
			"								vec4(0.f, 0.f, 0.5f, 1.f), \n"
			"								vec4(0.f,  0.25f, 0.5f, 1.f), \n"
			"								vec4(-0.25f, 0.25f, 0.5f, 1.f), \n"
											// left
			"								vec4(0.f, 0.f, 0.5f, 1.f), \n"
			"								vec4(-0.25f, 0.f, 0.5f, 1.f),\n"
			"								vec4(-0.25f, -0.25f, 0.5f, 1.f),  \n"
											// down
			"								vec4(0.f, 0.f, 0.5f, 1.f), \n"
			"								vec4(0.f, -0.25f, 0.5f, 1.f),\n"
			"								vec4(0.25f, -0.25f, 0.5f, 1.f),  \n"
											// right
			"								vec4(0.f, 0.f, 0.5f, 1.f), \n"
			"								vec4(0.25f, 0.f, 0.5f, 1.f),\n"
			"								vec4(0.25f, 0.25f, 0.5f, 1.f)  \n"
			"								);								\n"
			"	gl_Position = vertices[gl_VertexID];						\n"
			"}																\n"
*/

/*
											// top
			"								vec4(0.f, 0.f, 0.5f, 1.f), \n"
			"								vec4(0.f,  0.25f, 0.5f, 1.f), \n"
			"								vec4(-0.25f, 0.25f, 0.5f, 1.f), \n"
											// left
			"								vec4(0.f, 0.f, 0.5f, 1.f), \n"
			"								vec4(-0.25f, 0.f, 0.5f, 1.f),\n"
			"								vec4(-0.25f, -0.25f, 0.5f, 1.f),  \n"
											// down
			"								vec4(0.f, 0.f, 0.5f, 1.f), \n"
			"								vec4(0.f, -0.25f, 0.5f, 1.f),\n"
			"								vec4(0.25f, -0.25f, 0.5f, 1.f),  \n"
											// right
			"								vec4(0.f, 0.f, 0.5f, 1.f), \n"
			"								vec4(0.25f, 0.f, 0.5f, 1.f),\n"
			"								vec4(0.25f, 0.25f, 0.5f, 1.f)  \n"
*/