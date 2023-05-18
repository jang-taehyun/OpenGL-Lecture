#define STB_IMAGE_IMPLEMENTATION

//#define DIRECTIONAL_LIGHT
//#define POINT_LIGHT
//#define SPOT_LIGHT
#define MULTIPLE_LIGHT

#include <sb7.h>
#include <vmath.h>
#include <shader.h>
#include "stb_image.h"
#include <vector>

class my_application : public sb7::application
{
private:
	GLuint shader_programs[3];
	GLuint VAOs[3], VBOs[3], EBOs[2];
	GLuint textures[3];

	std::vector<vmath::vec3> boxPositions;

public:
	GLuint compile_shader(const char* vs_file, const char* fs_file)					// -> *vs_file, *fs_file ���� ���� && vs_file, fs_file�� ���� �Ұ���
	{
		GLuint vertex_shader = sb7::shader::load(vs_file, GL_VERTEX_SHADER);
		GLuint fragment_shader = sb7::shader::load(fs_file, GL_FRAGMENT_SHADER);

		GLuint program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);
		glLinkProgram(program);

		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		return program;
	}

	void load_texture(GLuint textureID, char const* filename, GLenum GL_ColorFormat = GL_RGB, GLenum GL_InputFormat = GL_RGB)						// -> *filename ���� �Ұ��� && fileane�� ���� ����
	{
		glBindTexture(GL_TEXTURE_2D, textureID);

		int width, height, nrChannels;
		unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);

		if (data) {
			// �տ� format : �ѱ� �������� ä��
			// �ڿ� format : 
			glTexImage2D(GL_TEXTURE_2D, 0, GL_ColorFormat, width, height, 0, GL_InputFormat, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		stbi_image_free(data);

		// �ؽ�ó ���ø�/���͸� ����
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	virtual void startup()
	{
		stbi_set_flip_vertically_on_load(true);

		shader_programs[0] = compile_shader("./texture_vs.glsl", "./texture_fs.glsl");
		shader_programs[1] = compile_shader("./test_vs.glsl", "./test_fs.glsl");
		shader_programs[2] = compile_shader("./simple_color_vs.glsl", "./simple_color_fs.glsl");

		// VAO, VBO, EBO, texture ����
		glGenVertexArrays(3, VAOs);
		glGenBuffers(3, VBOs);
		glGenBuffers(2, EBOs);
		glGenTextures(3, textures);

		// ù ��° ��ü ���� : �ٴ� --------------------------------------------------
		glBindVertexArray(VAOs[0]);
		// �ٴ� ������ ��ġ�� �÷�, �ؽ�ó ��ǥ�� �����Ѵ�.
		float floor_s = 5.0f, floor_t = 5.0f;
		GLfloat floor_vertices[] = {
			1.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, floor_s, floor_t,  // ���� ���
			-1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, floor_t,  // ���� ���
			-1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,   // ���� �ϴ�
			1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, floor_s, 0.0f   // ���� �ϴ�
		};

		// �ﰢ������ �׸� �ε����� �����Ѵ�.
		GLuint floor_indices[] = {
			0, 1, 2,	// ù��° �ﰢ��
			0, 2, 3		// �ι�° �ﰢ��
		};

		// VBO�� �����Ͽ� vertices ������ ����
		glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(floor_vertices), floor_vertices, GL_STATIC_DRAW);

		// VBO�� ����� �� ���ؽ� �Ӽ����� ����
		// ��ġ �Ӽ� (location = 0)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// �÷� �Ӽ� (location = 1)
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// �ؽ�ó ��ǥ �Ӽ� (location = 2)
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		// EBO�� �����ϰ� indices ������ ����
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(floor_indices), floor_indices, GL_STATIC_DRAW);

		// VBO �� ���ؽ� �Ӽ��� �� ������ VBO�� VAO�� unbind�Ѵ�.
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		// �ؽ�ó ��ü ����� ���ε�
		load_texture(textures[0], "../src/wall.jpg");
		// �ؽ�ó ��ü ����� ���ε� 
		load_texture(textures[1], "../src/container2.png", GL_RGBA, GL_RGBA);
		// �ؽ�ó ��ü ����� ���ε� 
		load_texture(textures[2], "../src/container2_specular.png", GL_RGBA, GL_RGBA);

		// �� ��° ��ü ���� : �ڽ� --------------------------------------------------
		glBindVertexArray(VAOs[1]);
		// �ڽ� ������ ��ġ�� �÷�, �ؽ�ó ��ǥ�� �����Ѵ�.
		float box_s = 1.f, box_t = 1.f;
		vmath::vec3 vertices[8] = {
			vmath::vec3(0.25f, 0.5f, 0.25f),		// v0
			vmath::vec3(-0.25f, 0.5f, 0.25f),		// v1
			vmath::vec3(-0.25f, 0.f, 0.25f),		// v2
			vmath::vec3(0.25f, 0.f, 0.25f),			// v3
			vmath::vec3(0.25f, 0.5f, -0.25f),		// v4
			vmath::vec3(-0.25f, 0.5f, -0.25f),		// v5
			vmath::vec3(-0.25f, 0.f, -0.25f),		// v6
			vmath::vec3(0.25f, 0.f, -0.25f)			// v7
		};
		vmath::vec3 normal[6] = {
			// back
			vmath::cross(vertices[4] - vertices[5], vertices[6] - vertices[5]),
			// right
			vmath::cross(vertices[0] - vertices[4], vertices[7] - vertices[4]),
			// front
			vmath::cross(vertices[1] - vertices[0], vertices[3] - vertices[0]),
			// left
			vmath::cross(vertices[6] - vertices[5], vertices[1] - vertices[5]),
			// buttom
			vmath::cross(vertices[3] - vertices[7], vertices[6] - vertices[7]),
			// top
			vmath::cross(vertices[5] - vertices[4], vertices[0] - vertices[4]),
		};
		GLfloat box_vertices[] = {
			// position				// color			// tex postion		// normal
			// �޸�
			-0.25f, 0.5f, -0.25f,	1.0f, 0.0f, 0.0f,	box_s, box_t,		normal[0][0], normal[0][1], normal[0][2],
			0.25f, 0.0f, -0.25f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f,			normal[0][0], normal[0][1], normal[0][2],
			-0.25f, 0.0f, -0.25f,	1.0f, 0.0f, 0.0f,	box_s, 0.0f,		normal[0][0], normal[0][1], normal[0][2],

			0.25f, 0.0f, -0.25f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f,			normal[0][0], normal[0][1], normal[0][2],
			-0.25f, 0.5f, -0.25f,	1.0f, 0.0f, 0.0f,	box_s, box_t,		normal[0][0], normal[0][1], normal[0][2],
			0.25f, 0.5f, -0.25f,	1.0f, 0.0f, 0.0f,	0.0f, box_t,		normal[0][0], normal[0][1], normal[0][2],
			// ������
			0.25f, 0.0f, -0.25f,	0.0f, 1.0f, 0.0f,	box_s, 0.0f,		normal[1][0], normal[1][1], normal[1][2],
			0.25f, 0.5f, -0.25f,	0.0f, 1.0f, 0.0f,	box_s, box_t,		normal[1][0], normal[1][1], normal[1][2],
			0.25f, 0.0f, 0.25f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f,			normal[1][0], normal[1][1], normal[1][2],

			0.25f, 0.0f, 0.25f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f,			normal[1][0], normal[1][1], normal[1][2],
			0.25f, 0.5f, -0.25f,	0.0f, 1.0f, 0.0f,	box_s, box_t,		normal[1][0], normal[1][1], normal[1][2],
			0.25f, 0.5f, 0.25f,		0.0f, 1.0f, 0.0f,	0.0f, box_t,		normal[1][0], normal[1][1], normal[1][2],
			// ����
			0.25f, 0.0f, 0.25f,		0.0f, 0.0f, 1.0f,	box_s, 0.0f,		normal[2][0], normal[2][1], normal[2][2],
			0.25f, 0.5f, 0.25f,		0.0f, 0.0f, 1.0f,	box_s, box_t,		normal[2][0], normal[2][1], normal[2][2],
			-0.25f, 0.0f, 0.25f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f,			normal[2][0], normal[2][1], normal[2][2],

			-0.25f, 0.0f, 0.25f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f,			normal[2][0], normal[2][1], normal[2][2],
			0.25f, 0.5f, 0.25f,		0.0f, 0.0f, 1.0f,	box_s, box_t,		normal[2][0], normal[2][1], normal[2][2],
			-0.25f, 0.5f, 0.25f,	0.0f, 0.0f, 1.0f,	0.0f, box_t,		normal[2][0], normal[2][1], normal[2][2],
			// ������
			-0.25f, 0.0f, 0.25f,	1.0f, 0.0f, 1.0f,	box_s, 0.0f,		normal[3][0], normal[3][1], normal[3][2],
			-0.25f, 0.5f, 0.25f,	1.0f, 0.0f, 1.0f,	box_s, box_t,		normal[3][0], normal[3][1], normal[3][2],
			-0.25f, 0.0f, -0.25f,	1.0f, 0.0f, 1.0f,	0.0f, 0.0f,			normal[3][0], normal[3][1], normal[3][2],

			-0.25f, 0.0f, -0.25f,	1.0f, 0.0f, 1.0f,	0.0f, 0.0f,			normal[3][0], normal[3][1], normal[3][2],
			-0.25f, 0.5f, 0.25f,	1.0f, 0.0f, 1.0f,	box_s, box_t,		normal[3][0], normal[3][1], normal[3][2],
			-0.25f, 0.5f, -0.25f,	1.0f, 0.0f, 1.0f,	0.0f, box_t,		normal[3][0], normal[3][1], normal[3][2],
			// �ٴڸ�
			-0.25f, 0.0f, 0.25f,	1.0f, 1.0f, 0.0f,	box_s, 0.0f,		normal[4][0], normal[4][1], normal[4][2],
			0.25f, 0.0f, -0.25f,	1.0f, 1.0f, 0.0f,	0.0f, box_t,		normal[4][0], normal[4][1], normal[4][2],
			0.25f, 0.0f, 0.25f,		1.0f, 1.0f, 0.0f,	0.0f, 0.0f,			normal[4][0], normal[4][1], normal[4][2],

			0.25f, 0.0f, -0.25f,	1.0f, 1.0f, 0.0f,	0.0f, box_t,		normal[4][0], normal[4][1], normal[4][2],
			-0.25f, 0.0f, 0.25f,	1.0f, 1.0f, 0.0f,	box_s, 0.0,			normal[4][0], normal[4][1], normal[4][2],
			-0.25f, 0.0f, -0.25f,	1.0f, 1.0f, 0.0f,	box_s, box_t,		normal[4][0], normal[4][1], normal[4][2],
			// ����
			-0.25f, 0.5f, -0.25f,	0.0f, 1.0f, 1.0f,	0.0f, box_t,		normal[5][0], normal[5][1], normal[5][2],
			0.25f, 0.5f, 0.25f,		0.0f, 1.0f, 1.0f,	box_s, 0.0f,		normal[5][0], normal[5][1], normal[5][2],
			0.25f, 0.5f, -0.25f,	0.0f, 1.0f, 1.0f,	box_s, box_t,		normal[5][0], normal[5][1], normal[5][2],

			0.25f, 0.5f, 0.25f,		0.0f, 1.0f, 1.0f,	box_s, 0.0f,		normal[5][0], normal[5][1], normal[5][2],
			-0.25f, 0.5f, -0.25f,	0.0f, 1.0f, 1.0f,	0.0f, box_t,		normal[5][0], normal[5][1], normal[5][2],
			-0.25f, 0.5f, 0.25f,	0.0f, 1.0f, 1.0f,	0.0f, 0.0f,			normal[5][0], normal[5][1], normal[5][2],
		};

		// VBO�� �����Ͽ� vertices ������ ����
		glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(box_vertices), box_vertices, GL_STATIC_DRAW);

		// VBO�� ����� �� ���ؽ� �Ӽ����� ����
		// ��ġ �Ӽ� (location = 0)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// �÷� �Ӽ� (location = 1)
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// �ؽ�ó ��ǥ �Ӽ� (location = 2)
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
		// normal vector �Ӽ� (location = 3)
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
		glEnableVertexAttribArray(3);

		// VBO �� ���ؽ� �Ӽ��� �� ������ VBO�� VAO�� unbind�Ѵ�.
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		// light box
		glBindVertexArray(VAOs[2]);
		// �Ƕ�̵� ������ ��ġ�� �÷�, �ؽ�ó ��ǥ�� �����Ѵ�.
		GLfloat pyramid_vertices[] = {
			1.0f, 0.0f, -1.0f,    // ���� ���
			-1.0f, 0.0f, -1.0f,   // ���� ���
			-1.0f, 0.0f, 1.0f,    // ���� �ϴ�
			1.0f, 0.0f, 1.0f,     // ���� �ϴ�
			0.0f, 1.0f, 0.0f,      // ��� ������
			0.0f, -1.0f, 0.0f,      // �ϴ� ������
		};

		// �ﰢ������ �׸� �ε����� �����Ѵ�.
		GLuint pyramid_indices[] = {
			4, 0, 1,
			4, 1, 2,
			4, 2, 3,
			4, 3, 0,

			5, 1, 0,
			5, 2, 1,
			5, 3, 2,
			5, 0, 3,
		};

		// VBO�� �����Ͽ� vertices ������ ����
		glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid_vertices), pyramid_vertices, GL_STATIC_DRAW);

		// VBO�� ����� �� ���ؽ� �Ӽ����� ����
		// ��ġ �Ӽ� (location = 0)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// EBO�� �����ϰ� indices ������ ����
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pyramid_indices), pyramid_indices, GL_STATIC_DRAW);

		// VBO �� ���ؽ� �Ӽ��� �� ������ VBO�� VAO�� unbind�Ѵ�.
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		// �ڽ� 10�� ������ ����
		boxPositions.push_back(vmath::vec3(0.0f, 0.0f, 0.0f));
		boxPositions.push_back(vmath::vec3(2.0f, 5.0f, -15.0f));
		boxPositions.push_back(vmath::vec3(-1.5f, -2.2f, -2.5f));
		boxPositions.push_back(vmath::vec3(-3.8f, -2.0f, -12.3f));
		boxPositions.push_back(vmath::vec3(2.4f, -0.4f, -3.5f));
		boxPositions.push_back(vmath::vec3(-1.7f, 3.0f, -7.5f));
		boxPositions.push_back(vmath::vec3(1.3f, -2.0f, -2.5f));
		boxPositions.push_back(vmath::vec3(1.5f, 2.0f, -2.5f));
		boxPositions.push_back(vmath::vec3(1.5f, 0.2f, -1.5f));
		boxPositions.push_back(vmath::vec3(-1.3f, 1.0f, -1.5f));
	}

	// ������ virtual �Լ��� �ۼ��ؼ� �������̵��Ѵ�.
	virtual void render(double currentTime)
	{
		const GLfloat color[] = { (float)sin(currentTime) * 0.5f + 0.5f, (float)cos(currentTime) * 0.5f + 0.5f, 0.0f, 1.0f };
		const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, black);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		GLint uniform_transform1 = glGetUniformLocation(shader_programs[0], "transform");
		GLint uniform_transform2 = glGetUniformLocation(shader_programs[1], "model");
		GLint uniform_transform3 = glGetUniformLocation(shader_programs[1], "view");
		GLint uniform_transform4 = glGetUniformLocation(shader_programs[1], "projection");

		// Get lightColor variable's location
		GLint uniform_transform8 = glGetUniformLocation(shader_programs[1], "viewPos");

		GLint uniform_transform18 = glGetUniformLocation(shader_programs[1], "material.diffuse");
		GLint uniform_transform14 = glGetUniformLocation(shader_programs[1], "material.specular");
		GLint uniform_transform15 = glGetUniformLocation(shader_programs[1], "material.shininess");

