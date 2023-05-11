/*
* 3d object 1���� ���� VAO�� �� �� ����
* VAO �ϳ����� ���� VBO�� �� �� �ְ�, render �ÿ� ���� Ȱ��ȭ������ �ʾƵ� �ȴ�.
* 
* ����ȭ �Ǿ� �ֱ� ������ ���ؼ� ǥ���� �� ����
* diffuse���� ������ ����ϴ� ���� : ���� �ݻ�Ǵ� ������ ���� �ٸ� ���� ���� ǥ���� �� �ֱ� ����
* normal vector�� ��ȯ�� ���� �ʿ��� ���� : ������ ��ġ�� ���� ���� �������� ���߱� ����
  -> �ٸ� space���� ���ǵǾ��ٸ� �� space�� ���� normal vector ��ȯ�� ���� �����Ѵ�.
* normal matrix���� 3*3�� ���� ���� : �̵��� �����ϱ� ����
*/

#define STB_IMAGE_IMPLEMENTATION
#include <sb7.h>
#include <vmath.h>
#include <shader.h>
#include "stb_image.h"

class my_application : public sb7::application
{
private:
	GLuint shader_programs[3];
	GLuint VAOs[3], VBOs[3], EBO;
	GLuint textures[2];

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

	void load_texture(GLuint textureID, char const* filename)						// -> *filename ���� �Ұ��� && fileane�� ���� ����
	{		
		glBindTexture(GL_TEXTURE_2D, textureID);

		int width, height, nrChannels;
		unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);

		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		stbi_image_free(data);

		// �ؽ�ó ���ø�/���͸� ����
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	virtual void startup()
	{
		stbi_set_flip_vertically_on_load(true);

		shader_programs[0] = compile_shader("./texture_vs.glsl", "./texture_fs.glsl");
		shader_programs[1] = compile_shader("./color_vs.glsl", "./color_fs.glsl");
		shader_programs[2] = compile_shader("./simple_color_vs.glsl", "./simple_color_fs.glsl");

		// VAO, VBO, EBO, texture ����
		glGenVertexArrays(3, VAOs);
		glGenBuffers(3, VBOs);
		glGenBuffers(1, &EBO);
		glGenTextures(2, textures);

		// ù ��° ��ü ���� : �ٴ� --------------------------------------------------
		glBindVertexArray(VAOs[0]);
		// �ٴ� ������ ��ġ�� �÷�, �ؽ�ó ��ǥ�� �����Ѵ�.
		float floor_s = 3.0f, floor_t = 3.0f;
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
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(floor_indices), floor_indices, GL_STATIC_DRAW);

		// VBO �� ���ؽ� �Ӽ��� �� ������ VBO�� VAO�� unbind�Ѵ�.
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		// �ؽ�ó ��ü ����� ���ε� (grass)
		load_texture(textures[0], "../src/wall.jpg");

		// �� ��° ��ü ���� : �ڽ� --------------------------------------------------
		glBindVertexArray(VAOs[1]);
		// �ڽ� ������ ��ġ�� �÷�, �ؽ�ó ��ǥ�� �����Ѵ�.
		float box_s = 1.0f, box_t = 1.0f;
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

		// �ؽ�ó ��ü ����� ���ε� 
		load_texture(textures[1], "../src/brick.jpg");

		// light box
		glBindVertexArray(VAOs[2]);
		GLfloat Light_vertices[] = {
			// position			
			// �޸�
			-0.25f, 0.5f, -0.25f,
			0.25f, 0.0f, -0.25f,
			-0.25f, 0.0f, -0.25f,

			0.25f, 0.0f, -0.25f,
			-0.25f, 0.5f, -0.25f,
			0.25f, 0.5f, -0.25f,
			// ������
			0.25f, 0.0f, -0.25f,
			0.25f, 0.5f, -0.25f,
			0.25f, 0.0f, 0.25f,	

			0.25f, 0.0f, 0.25f,	
			0.25f, 0.5f, -0.25f,
			0.25f, 0.5f, 0.25f,	
			// ����
			0.25f, 0.0f, 0.25f,	
			0.25f, 0.5f, 0.25f,	
			-0.25f, 0.0f, 0.25f,

			-0.25f, 0.0f, 0.25f,
			0.25f, 0.5f, 0.25f,	
			-0.25f, 0.5f, 0.25f,
			// ������
			-0.25f, 0.0f, 0.25f,
			-0.25f, 0.5f, 0.25f,
			-0.25f, 0.0f, -0.25f,

			-0.25f, 0.0f, -0.25f,
			-0.25f, 0.5f, 0.25f,
			-0.25f, 0.5f, -0.25f,
			// �ٴڸ�
			-0.25f, 0.0f, 0.25f,
			0.25f, 0.0f, -0.25f,
			0.25f, 0.0f, 0.25f,	

			0.25f, 0.0f, -0.25f,
			-0.25f, 0.0f, 0.25f,
			-0.25f, 0.0f, -0.25f,
			// ����
			-0.25f, 0.5f, -0.25f,
			0.25f, 0.5f, 0.25f,	
			0.25f, 0.5f, -0.25f,

			0.25f, 0.5f, 0.25f,	
			-0.25f, 0.5f, -0.25f,
			-0.25f, 0.5f, 0.25f,
		};

