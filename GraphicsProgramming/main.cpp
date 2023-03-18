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

		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		TessellationControlShader = glCreateShader(GL_TESS_CONTROL_SHADER);
		TessellationEvaluationShader = glCreateShader(GL_TESS_EVALUATION_SHADER);

		const GLchar* vertexShaderSource[] =
		{
			//"#version 430 core												\n"
			//"																\n"
			//"layout (location = 0) in vec4 offset;							\n"		// layout Ű���� : vertex attribute�� ���� �� ���
			//																		// location ������ ���ڿ� vertex attribute�� index�� ���ƾ� �Ѵ�.
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
			//"#version 430 core						\n"
			//"										\n"
			//"in vec4 VertexShaderColor;				\n"
			//"out vec4 color;						\n"
			//"										\n"
			//"void main()							\n"
			//"{										\n"
			//"	color = VertexShaderColor;			\n"
			//"}										\n"

			"#version 430 core						\n"
			"										\n"
			"in	VS_OUT {							\n"			// ��� �̸�(in Ű����� ��Ī)
			"	vec4 color;							\n"			// ��� ���� ��� ����(in Ű����� ��Ī)
			"} fs_in;								\n"			// ���� �̸�(in Ű����� ��Ī ���ص� ��)
			"out vec4 color;						\n"
			"										\n"
			"void main()							\n"
			"{										\n"
			"	color = fs_in.color;				\n"
			"}										\n"
		};
		glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);

		const GLchar* TCSsource[] = {
			"#version 430 core																\n"
			"																				\n"
			"layout (vertices = 3) out;														\n"		// ��� �������� ���� ����
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

		glCompileShader(vertexShader);
		glCompileShader(fragmentShader);
		glCompileShader(TessellationControlShader);
		glCompileShader(TessellationEvaluationShader);

		GLuint shaderProgram;
		shaderProgram = glCreateProgram();

		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glAttachShader(shaderProgram, TessellationControlShader);
		glAttachShader(shaderProgram, TessellationEvaluationShader);

		glLinkProgram(shaderProgram);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glDeleteShader(TessellationControlShader);
		glDeleteShader(TessellationEvaluationShader);

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
		GLfloat attrib[] = {
						(float)sin(currentTime) * 0.5f,		// x
						(float)cos(currentTime) * 0.5f,		// y
						0.f,								// z
						0.f									// homogeneous ���
		};
		glVertexAttrib4fv(0,			// GLuint index : vertex shader���� attribute�� �����ϱ� ���� index
						  attrib		// const GLfloat* v : attribute�� ���� ���ο� data�� ����Ű�� pointer
		);
		// vertex attribute�� 1��° index�� ���� ����(�ﰢ�� color)
		GLfloat color[] = {
						1.f,								// R
						0.f,								// G
						0.f,								// B
						0.f									// A
		};
		glVertexAttrib4fv(1, color);

		// tessellation ��� ��
		// glDrawArrays(GL_TRIANGLES, 0, 3);

		// tessellation ��� ��
		glDrawArrays(GL_PATCHES, 0, 3);

		// tessellation ����� ���� ���� wireframe mode�� ����
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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
* �� pipeline�� shader�� data�� �����ϴ� ��� : in, out Ű���� ���
  - ���������ο��� ���� ������ �����ϰ� ����ϴ� ���
  - in Ű���� : ���� stage���� ����� ���� ���� stage�� �޾ƿ��� Ű����
  - out Ű���� : ���� stage���� ����� ���� ���� stage�� �����ϴ� Ű����
  - ���� ����
    - ���� stage���� out Ű���带 �̿��� ������ ������ �̸��� data type��,
	  ���� stage�� in Ű���带 �̿��� �޴� ������ �̸��� data tyep�� ���ƾ� �Ѵ�!
	  ex) vertex shader���� vec4 B�� out Ű���带 �����ߴٸ�, fragment shader���� in Ű���忡 ���� ������ vec4 B�� �����ؾ� �Ѵ�!

* interface block : ���� ������ �ϳ��� interface block���� �׷�ȭ ����
  - C����� structure�� ���� : struct ��� in �Ǵ� out Ű���� ���
  - interface block�� '��� �̸�'(���� �̸� �ƴ�)�� ����� ��Ī�Ѵ�!
  - in �Ǵ� out Ű����� �ѱ� �� ��� �̸��� ���� ��� ������ �̸��� ���� ��� ������ data type�� ���ƾ� �Ѵ�.
    -> ��� �ڿ� ���� �̸��� �޶����� ��!

* tessellation : polygon�� ���� ���� polygon���� �ɰ��� �۾�
  - patch(���� primitive)�� �� �۰�, �ܼ��� ���� ���� ������ ������ primitive�� �����ϴ� �۾�
  - low-polygon�� 3D model�� tessellation�� �̿��� �Ų����� ������� ���� �� �ִ�!(Subdivision �۾�)

* Tessellation control shader : tessellation level(���� ����) ���� & tessellation evaluation shader�� ���� data ����
  - in �Ǵ� out Ű���带 �̿��� data ����
*/