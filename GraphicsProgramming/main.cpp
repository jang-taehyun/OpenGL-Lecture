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

		// -> glsl ���Ϸ� �ε��ϴ� ���
		// --> shader �����ϱ��� ���ش�!
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

			// geometry shader ���� ���� vertex shader
			"#version 430 core												\n"
			"																\n"
			"layout (location = 0) in vec4 offset;							\n"
			"layout (location = 1) in vec4 color;							\n"
			"																\n"
			//"out VS_OUT {													\n"		// ��� �̸�(in Ű����� ��Ī)
			//"	vec4 color;													\n"		// ��� ���� ��� ����(in Ű����� ��Ī)
			//"} vs_out;														\n"		// ���� �̸�(in Ű����� ��Ī ���ص� ��)
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

			// gl_FragCoord ��� ���� vertex shader
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

			// �Է� ���� vertex shader
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

			// geometry shader ���� ���� fragment shader
			"#version 430 core						\n"
			"										\n"
			//"in VS_OUT {							\n"			// ��� �̸�(in Ű����� ��Ī)
			//"	vec4 color;							\n"			// ��� ���� ��� ����(in Ű����� ��Ī)
			//"} fs_in;								\n"			// ���� �̸�(in Ű����� ��Ī ���ص� ��)
			"in vec4 vs_color;						\n"
			"out vec4 color;						\n"
			"										\n"
			"void main()							\n"
			"{										\n"
			//"	color = fs_in.color;				\n"
			"	color = vs_color;					\n"
			"}										\n"

			// gl_FrgaCoord ��� ���� fragment shader
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

			//// �Է� ���� fragment shader
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

		// vertex attribute�� 0��° index�� ���� ����(�ﰢ�� offset)
		const GLfloat attrib[] = {
						(float)sin(currentTime) * 0.5f,		// x
						(float)cos(currentTime) * 0.5f,		// y
						0.f,								// z
						0.f									// homogeneous ���
		};
		glVertexAttrib4fv(0,			// GLuint index : vertex shader���� attribute�� �����ϱ� ���� index
						  attrib		// const GLfloat* v : attribute�� ���� ���ο� data�� ����Ű�� pointer
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