		// VBO�� �����Ͽ� vertices ������ ����
		glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Light_vertices), Light_vertices, GL_STATIC_DRAW);

		// VBO�� ����� �� ���ؽ� �Ӽ����� ����
		// ��ġ �Ӽ� (location = 0)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// VBO �� ���ؽ� �Ӽ��� �� ������ VBO�� VAO�� unbind�Ѵ�.
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
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

		// ī�޶� ��Ʈ���� ���
		float distance = 2.f;
		vmath::vec3 eye((float)cos(currentTime * 0.1f) * distance, 1.f, (float)sin(currentTime * 0.1f) * distance);
		// vmath::vec3 eye(0.f, 2.f, 3.f);
		vmath::vec3 center(0.0, 0.0, 0.0);
		vmath::vec3 up(0.0, 1.0, 0.0);
		vmath::mat4 lookAt = vmath::lookat(eye, center, up);
		float fov = 50.f;// (float)cos(currentTime)*20.f + 50.0f;
		vmath::mat4 projM = vmath::perspective(fov, info.windowWidth / info.windowHeight, 0.1f, 1000.0f);

		// �ٴ� �׸��� ---------------------------------------
		glUseProgram(shader_programs[0]);
		glUniformMatrix4fv(uniform_transform1, 1, GL_FALSE, projM * lookAt * vmath::scale(1.5f));
		glUniform1i(glGetUniformLocation(shader_programs[0], "texture1"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glBindVertexArray(VAOs[0]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// ������ü �׸��� --------------------------------------- 1
		glUseProgram(shader_programs[1]);
		glBindVertexArray(VAOs[1]);

		// define light's color, ambientStrength, light's position, shininess
		vmath::vec3 lightColor(1.f, 1.f, 1.f);
		GLfloat ambientStrength = 0.1f;
		vmath::vec3 lightPos(0.f, 0.f, 0.f);
		int shininess = 64;
		// vmath::vec3 lightPos((float)cos(currentTime * 0.1f) * distance, 1.f, (float)sin(currentTime * 0.1f) * distance);
		// Get lightColor variable's location
		GLint uniform_transform5 = glGetUniformLocation(shader_programs[1], "lightColor");
		GLint uniform_transform6 = glGetUniformLocation(shader_programs[1], "ambientStrength");
		GLint uniform_transform7 = glGetUniformLocation(shader_programs[1], "lightPos");
		GLint uniform_transform8 = glGetUniformLocation(shader_programs[1], "viewPos");
		GLint uniform_transform9 = glGetUniformLocation(shader_programs[1], "shininess");

		// define model translation
		vmath::mat4 model = vmath::rotate((float)currentTime * 50.f, 0.f, 1.f, 0.f);
		vmath::mat4 model_1 = vmath::translate(1.f, 0.f, 0.f) * model;
		
		glUniformMatrix4fv(uniform_transform2, 1, GL_FALSE, model_1);
		glUniformMatrix4fv(uniform_transform3, 1, GL_FALSE, lookAt);
		glUniformMatrix4fv(uniform_transform4, 1, GL_FALSE, projM);
		glUniform3fv(uniform_transform5, 1, lightColor);
		glUniform1f(uniform_transform6, ambientStrength);
		glUniform3fv(uniform_transform7, 1, lightPos);
		glUniform3fv(uniform_transform8, 1, eye);
		glUniform1i(uniform_transform9, shininess);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// ������ü �׸��� --------------------------------------- 2
		shininess = 1;
		vmath::mat4 model_2 = vmath::translate(-1.f, 0.f, 0.f) * model;
		glUniformMatrix4fv(uniform_transform2, 1, GL_FALSE, model_2);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// ������ü �׸��� --------------------------------------- 3
		glUseProgram(shader_programs[2]);
		glBindVertexArray(VAOs[2]);

		GLint uniform_transform10 = glGetUniformLocation(shader_programs[2], "color");
		GLint uniform_transform11 = glGetUniformLocation(shader_programs[2], "model");
		GLint uniform_transform12 = glGetUniformLocation(shader_programs[2], "view");
		GLint uniform_transform13 = glGetUniformLocation(shader_programs[2], "projection");

		vmath::mat4 model_3 = vmath::translate(lightPos) * model * vmath::scale(0.1f);

		glUniform3fv(uniform_transform10, 1, lightColor);
		glUniformMatrix4fv(uniform_transform11, 1, GL_FALSE, model_3);
		glUniformMatrix4fv(uniform_transform12, 1, GL_FALSE, lookAt);
		glUniformMatrix4fv(uniform_transform13, 1, GL_FALSE, projM);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	virtual void shutdown()
	{
		glDeleteTextures(2, textures);
		glDeleteBuffers(1, &EBO);
		glDeleteBuffers(2, VBOs);
		glDeleteVertexArrays(2, VAOs);
		glDeleteProgram(shader_programs[0]);
		glDeleteProgram(shader_programs[1]);
	}
};

// DECLARE_MAIN�� �ϳ����� �ν��Ͻ�
DECLARE_MAIN(my_application)