#ifdef MULTIPLE_LIGHT

#else
		GLint uniform_transform5 = glGetUniformLocation(shader_programs[1], "light.ambient");
		GLint uniform_transform16 = glGetUniformLocation(shader_programs[1], "light.diffuse");
		GLint uniform_transform17 = glGetUniformLocation(shader_programs[1], "light.specular");

#ifdef DIRECTIONAL_LIGHT
		GLint uniform_transform7 = glGetUniformLocation(shader_programs[1], "light.direction");
#endif // DIRECTIONAL_LIGHT

#ifdef POINT_LIGHT
		GLint uniform_transform7 = glGetUniformLocation(shader_programs[1], "light.position");
#endif // POINT_LIGHT

#ifdef SPOT_LIGHT
		GLint uniform_transform7 = glGetUniformLocation(shader_programs[1], "light.position");
		GLint uniform_transform19 = glGetUniformLocation(shader_programs[1], "light.direction");
#endif // SPOT_LIGHT

#endif // MULTIPLE_LIGHT

		// ī�޶� ��Ʈ���� ���
		float distance = 5.f;
		vmath::vec3 eye((float)cos(currentTime * 0.1f) * distance, 1.0, (float)sin(currentTime * 0.1f) * distance);
		vmath::vec3 center(0.0, 0.0, 0.0);
		vmath::vec3 up(0.0, 1.0, 0.0);
		vmath::mat4 lookAt = vmath::lookat(eye, center, up);
		float fov = 50.f;
		vmath::mat4 projM = vmath::perspective(fov, (float)info.windowWidth / info.windowHeight, 0.1f, 1000.0f);

		// ������ü �׸��� --------------------------------------- 1
		glUseProgram(shader_programs[1]);
		glBindVertexArray(VAOs[1]);

		// define object's ambient, diffuse, specular, shininess
		vmath::vec3 ambient(1.f, 0.5f, 0.31f);
		vmath::vec3 diffuse(1.f, 0.5f, 0.31f);
		vmath::vec3 specular(0.5f, 0.5f, 0.5f);
		int shininess = 32;

		// define light's ambient, diffuse, shininess
		vmath::vec3 lightAmbient(0.2f, 0.2f, 0.2f);
		vmath::vec3 lightDiffuse(0.5f, 0.5f, 0.5f);
		vmath::vec3 lightSpecular(1.f, 1.f, 1.f);

