#include <sb7.h>

// shader : graphics pipeline�� ���ư��� ����
// �����Ͷ����� : vertex�� ������ ä��� ����

// CCW or CW :: �� �� �� rendering�� �Ǵ°���??
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

		// 1. shader object ����
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		fprintf(stdout, "\n1. create shader object\n");

		// 2. shader source code�� shader object�� ���� : shader source code�� �� vertex���� ȣ���(vertex�� ������ŭ ȣ��ȴ�!)
		const GLchar* vertexShaderSource[] =
		{
			// CCW ���
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

		// 4. shader program object ����
		GLuint shaderProgram;
		shaderProgram = glCreateProgram();

		// 5. shader program object�� shader object attach
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		fprintf(stdout, "4. create shader program & attach shader object ");

		// 6. shader program�� attach�� ��� shader object�� link ==> OpenGL�� shader program�� link
		glLinkProgram(shaderProgram);
		fprintf(stdout, "5. Link to OpenGL and shader program\n");

		// 7. shader object ��ȯ
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

		// 1. shader object ����
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		fprintf(stdout, "\n1. create shader object\n");

		// 2. shader source code�� shader object�� ���� : shader source code�� �� vertex���� ȣ���(vertex�� ������ŭ ȣ��ȴ�!)
		const GLchar* vertexShaderSource[] =
		{
			// CCW ���
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

		// 4. shader program object ����
		GLuint shaderProgram;
		shaderProgram = glCreateProgram();

		// 5. shader program object�� shader object attach
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		fprintf(stdout, "4. create shader program & attach shader object ");

		// 6. shader program�� attach�� ��� shader object�� link ==> OpenGL�� shader program�� link
		glLinkProgram(shaderProgram);
		fprintf(stdout, "5. Link to OpenGL and shader program\n");

		// 7. shader object ��ȯ
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

		// 1. shader object ����
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		fprintf(stdout, "\n1. create shader object\n");

		// 2. shader source code�� shader object�� ���� : shader source code�� �� vertex���� ȣ���(vertex�� ������ŭ ȣ��ȴ�!)
		const GLchar* vertexShaderSource[] =
		{
			// CCW ���
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

		// 4. shader program object ����
		GLuint shaderProgram;
		shaderProgram = glCreateProgram();

		// 5. shader program object�� shader object attach
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		fprintf(stdout, "4. create shader program & attach shader object ");

		// 6. shader program�� attach�� ��� shader object�� link ==> OpenGL�� shader program�� link
		glLinkProgram(shaderProgram);
		fprintf(stdout, "5. Link to OpenGL and shader program\n");

		// 7. shader object ��ȯ
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

		// 1. shader object ����
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		fprintf(stdout, "\n1. create shader object\n");

		// 2. shader source code�� shader object�� ���� : shader source code�� �� vertex���� ȣ���(vertex�� ������ŭ ȣ��ȴ�!)
		const GLchar* vertexShaderSource[] =
		{
			// CCW ���
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

		// 4. shader program object ����
		GLuint shaderProgram;
		shaderProgram = glCreateProgram();

		// 5. shader program object�� shader object attach
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		fprintf(stdout, "4. create shader program & attach shader object ");

		// 6. shader program�� attach�� ��� shader object�� link ==> OpenGL�� shader program�� link
		glLinkProgram(shaderProgram);
		fprintf(stdout, "5. Link to OpenGL and shader program\n");

		// 7. shader object ��ȯ
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