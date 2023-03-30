#include <sb7.h>
#include <shader.h>

class tmp : public sb7::application
{
private:
	GLuint RenderingProgram;		// shader program
	GLuint VertexArrayObject;		// vertex array object(VAO)
public:
	GLuint CompileShader()
	{
		GLuint vertexShader;
		GLuint fragmentShader;
		GLuint TessellationControlShader;
		GLuint TessellationEvaluationShader;
		GLuint GeometryShader;

		// -> glsl 파일로 로드하는 방법
		// --> shader 컴파일까지 해준다!
		// vertexShader = sb7::shader::load("pinWheel_vs.glsl", GL_VERTEX_SHADER);

		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		TessellationControlShader = glCreateShader(GL_TESS_CONTROL_SHADER);
		TessellationEvaluationShader = glCreateShader(GL_TESS_EVALUATION_SHADER);
		GeometryShader = glCreateShader(GL_GEOMETRY_SHADER);

		const GLchar* vertexShaderSource[] =
		{
			//"#version 430 core												\n"
			//"																\n"
			//"layout (location = 0) in vec4 offset;							\n"		// layout 키워드 : vertex attribute를 받을 때 사용
			//																		// location 다음의 숫자와 vertex attribute의 index와 같아야 한다.
			//"layout (location = 1) in vec4 color;							\n"
			//"																\n"
			//"out vec4 VertexShaderColor;									\n"
			//"																\n"
			//"void main()													\n"
			//"{																\n"
			//"	const vec4 vertices[3] = vec4[3](							\n"
			//"								vec4(0.25f, -0.25f, 0.5f, 1.f), \n"
			//"								vec4(-0.25f, 0.25f, 0.5f, 1.f), \n"
			//"								vec4(0.25f,  0.25f, 0.5f, 1.f)  \n"
			//"								);								\n"
			//"	gl_Position = vertices[gl_VertexID] + offset;				\n"
			//"	VertexShaderColor = color;									\n"
			//"}																\n"

			// geometry shader 까지 먹인 vertex shader
			"#version 430 core												\n"
			"																\n"
			"layout (location = 0) in vec4 offset;							\n"
			"layout (location = 1) in vec4 color;							\n"
			"																\n"
			//"out VS_OUT {													\n"		// 블록 이름(in 키워드와 매칭)
			//"	vec4 color;													\n"		// 블록 내부 멤버 변수(in 키워드와 매칭)
			//"} vs_out;														\n"		// 변수 이름(in 키워드와 매칭 안해도 됨)
			"out vec4 vs_color;												\n"
			"																\n"
			"void main()													\n"
			"{																\n"
			"	const vec4 vertices[3] = vec4[3](							\n"
			"								vec4(0.25f, -0.25f, 0.5f, 1.f), \n"
			"								vec4(-0.25f, 0.25f, 0.5f, 1.f), \n"
			"								vec4(0.25f,  0.25f, 0.5f, 1.f)  \n"
			"								);								\n"
			"	gl_Position = vertices[gl_VertexID] + offset;				\n"
			//"	vs_out.color = color;										\n"
			"	vs_color = color;											\n"
			"}																\n"

			// gl_FragCoord 사용 예시 vertex shader
			//"#version 430 core												\n"
			//"																\n"
			//"layout (location = 0) in vec4 offset;							\n"
			//"																\n"
			//"void main()													\n"
			//"{																\n"
			//"	const vec4 vertices[3] = vec4[3](							\n"
			//"								vec4(0.25f, -0.25f, 0.5f, 1.f), \n"
			//"								vec4(-0.25f, 0.25f, 0.5f, 1.f), \n"
			//"								vec4(0.25f,  0.25f, 0.5f, 1.f)  \n"
			//"								);								\n"
			//"	gl_Position = vertices[gl_VertexID] + offset;				\n"
			//"}																\n"

			// 입력 보간 vertex shader
			//"#version 430 core												\n"
			//"																\n"
			//"layout (location = 0) in vec4 offset;							\n"
			//"																\n"
			//"out vec4 vs_color;												\n"
			//"																\n"
			//"void main()													\n"
			//"{																\n"
			//"	const vec4 vertices[3] = vec4[3](							\n"
			//"								vec4(0.25f, -0.25f, 0.5f, 1.f), \n"
			//"								vec4(-0.25f, 0.25f, 0.5f, 1.f), \n"
			//"								vec4(0.25f,  0.25f, 0.5f, 1.f)  \n"
			//"								);								\n"
			//"	gl_Position = vertices[gl_VertexID] + offset;				\n"
			//"	vec4 colors[3] = vec4[3](vec4(1.f, 0.f, 0.f, 1.f),			\n"
			//"							 vec4(0.f, 1.f, 0.f, 1.f),			\n"
			//"							 vec4(0.f, 0.f, 1.f, 1.f));			\n"
			//"	vs_color = colors[gl_VertexID];								\n"
			//"}																\n"

		};
		glShaderSource(vertexShader, 1, vertexShaderSource, NULL);

		const GLchar* fragmentShaderSource[]
		{
			//"#version 430 core						\n"
			//"										\n"
			//"in vec4 VertexShaderColor;				\n"
			//"out vec4 color;						\n"
			//"										\n"
			//"void main()							\n"
			//"{										\n"
			//"	color = VertexShaderColor;			\n"
			//"}										\n"

			// geometry shader 까지 먹인 fragment shader
			"#version 430 core						\n"
			"										\n"
			//"in VS_OUT {							\n"			// 블록 이름(in 키워드와 매칭)
			//"	vec4 color;							\n"			// 블록 내부 멤버 변수(in 키워드와 매칭)
			//"} fs_in;								\n"			// 변수 이름(in 키워드와 매칭 안해도 됨)
			"in vec4 vs_color;						\n"
			"out vec4 color;						\n"
			"										\n"
			"void main()							\n"
			"{										\n"
			//"	color = fs_in.color;				\n"
			"	color = vs_color;					\n"
			"}										\n"

			// gl_FrgaCoord 사용 예시 fragment shader
			//"#version 430 core															\n"
			//"																			\n"
			//"out vec4 color;															\n"
			//"																			\n"
			//"void main()																\n"
			//"{																			\n"
			//"	color = vec4(sin(gl_FragCoord.x * 0.25) * 0.5 + 0.5,					\n"
			//"				 cos(gl_FragCoord.y * 0.25) * 0.5 + 0.5,					\n"
			//"				 sin(gl_FragCoord.x * 0.25) * cos(gl_FragCoord.y * 0.25),	\n"
			//"				 1.f);														\n"
			//"}																			\n"

			//// 입력 보간 fragment shader
			//"#version 430 core						\n"
			//"										\n"
			//"in vec4 vs_color;						\n"
			//"out vec4 color;						\n"
			//"										\n"
			//"void main()							\n"
			//"{										\n"
			//"	color = vs_color;					\n"
			//"}										\n"

		};
		glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);