#ifdef DIRECTIONAL_LIGHT
		vmath::vec3 lightDirection(vmath::vec3((float)sin(currentTime * 0.5f), 0.25f, (float)cos(currentTime * 0.5f) * 0.7f));
#endif

#ifdef POINT_LIGHT
		vmath::vec3 lightPosition(vmath::vec3((float)sin(currentTime * 0.5f), 0.25f, (float)cos(currentTime * 0.5f) * 0.7f));
#endif

#ifdef SPOT_LIGHT
		vmath::vec3 lightPosition(vmath::vec3((float)sin(currentTime * 0.5f), 0.25f, (float)cos(currentTime * 0.5f) * 0.7f));
		vmath::vec3 lightDirection(vmath::vec3((float)sin(currentTime * 0.5f), 0.25f, (float)cos(currentTime * 0.5f) * 0.7f));
#endif

		// define model translation
		vmath::mat4 model = vmath::rotate((float)currentTime * 50.f, 0.f, 1.f, 0.f);
		vmath::mat4 model_1 = model;

		// link model transform
		glUniformMatrix4fv(uniform_transform3, 1, GL_FALSE, lookAt);
		glUniformMatrix4fv(uniform_transform4, 1, GL_FALSE, projM);

		// link camera position
		glUniform3fv(uniform_transform8, 1, eye);

		// link object's material
		glUniform1i(uniform_transform18, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[1]);
		glUniform1i(uniform_transform14, 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textures[2]);
		glUniform1i(uniform_transform15, shininess);

