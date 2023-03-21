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
			"out VS_OUT {													\n"		// ��� �̸�(in Ű����� ��Ī)
			"	vec4 color;													\n"		// ��� ���� ��� ����(in Ű����� ��Ī)
			"} vs_out;														\n"		// ���� �̸�(in Ű����� ��Ī ���ص� ��)
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
			"layout (vertices = 3) out;														\n"		// ��� �������� ���� ����
			"in VS_OUT {																	\n"
			"	vec4 color;																	\n"
			"} tcs_in;																		\n"
			"out vec4 tcs_color[];															\n"
			"																				\n"
			"void main()																	\n"
			"{																				\n"
			"	if(gl_InvocationID == 0) {													\n"		// ������ ȣ��(gl__VertexID�� ����)
																									// ����ȭ ���� ����
			"		gl_TessLevelInner[0] = 5.f;												\n"		// Inner level ���� : polygon ���� ����ȭ ����
			"		gl_TessLevelOuter[0] = 5.f;												\n"		// outer level ���� : polygon�� �� ���� ����ȭ ����
			"		gl_TessLevelOuter[1] = 5.f;												\n"
			"		gl_TessLevelOuter[2] = 5.f;												\n"
			"	}																			\n"
			"	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;	\n"		// ���������� �׷����� ���� �ѹ��� ó��
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

		GLint compileResult;													// shader compile ���� ���� Ȯ��

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

		// vertex attribute�� 0��° index�� ���� ����(�ﰢ�� offset)
		const GLfloat attrib[] = {
						(float)sin(currentTime) * 0.5f,		// x
						(float)cos(currentTime) * 0.5f,		// y
						0.f,								// z
						0.f									// homogeneous ���
		};
		glVertexAttrib4fv(0,			// GLuint index : vertex shader���� attribute�� �����ϱ� ���� index
			attrib						// const GLfloat* v : attribute�� ���� ���ο� data�� ����Ű�� pointer
		);
		// vertex attribute�� 1��° index�� ���� ����(�ﰢ�� color)
		const GLfloat color[] = {
						1.f,								// R
						0.f,								// G
						0.f,								// B
						1.f									// A
		};
		glVertexAttrib4fv(1, color);

		// tessellation ��� ��
		// glDrawArrays(GL_TRIANGLES, 0, 3);

		// point ũ�� ����
		glPointSize(5.f);

		// patch�� �������� ���� ����
		glPatchParameteri(GL_PATCH_VERTICES, 3);

		// tessellation ����� ���� ���� wireframe mode�� ����
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		// tessellation ���
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
* tessellation�� ��ġ�� vertex attribute���� ���� ��������� �Ѵ�!!
* https://codeonwort.tistory.com/298
*/