		const GLchar* TCSsource[] = {
			"#version 430 core																\n"
			"																				\n"
			"layout (vertices = 3) out;														\n"		// 출력 제어점의 개수 설정
			"																				\n"
			"void main()																	\n"
			"{																				\n"
			"	if(gl_InvocationID == 0) {													\n"		// 제어점 호출(gl__VertexID와 동일)
																									// 세분화 정보 설정
			"		gl_TessLevelInner[0] = 5.f;												\n"		// Inner level 설정 : polygon 내부 세분화 설정
			"		gl_TessLevelOuter[0] = 5.f;												\n"		// outer level 설정 : polygon의 각 변의 세분화 설정
			"		gl_TessLevelOuter[1] = 5.f;												\n"
			"		gl_TessLevelOuter[2] = 5.f;												\n"
			"	}																			\n"
			"	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;	\n"		// 제어점들을 그룹으로 묶어 한번에 처리
			"}																				\n"
		};
		glShaderSource(TessellationControlShader, 1, TCSsource, NULL);

		const GLchar* TESsource[] = {
			"#version 430 core											\n"
			"															\n"
			"layout (triangles, equal_spacing, cw) in;					\n"
			"															\n"
			"void main()												\n"
			"{															\n"
			"	gl_Position = (gl_TessCoord.x * gl_in[0].gl_Position +  \n"
			"				   gl_TessCoord.y * gl_in[1].gl_Position +  \n"
			"				   gl_TessCoord.z * gl_in[2].gl_Position);	\n"
			"}															\n"
		};
		glShaderSource(TessellationEvaluationShader, 1, TESsource, NULL);

		const GLchar* GeometryShaderSource[] = {
			"#version 430 core											\n"
			"															\n"
			"layout (triangles) in;										\n"
			"layout (points, max_vertices = 6) out;						\n"
			"															\n"
			"void main()												\n"
			"{															\n"
			"	int index;												\n"
			"	for(index=0; index<gl_in.length(); index++)				\n"
			"	{														\n"
			"		gl_Position = gl_in[index].gl_Position;				\n"
			"		EmitVertex();										\n"
			"		gl_Position[0] *= -1;								\n"
			"		gl_Position[1] *= -1;								\n"
			"		EmitVertex();										\n"
			"	}														\n"
			"}															\n"
		};
		glShaderSource(GeometryShader, 1, GeometryShaderSource, NULL);