#ifdef MULTIPLE_LIGHT
		vmath::vec3 pointLightPos[3] = {
			vmath::vec3(vmath::vec3((float)sin(currentTime * 0.5f), 0.25f, (float)cos(currentTime * 0.5f) * 0.7f)),
			vmath::vec3(vmath::vec3(0.25f, (float)sin(currentTime * 1.f), (float)cos(currentTime * 1.f) * 0.7f)),
			vmath::vec3(vmath::vec3((float)cos(currentTime * 1.5f) * 0.7f, (float)sin(currentTime * 1.5f) * 0.7f, 0.25f))
		};
		vmath::vec3 pointLightColor[3] = {
			vmath::vec3(1.f, 0.f, 0.f),
			vmath::vec3(0.f, 1.f, 0.f),
			vmath::vec3(0.f, 0.f, 1.f)
		};

		float pointLightAmbient = 0.05f;	// 0.05f
		float pointLightSpecular = 1.f;		// 1.f
		float c1 = 0.09f;					// 0.09f
		float c2 = 0.032f;					// 0.032f
		float spotLightSpecular = 0.8f;		// 1.f
		float spotLightDiffuse = 1.f;		// 1.f

		glUniform3f(glGetUniformLocation(shader_programs[1], "dirLight.direction"), -1.f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(shader_programs[1], "dirLight.ambient"), 0.005f, 0.005f, 0.005f);
		glUniform3f(glGetUniformLocation(shader_programs[1], "dirLight.diffuse"), 0.04f, 0.04f, 0.04f);
		glUniform3f(glGetUniformLocation(shader_programs[1], "dirLight.specular"), 0.05f, 0.05f, 0.05f);

		glUniform3fv(glGetUniformLocation(shader_programs[1], "pointLights[0].position"), 1, pointLightPos[0]);
		glUniform3f(glGetUniformLocation(shader_programs[1], "pointLights[0].ambient"), pointLightAmbient, 0.f, 0.f);
		//glUniform3f(glGetUniformLocation(shader_programs[1], "pointLights[0].diffuse"), 0.8f, 0.8f, 0.8f);
		glUniform3fv(glGetUniformLocation(shader_programs[1], "pointLights[0].diffuse"), 1, pointLightColor[0]);
		glUniform3f(glGetUniformLocation(shader_programs[1], "pointLights[0].specular"), pointLightSpecular, 0.f, 0.f);
		glUniform1f(glGetUniformLocation(shader_programs[1], "pointLights[0].c1"), c1);
		glUniform1f(glGetUniformLocation(shader_programs[1], "pointLights[0].c2"), c2);

		glUniform3fv(glGetUniformLocation(shader_programs[1], "pointLights[1].position"), 1, pointLightPos[1]);
		glUniform3f(glGetUniformLocation(shader_programs[1], "pointLights[1].ambient"), 0.f, pointLightAmbient, 0.f);
		//glUniform3f(glGetUniformLocation(shader_programs[1], "pointLights[1].diffuse"), 0.8f, 0.8f, 0.8f);
		glUniform3fv(glGetUniformLocation(shader_programs[1], "pointLights[1].diffuse"), 1, pointLightColor[1]);
		glUniform3f(glGetUniformLocation(shader_programs[1], "pointLights[1].specular"), 0.f, pointLightSpecular, 0.f);
		glUniform1f(glGetUniformLocation(shader_programs[1], "pointLights[1].c1"), c1);
		glUniform1f(glGetUniformLocation(shader_programs[1], "pointLights[1].c2"), c2);

		glUniform3fv(glGetUniformLocation(shader_programs[1], "pointLights[2].position"), 1, pointLightPos[2]);
		glUniform3f(glGetUniformLocation(shader_programs[1], "pointLights[2].ambient"), 0.f, 0.f, pointLightAmbient);
		//glUniform3f(glGetUniformLocation(shader_programs[1], "pointLights[2].diffuse"), 0.8f, 0.8f, 0.8f);
		glUniform3fv(glGetUniformLocation(shader_programs[1], "pointLights[2].diffuse"), 1, pointLightColor[2]);
		glUniform3f(glGetUniformLocation(shader_programs[1], "pointLights[2].specular"), 0.f, 0.f, pointLightSpecular);
		glUniform1f(glGetUniformLocation(shader_programs[1], "pointLights[2].c1"), c1);
		glUniform1f(glGetUniformLocation(shader_programs[1], "pointLights[2].c2"), c2);

		glUniform3fv(glGetUniformLocation(shader_programs[1], "spotLight.position"), 1, eye);
		glUniform3fv(glGetUniformLocation(shader_programs[1], "spotLight.direction"), 1, center - eye);
		glUniform1f(glGetUniformLocation(shader_programs[1], "spotLight.cutOff"), (float)cos(vmath::radians(12.5)));
		glUniform1f(glGetUniformLocation(shader_programs[1], "spotLight.outerCutOff"), (float)cos(vmath::radians(15.5)));
		glUniform1f(glGetUniformLocation(shader_programs[1], "spotLight.c1"), 0.09f);
		glUniform1f(glGetUniformLocation(shader_programs[1], "spotLight.c2"), 0.032f);
		glUniform3f(glGetUniformLocation(shader_programs[1], "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(shader_programs[1], "spotLight.diffuse"), spotLightDiffuse, spotLightDiffuse, spotLightDiffuse);
		glUniform3f(glGetUniformLocation(shader_programs[1], "spotLight.specular"), spotLightSpecular, spotLightSpecular, spotLightSpecular);

#else

#ifdef DIRECTIONAL_LIGHT
		// Directional Light's material Link
		glUniform3f(uniform_transform5, 0.5f, 0.5f, 0.5f);			// ambient
		glUniform3f(uniform_transform16, 0.4f, 0.4f, 0.4f);			// diffuse
		glUniform3f(uniform_transform17, 0.5f, 0.5f, 0.5f);			// specular
		glUniform3f(uniform_transform7, -1.f, 0.f, 0.f);			// direction
#endif // DIRECTIONAL_LIGHT

#ifdef POINT_LIGHT
		// Point light's material Link
		glUniform3f(uniform_transform5, 0.1f, 0.1f, 0.1f);							// ambient
		glUniform3f(uniform_transform16, 0.8f, 0.8f, 0.8f);							// diffuse
		glUniform3f(uniform_transform17, 1.f, 1.f, 1.f);							// specular
		glUniform3fv(uniform_transform7, 1, lightPosition);							// position
		glUniform1f(glGetUniformLocation(shader_programs[1], "light.c1"), 0.09f);	// c1
		glUniform1f(glGetUniformLocation(shader_programs[1], "light.c2"), 0.032f);	// c2
#endif // POINT_LIGHT

#ifdef SPOT_LIGHT
		// Spot light's material Link
		glUniform3f(uniform_transform5, 0.2f, 0.2f, 0.2f);																// ambient
		glUniform3f(uniform_transform16, 1.f, 1.f, 1.f);																// diffuse
		glUniform3f(uniform_transform17, 1.f, 1.f, 1.f);																// specular
		glUniform3fv(uniform_transform7, 1, eye);																		// position
		glUniform3fv(uniform_transform19, 1, center - eye);																// direction
		glUniform1f(glGetUniformLocation(shader_programs[1], "light.cutOff"), (float)cos(vmath::radians(12.5)));		// cut off
		glUniform1f(glGetUniformLocation(shader_programs[1], "light.outerCufOff"), (float)cos(vmath::radians(15.5)));	// cut off
		glUniform1f(glGetUniformLocation(shader_programs[1], "light.c1"), 0.09f);										// c1
		glUniform1f(glGetUniformLocation(shader_programs[1], "light.c2"), 0.032f);										// c2
#endif // SPOT_LIGHT

#endif // MULTIPLE_LIGHT

		for (int i = 0; i < boxPositions.size(); i++)
		{
			float angle = 20.f * i;
			vmath::mat4 model = vmath::translate(boxPositions[i]) *
				vmath::rotate(angle, 1.0f, 0.3f, 0.5f) *
				vmath::scale(1.0f);
			glUniformMatrix4fv(uniform_transform2, 1, GL_FALSE, model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glBindTexture(GL_TEXTURE_2D, 0);
		// ���� �׸��� ---------------------------------------
		glUseProgram(shader_programs[2]);
		glBindVertexArray(VAOs[2]);

		float angle = currentTime * 100;
		float move_y = (float)cos(currentTime) * 0.2f + 0.5f;
		float scaleFactor = 0.05f;// (float)cos(currentTime)*0.05f + 0.2f;
		vmath::vec3 lightPos = vmath::vec3((float)sin(currentTime * 0.5f), 0.25f, (float)cos(currentTime * 0.5f) * 0.7f);// (0.0f, 0.5f, 0.0f);
		vmath::vec3 lightColor(1.0f, 1.0f, 1.0f);
		vmath::mat4 transform = vmath::translate(lightPos) *
			vmath::rotate(angle * 0.5f, 0.0f, 1.0f, 0.0f) *
			vmath::scale(scaleFactor, scaleFactor, scaleFactor);

#ifdef MULTIPLE_LIGHT
		
		for (int i = 0; i < 3; i++)
		{
			lightPos = pointLightPos[i];
			lightColor = pointLightColor[i];
			transform = vmath::translate(lightPos) *
				vmath::rotate(angle * 0.5f, 0.0f, 1.0f, 0.0f) *
				vmath::scale(scaleFactor, scaleFactor, scaleFactor);

			glUniform3fv(glGetUniformLocation(shader_programs[2], "color"), 1, lightColor);
			glUniformMatrix4fv(glGetUniformLocation(shader_programs[2], "projection"), 1, GL_FALSE, projM);
			glUniformMatrix4fv(glGetUniformLocation(shader_programs[2], "view"), 1, GL_FALSE, lookAt);
			glUniformMatrix4fv(glGetUniformLocation(shader_programs[2], "model"), 1, GL_FALSE, transform);

			glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);
		}
#else
		glUniform3fv(glGetUniformLocation(shader_programs[2], "color"), 1, lightColor);
		glUniformMatrix4fv(glGetUniformLocation(shader_programs[2], "projection"), 1, GL_FALSE, projM);
		glUniformMatrix4fv(glGetUniformLocation(shader_programs[2], "view"), 1, GL_FALSE, lookAt);
		glUniformMatrix4fv(glGetUniformLocation(shader_programs[2], "model"), 1, GL_FALSE, transform);
		glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);
#endif


		
	}

	virtual void shutdown()
	{
		glDeleteTextures(2, textures);
		glDeleteBuffers(2, EBOs);
		glDeleteBuffers(3, VBOs);
		glDeleteVertexArrays(3, VAOs);
		glDeleteProgram(shader_programs[0]);
		glDeleteProgram(shader_programs[1]);
		glDeleteProgram(shader_programs[2]);
	}
};

// DECLARE_MAIN�� �ϳ����� �ν��Ͻ�
DECLARE_MAIN(my_application)