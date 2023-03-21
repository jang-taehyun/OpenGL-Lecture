#include <sb7.h>

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

		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		TessellationControlShader = glCreateShader(GL_TESS_CONTROL_SHADER);
		TessellationEvaluationShader = glCreateShader(GL_TESS_EVALUATION_SHADER);
		GeometryShader = glCreateShader(GL_GEOMETRY_SHADER);

		const GLchar* vertexShaderSource[] =
		{
			"#version 430 core												\n"
			"																\n"
			"layout (location = 0) in vec4 offset;							\n"
			"layout (location = 1) in vec4 color;							\n"
			"																\n"
			"out VS_OUT {													\n"		// 블록 이름(in 키워드와 매칭)
			"	vec4 color;													\n"		// 블록 내부 멤버 변수(in 키워드와 매칭)
			"} vs_out;														\n"		// 변수 이름(in 키워드와 매칭 안해도 됨)
			"																\n"
			"void main()													\n"
			"{																\n"
			"	const vec4 vertices[3] = vec4[3](							\n"
			"								vec4(0.25f, -0.25f, 0.5f, 1.f), \n"
			"								vec4(-0.25f, 0.25f, 0.5f, 1.f), \n"
			"								vec4(0.25f,  0.25f, 0.5f, 1.f)  \n"
			"								);								\n"
			"	gl_Position = vertices[gl_VertexID] + offset;				\n"
			"	vs_out.color = color;										\n"
			"}																\n"
		};
		glShaderSource(vertexShader, 1, vertexShaderSource, NULL);

		const GLchar* fragmentShaderSource[]
		{
			"#version 430 core						\n"
			"										\n"
			"in vec4 tes_color;						\n"
			"out vec4 fs_color;						\n"
			"										\n"
			"void main()							\n"
			"{										\n"
			"	fs_color = tes_color;				\n"
			"}										\n"
		};
		glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);

		const GLchar* TCSsource[] = {
			"#version 430 core																\n"
			"																				\n"
			"layout (vertices = 3) out;														\n"		// 출력 제어점의 개수 설정
			"in VS_OUT {																	\n"
			"	vec4 color;																	\n"
			"} tcs_in;																		\n"
			"out vec4 tcs_color[];															\n"
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
			"	tcs_color[gl_InvocationID] = tcs_in.color;									\n"
			"}																				\n"
		};
		glShaderSource(TessellationControlShader, 1, TCSsource, NULL);

		const GLchar* TESsource[] = {
			"#version 430 core											\n"
			"															\n"
			"layout (triangles, equal_spacing, cw) in;					\n"
			"in vec4 tcs_color[];										\n"
			"out vec4 tes_color;										\n"
			"															\n"
			"void main()												\n"
			"{															\n"
			"	gl_Position = (gl_TessCoord.x * gl_in[0].gl_Position +  \n"
			"				   gl_TessCoord.y * gl_in[1].gl_Position +  \n"
			"				   gl_TessCoord.z * gl_in[2].gl_Position);	\n"
			"															\n"
			"	tes_color = gl_TessCoord.x * tcs_color[0] + gl_TessCoord.y * tcs_color[1] + gl_TessCoord.z * tcs_color[2];\n"
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

		GLint compileResult;													// shader compile 성공 여부 확인

		glCompileShader(vertexShader);
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compileResult);
		if (compileResult != GL_TRUE)
		{
			fprintf(stdout, "vertex shader compile failed\n");
			GLsizei logLength;
			GLchar logMessage[1024];
			glGetShaderInfoLog(vertexShader, 1024, &logLength, logMessage);
			fprintf(stdout, "%s", logMessage);
		}

		glCompileShader(fragmentShader);
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compileResult);
		if (compileResult != GL_TRUE)
		{
			fprintf(stdout, "fragment Shader compile failed\n");
			GLsizei logLength;
			GLchar logMessage[1024];
			glGetShaderInfoLog(fragmentShader, 1024, &logLength, logMessage);
			fprintf(stdout, "%s", logMessage);
		}

		glCompileShader(TessellationControlShader);
		glGetShaderiv(TessellationControlShader, GL_COMPILE_STATUS, &compileResult);
		if (compileResult != GL_TRUE)
		{
			fprintf(stdout, "TessellationControl Shader compile failed\n");
			GLsizei logLength;
			GLchar logMessage[1024];
			glGetShaderInfoLog(TessellationControlShader, 1024, &logLength, logMessage);
			fprintf(stdout, "%s", logMessage);
		}

		glCompileShader(TessellationEvaluationShader);
		glGetShaderiv(TessellationEvaluationShader, GL_COMPILE_STATUS, &compileResult);
		if (compileResult != GL_TRUE)
		{
			fprintf(stdout, "TessellationEvaluation Shader compile failed\n");
			GLsizei logLength;
			GLchar logMessage[1024];
			glGetShaderInfoLog(TessellationEvaluationShader, 1024, &logLength, logMessage);
			fprintf(stdout, "%s", logMessage);
		}

		glCompileShader(GeometryShader);
		glGetShaderiv(GeometryShader, GL_COMPILE_STATUS, &compileResult);
		if (compileResult != GL_TRUE)
		{
			fprintf(stdout, "Geometry Shader compile failed\n");
			GLsizei logLength;
			GLchar logMessage[1024];
			glGetShaderInfoLog(GeometryShader, 1024, &logLength, logMessage);
			fprintf(stdout, "%s", logMessage);
		}

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
			attrib						// const GLfloat* v : attribute에 넣을 새로운 data를 가르키는 pointer
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
* tessellation을 거치면 vertex attribute들을 직접 보간해줘야 한다!!
* https://codeonwort.tistory.com/298
*/