		glCompileShader(vertexShader);
		glCompileShader(fragmentShader);
		glCompileShader(TessellationControlShader);
		glCompileShader(TessellationEvaluationShader);
		glCompileShader(GeometryShader);

		GLuint shaderProgram;
		shaderProgram = glCreateProgram();

		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glAttachShader(shaderProgram, TessellationControlShader);
		glAttachShader(shaderProgram, TessellationEvaluationShader);
		glAttachShader(shaderProgram, GeometryShader);

		glLinkProgram(shaderProgram);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glDeleteShader(TessellationControlShader);
		glDeleteShader(TessellationEvaluationShader);
		glDeleteShader(GeometryShader);

		return shaderProgram;
	}


	// application initialize
	virtual void startup()
	{
		RenderingProgram = CompileShader();

		glGenVertexArrays(1, &VertexArrayObject);
		glBindVertexArray(VertexArrayObject);
	}

	// rendering
	virtual void render(double currentTime)
	{
		const GLfloat ClearColor[] = { (float)sin(currentTime) * 0.5f + 0.5f,		// R
									   (float)cos(currentTime) * 0.5f + 0.5f,		// G
									   (float)sin(currentTime) * 0.5f + 0.5f,		// B
									   1.f											// A
									 };

		glClearBufferfv(GL_COLOR, 0, ClearColor);

		glUseProgram(RenderingProgram);

		// vertex attribute의 0번째 index의 값을 갱신(삼각형 offset)
		const GLfloat attrib[] = {
						(float)sin(currentTime) * 0.5f,		// x
						(float)cos(currentTime) * 0.5f,		// y
						0.f,								// z
						0.f									// homogeneous 상수
		};
		glVertexAttrib4fv(0,			// GLuint index : vertex shader에서 attribute을 참조하기 위한 index
						  attrib		// const GLfloat* v : attribute에 넣을 새로운 data를 가르키는 pointer
		);
		// vertex attribute의 1번째 index의 값을 갱신(삼각형 color)
		const GLfloat color[] = {
						1.f,								// R
						0.f,								// G
						0.f,								// B
						1.f									// A
		};
		glVertexAttrib4fv(1, color);

		// tessellation 사용 전
		// glDrawArrays(GL_TRIANGLES, 0, 3);

		// point 크기 설정
		glPointSize(5.f);

		// patch당 제어점의 개수 설정
		glPatchParameteri(GL_PATCH_VERTICES, 3);

		// tessellation 결과를 보기 위해 wireframe mode로 변경
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		// tessellation 사용
		glDrawArrays(GL_PATCHES, 0, 3);

		glUseProgram(0);
	}

	virtual void shutdown()
	{
		glDeleteVertexArrays(1, &VertexArrayObject);
		glDeleteProgram(RenderingProgram);
	}
};

DECLARE_MAIN(tmp)

/*
* 각 pipeline의 shader에 data를 전달하는 방법 : in, out 키워드 사용
  - 파이프라인에서 전역 변수를 선언하고 사용하는 방식
  - in 키워드 : 이전 stage에서 계산한 값을 현재 stage로 받아오는 키워드
  - out 키워드 : 현재 stage에서 계산한 값을 다음 stage로 전달하는 키워드
  - 주의 사항
    - 이전 stage에서 out 키워드를 이용해 전달한 변수의 이름과 data type은,
	  다음 stage의 in 키워드를 이용해 받는 변수의 이름과 data tyep이 같아야 한다!
	  ex) vertex shader에서 vec4 B에 out 키워드를 선언했다면, fragment shader에서 in 키워드에 들어가는 변수는 vec4 B로 선언해야 한다!

* interface block : 여러 변수를 하나의 interface block으로 그룹화 가능
  - C언어의 structure와 유사 : struct 대신 in 또는 out 키워드 사용
  - interface block은 '블록 이름'(변수 이름 아님)을 사용해 매칭한다!
  - in 또는 out 키워드로 넘길 때 블록 이름과 내부 멤버 변수의 이름과 내부 멤버 변수의 data type은 같아야 한다.
    -> 블록 뒤에 변수 이름은 달라져도 됨!

* tessellation : polygon을 여러 개의 polygon으로 쪼개는 작업
  - patch(고차 primitive)를 더 작고, 단순한 여러 개의 렌더링 가능한 primitive로 분할하는 작업
  - low-polygon의 3D model을 tessellation을 이용해 매끄러운 평면으로 만들 수 있다!(Subdivision 작업)

* Tessellation control shader : tessellation level(분할 정도) 결정 & tessellation evaluation shader에 보낼 data 생성
  - in 또는 out 키워드를 이용해 data 전달
*/