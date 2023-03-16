#include <sb7.h>

// shader : graphics pipeline�� ���ư��� ����
// �����Ͷ����� : vertex�� ������ ä��� ����

class tmp : public sb7::application
{
private:
	GLuint RenderingProgram;		// shader program
	GLuint VertexArrayObject;		// vertex array object(VAO) : vertex fetch stage�� ��Ÿ���� object
									// -> OpenGL pipeline�� input�� ���õ� ��� ���¸� ����
public:
	GLuint CompileShader()
	{
		// shader object ID
		GLuint vertexShader;
		GLuint fragmentShader;

		// 1. shader object ����
		vertexShader = glCreateShader(GL_VERTEX_SHADER);		// shader type�� ����� � shader object�� ������ �� ���
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// 2. shader source code�� shader object�� ���� : shader source code�� �� vertex���� ȣ���(vertex�� ������ŭ ȣ��ȴ�!)
		const GLchar* vertexShaderSource[] =
		{
			// Point
			//// GLSL version && profile
			//"#version 430 core							\n"
			//"											\n"
			//"void main()								\n"
			//"{											\n"
			//"	gl_Position = vec4(0.f, 0.f, 0.5f, 1.f);\n"		// vertex�� output ����
			//"}											\n"
			
			// Triangle
			"#version 430 core												\n"
			"																\n"
			"void main()													\n"
			"{																\n"
			"	const vec4 vertices[3] = vec4[3](							\n"
			"								vec4(0.25f, -0.25f, 0.5f, 1.f), \n"
			"								vec4(-0.25f, 0.25f, 0.5f, 1.f), \n"
			"								vec4(0.25f,  0.25f, 0.5f, 1.f)  \n"
			"								);								\n"
			"	gl_Position = vertices[gl_VertexID];						\n"		// gl_VertexID : glDrawArrays()�� first���� �����Ͽ� count��ŭ�� vertex�� index�� 1�� �����ϴ� OpenGL ���� ����
																					// -> gl_VertexID�� ���� ���� vertex shader�� �� ��° vertex�� �����ϰ� �ִ��� �� �� �ִ�.
																					// -> vertex shader������ ��� ����
			"}																\n"

		};
		glShaderSource(vertexShader,			// shader object
					   1,						// source code�� ����(����� string�� line�� ����)
					   vertexShaderSource,		// source code
					   NULL						// source code�� ���� ���� : NULL �Է� �� �ڵ����� ������ ���� �ľ�
		);

		const GLchar* fragmentShaderSource[]
		{
			"#version 430 core						\n"
			"										\n"
			"out vec4 color;						\n"	// out Ű���带 �̿��� color ������ output ������ ���
			"										\n"
			"void main()							\n"
			"{										\n"
			"	color = vec4(0.f, 0.f, 1.f, 1.f);	\n"
			"}										\n"
		};
		glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);

		// 3. shader sourse code compile
		glCompileShader(vertexShader);			// shader object ����
		glCompileShader(fragmentShader);

		// 4. shader program object ����
		GLuint shaderProgram;
		shaderProgram = glCreateProgram();

		// 5. shader program object�� shader object attach
		glAttachShader(shaderProgram,					// shader program object
					  vertexShader);					// shader object
		glAttachShader(shaderProgram, fragmentShader);

		// 6. shader program�� attach�� ��� shader object�� link ==> OpenGL�� shader program�� link
		glLinkProgram(shaderProgram);

		// 7. shader object ��ȯ
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		return shaderProgram;
	}


	// application initialize
	virtual void startup()
	{
		// shader program ����
		RenderingProgram = CompileShader();

		// Vertex Array Object(VAO) ����
		glGenVertexArrays(1, &VertexArrayObject);
		// VAO�� pipeline�� bind(����)
		glBindVertexArray(VertexArrayObject);
	}

	// main loop : rendering code
	// currentTime : Application�� ������ ������ ��� �ð�(second ����)
	// -> animation�� ���� ������ Ȱ�� ����
	virtual void render(double currentTime)
	{
		// �߻�ȭ�� ���� 0~1 ������ value ���
		const GLfloat red[] = { (float)sin(currentTime) * 0.5f + 0.5f,		// R
								(float)cos(currentTime) * 0.5f + 0.5f,		// G
								(float)sin(currentTime) * 0.5f + 0.5f,		// B
								1.f											// A : 
							  };

		// ȭ�鿡 �ѷ��� Framebuffer�� Ư�� RGB-A ������ �����
		glClearBufferfv(GL_COLOR,			// GLenum buffer : ǥ�� FrameBuffer�� type
											// -> �پ��� Ű���� ���� && glDocs�� ����� �� �ִ� Ű���� ����
						0,					// GLint drawbuffer : ��� buffer�� ���� �� �϶� ����(0��� index ���)
											// -> ������ buffer�� ���� �� ���� �� ��� && default(0)�� ��� default draw framebuffer �Ǵ� framebufffer object�� ����Ų��.
						red);				// const GLfloat* value : ���� color ��

		// ����� shader program ���
		glUseProgram(RenderingProgram);

		//glPointSize(40.f);					// -> Point�� ũ�� ����

		//// draw
		//glDrawArrays(GL_POINTS,			// GLenum mode : primitive ���
		//			 0,						// GLint first : ���� index
		//			 1						// GLsizei count : index�� ����
		//);
		// -> line �Ǵ� triangle���� 2�� �̻��� vertex�� ���� ��ġ�� �ִٸ� primitive�� ��ҵȴ�.

		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	virtual void shutdown()
	{
		// VAO ��ȯ
		glDeleteVertexArrays(1, &VertexArrayObject);
		// shader progrma ��ȯ
		glDeleteProgram(RenderingProgram);
	}
};

DECLARE_MAIN(tmp)

/**
* ��� OpenGL function�� gl�� ����(���λ� gl)
* 'gl_'�� �����ϴ� ������ OpenGL�� ����� variable, �ٸ� ���������� shader��� ����ȴ�.
* 
* fv : ���� Ÿ���� ���̻�� �ٿ����� convention �� �ϳ�(��Ģ, ���)
* -> f : �ε��Ҽ���, v : ����(�迭)
* 
* shader source code�� shader object�� ���޵Ǿ� OpenGL�� ����� compiler���� compile�ȴ�.
* -> shader source code�� �帧 : CPU side -> GPU side(shader object) -> GPU side(OpenGL compiler)
* 
* shader program object�� ���� shader shader object��� ����Ǿ� �ִ�.
* 
* vertex shader : vertex�� position�� ó��
* fragment shader : �׷��� 2D image�� pixel �� ����
*/