#include <sb7.h>

// shader : graphics pipeline이 돌아가는 단위
// 레스터라이저 : vertex에 색상을 채우는 역할

// CCW or CW :: 왜 둘 다 rendering이 되는거지??
// -> current : CW

class tmp : public sb7::application
{
private:
	GLuint RenderingProgramUp;		// shader program
	GLuint RenderingProgramDown;
	GLuint RenderingProgramLeft;
	GLuint RenderingProgramRight;
	GLuint VertexArrayObject;		// vertex array object(VAO)
public:
	GLuint CompileShaderUp()
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
			"void main()													\n"
			"{																\n"
			"	const vec4 vertices[3] = vec4[3](							\n"
											// top
			"								vec4(0.f, 0.f, 0.5f, 1.f), \n"
			"								vec4(0.f,  0.25f, 0.5f, 1.f), \n"
			"								vec4(-0.25f, 0.25f, 0.5f, 1.f) \n"
			"								);								\n"
			"	gl_Position = vertices[gl_VertexID];						\n"
			"}																\n"

		};
		glShaderSource(vertexShader, 1, vertexShaderSource, NULL);
		fprintf(stdout, "2-1. vertex shader source code attach to vertex shader object\n");

		const GLchar* fragmentShaderSource[]
		{
			"#version 430 core						\n"
			"										\n"
			"out vec4 color;						\n"
			"										\n"
			"void main()							\n"
			"{										\n"
			"	color = vec4(1.f, 1.f, 1.f, 1.f);	\n"
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
	GLuint CompileShaderDown()
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
			"void main()													\n"
			"{																\n"
			"	const vec4 vertices[3] = vec4[3](							\n"
			// down
"								vec4(0.f, 0.f, 0.5f, 1.f), \n"
"								vec4(0.f, -0.25f, 0.5f, 1.f),\n"
"								vec4(0.25f, -0.25f, 0.5f, 1.f)  \n"
"								);								\n"
"	gl_Position = vertices[gl_VertexID];						\n"
"}																\n"

		};
		glShaderSource(vertexShader, 1, vertexShaderSource, NULL);
		fprintf(stdout, "2-1. vertex shader source code attach to vertex shader object\n");

		const GLchar* fragmentShaderSource[]
		{
			"#version 430 core						\n"
			"										\n"
			"out vec4 color;						\n"
			"										\n"
			"void main()							\n"
			"{										\n"
			"	color = vec4(1.f, 0.f, 0.f, 1.f);	\n"
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
	GLuint CompileShaderRight()
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
			"void main()													\n"
			"{																\n"
			"	const vec4 vertices[3] = vec4[3](							\n"
			"								vec4(0.f, 0.f, 0.5f, 1.f), \n"
			"								vec4(0.25f, 0.f, 0.5f, 1.f),\n"
			"								vec4(0.25f, 0.25f, 0.5f, 1.f)  \n"
"								);								\n"
"	gl_Position = vertices[gl_VertexID];						\n"
"}																\n"

		};
		glShaderSource(vertexShader, 1, vertexShaderSource, NULL);
		fprintf(stdout, "2-1. vertex shader source code attach to vertex shader object\n");

		const GLchar* fragmentShaderSource[]
		{
			"#version 430 core						\n"
			"										\n"
			"out vec4 color;						\n"
			"										\n"
			"void main()							\n"
			"{										\n"
			"	color = vec4(0.f, 1.f, 0.f, 1.f);	\n"
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
	GLuint CompileShaderLeft()
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
			"void main()													\n"
			"{																\n"
			"	const vec4 vertices[3] = vec4[3](							\n"
			// left
"								vec4(0.f, 0.f, 0.5f, 1.f), \n"
"								vec4(-0.25f, 0.f, 0.5f, 1.f),\n"
"								vec4(-0.25f, -0.25f, 0.5f, 1.f)  \n"
"								);								\n"
"	gl_Position = vertices[gl_VertexID];						\n"
"}																\n"

		};
		glShaderSource(vertexShader, 1, vertexShaderSource, NULL);
		fprintf(stdout, "2-1. vertex shader source code attach to vertex shader object\n");

		const GLchar* fragmentShaderSource[]
		{
			"#version 430 core						\n"
			"										\n"
			"out vec4 color;						\n"
			"										\n"
			"void main()							\n"
			"{										\n"
			"	color = vec4(0.f, 0.f, 1.f, 1.f);	\n"
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
		RenderingProgramUp = CompileShaderUp();
		RenderingProgramDown = CompileShaderDown();
		RenderingProgramLeft = CompileShaderLeft();
		RenderingProgramRight = CompileShaderRight();

		glGenVertexArrays(1, &VertexArrayObject);
		glBindVertexArray(VertexArrayObject);
	}

	// rendering loop
	virtual void render(double currentTime)
	{
		const GLfloat red[] = { (float)sin(currentTime) * 0.5f + 0.5f,		// R
								(float)cos(currentTime) * 0.5f + 0.5f,		// G
								(float)sin(currentTime) * 0.5f + 0.5f,		// B
								1.f											// A
		};

		glClearBufferfv(GL_COLOR, 0, red);

		glUseProgram(RenderingProgramUp);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glUseProgram(0);

		glUseProgram(RenderingProgramDown);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glUseProgram(0);

		glUseProgram(RenderingProgramLeft);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glUseProgram(0);

		glUseProgram(RenderingProgramRight);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glUseProgram(0);
	}

	virtual void shutdown()
	{
		glDeleteVertexArrays(1, &VertexArrayObject);
		glDeleteProgram(RenderingProgramUp);
		glDeleteProgram(RenderingProgramDown);
		glDeleteProgram(RenderingProgramLeft);
		glDeleteProgram(RenderingProgramRight);
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