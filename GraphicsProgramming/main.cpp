#include <sb7.h>

// shader : graphics pipeline이 돌아가는 단위
// 레스터라이저 : vertex에 색상을 채우는 역할

class tmp : public sb7::application
{
private:
	GLuint RenderingProgram;		// shader program
	GLuint VertexArrayObject;		// vertex array object(VAO) : vertex fetch stage를 나타내는 object
									// -> OpenGL pipeline의 input과 관련된 모든 상태를 유지
public:
	GLuint CompileShader()
	{
		// shader object ID
		GLuint vertexShader;
		GLuint fragmentShader;

		// 1. shader object 생성
		vertexShader = glCreateShader(GL_VERTEX_SHADER);		// shader type을 명시해 어떤 shader object를 생성할 지 명시
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// 2. shader source code를 shader object에 전달 : shader source code는 각 vertex마다 호출됨(vertex의 개수만큼 호출된다!)
		const GLchar* vertexShaderSource[] =
		{
			// Point
			//// GLSL version && profile
			//"#version 430 core							\n"
			//"											\n"
			//"void main()								\n"
			//"{											\n"
			//"	gl_Position = vec4(0.f, 0.f, 0.5f, 1.f);\n"		// vertex의 output 지정
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
			"	gl_Position = vertices[gl_VertexID];						\n"		// gl_VertexID : glDrawArrays()의 first부터 시작하여 count만큼의 vertex의 index를 1씩 증가하는 OpenGL 내장 변수
																					// -> gl_VertexID를 통해 현재 vertex shader가 몇 번째 vertex에 적용하고 있는지 알 수 있다.
																					// -> vertex shader에서만 사용 가능
			"}																\n"

		};
		glShaderSource(vertexShader,			// shader object
					   1,						// source code의 개수(상수형 string의 line의 개수)
					   vertexShaderSource,		// source code
					   NULL						// source code의 문자 개수 : NULL 입력 시 자동으로 문자의 개수 파악
		);

		const GLchar* fragmentShaderSource[]
		{
			"#version 430 core						\n"
			"										\n"
			"out vec4 color;						\n"	// out 키워드를 이용해 color 변수를 output 변수로 사용
			"										\n"
			"void main()							\n"
			"{										\n"
			"	color = vec4(0.f, 0.f, 1.f, 1.f);	\n"
			"}										\n"
		};
		glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);

		// 3. shader sourse code compile
		glCompileShader(vertexShader);			// shader object 전달
		glCompileShader(fragmentShader);

		// 4. shader program object 생성
		GLuint shaderProgram;
		shaderProgram = glCreateProgram();

		// 5. shader program object에 shader object attach
		glAttachShader(shaderProgram,					// shader program object
					  vertexShader);					// shader object
		glAttachShader(shaderProgram, fragmentShader);

		// 6. shader program에 attach된 모든 shader object를 link ==> OpenGL과 shader program을 link
		glLinkProgram(shaderProgram);

		// 7. shader object 반환
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		return shaderProgram;
	}


	// application initialize
	virtual void startup()
	{
		// shader program 생성
		RenderingProgram = CompileShader();

		// Vertex Array Object(VAO) 생성
		glGenVertexArrays(1, &VertexArrayObject);
		// VAO를 pipeline에 bind(연결)
		glBindVertexArray(VertexArrayObject);
	}

	// main loop : rendering code
	// currentTime : Application이 시작한 이후의 경과 시간(second 단위)
	// -> animation을 위한 값으로 활용 가능
	virtual void render(double currentTime)
	{
		// 추상화를 위해 0~1 사이의 value 사용
		const GLfloat red[] = { (float)sin(currentTime) * 0.5f + 0.5f,		// R
								(float)cos(currentTime) * 0.5f + 0.5f,		// G
								(float)sin(currentTime) * 0.5f + 0.5f,		// B
								1.f											// A : 
							  };

		// 화면에 뿌려질 Framebuffer를 특정 RGB-A 값으로 지우기
		glClearBufferfv(GL_COLOR,			// GLenum buffer : 표적 FrameBuffer의 type
											// -> 다양한 키워드 존재 && glDocs에 사용할 수 있는 키워드 존재
						0,					// GLint drawbuffer : 출력 buffer가 여러 개 일때 지정(0기반 index 사용)
											// -> 동일한 buffer가 여러 개 있을 때 사용 && default(0)인 경우 default draw framebuffer 또는 framebufffer object를 가르킨다.
						red);				// const GLfloat* value : 지울 color 값

		// 사용할 shader program 명시
		glUseProgram(RenderingProgram);

		//glPointSize(40.f);					// -> Point의 크기 조절

		//// draw
		//glDrawArrays(GL_POINTS,			// GLenum mode : primitive 명시
		//			 0,						// GLint first : 시작 index
		//			 1						// GLsizei count : index의 개수
		//);
		// -> line 또는 triangle에서 2개 이상의 vertex가 같은 위치에 있다면 primitive가 취소된다.

		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	virtual void shutdown()
	{
		// VAO 반환
		glDeleteVertexArrays(1, &VertexArrayObject);
		// shader progrma 반환
		glDeleteProgram(RenderingProgram);
	}
};

DECLARE_MAIN(tmp)

/**
* 모든 OpenGL function은 gl로 시작(접두사 gl)
* 'gl_'로 시작하는 변수는 OpenGL에 내장된 variable, 다른 스테이지의 shader들과 연결된다.
* 
* fv : 인자 타입을 접미사로 줄여쓰는 convention 중 하나(규칙, 약속)
* -> f : 부동소수점, v : 벡터(배열)
* 
* shader source code는 shader object에 전달되어 OpenGL에 내장된 compiler에서 compile된다.
* -> shader source code의 흐름 : CPU side -> GPU side(shader object) -> GPU side(OpenGL compiler)
* 
* shader program object는 여러 shader shader object들과 연결되어 있다.
* 
* vertex shader : vertex의 position을 처리
* fragment shader : 그려질 2D image의 pixel 값 지정
*/