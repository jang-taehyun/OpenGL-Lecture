/*
* 3d object 1개에 여러 VAO가 들어갈 수 있음
* VAO 하나에는 여러 VBO가 들어갈 수 있고, render 시에 따로 활성화해주지 않아도 된다.
* 
* 정규화 되어 있기 때문에 곱해서 표현할 수 있음
* diffuse에서 내적을 사용하는 이유 : 빛이 반사되는 각도에 따라 다른 빛의 양을 표현할 수 있기 때문
* normal vector의 변환이 따로 필요한 이유 : 광원의 위치를 월드 공간 기준으로 정했기 때문
  -> 다른 space에서 정의되었다면 그 space에 따라 normal vector 변환을 따로 정의한다.
* normal matrix에서 3*3만 쓰는 이유 : 이동을 제외하기 위해
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
	GLuint compile_shader(const char* vs_file, const char* fs_file)					// -> *vs_file, *fs_file 변경 가능 && vs_file, fs_file은 변경 불가능
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

	void load_texture(GLuint textureID, char const* filename)						// -> *filename 변경 불가능 && fileane은 변경 가능
	{		
		glBindTexture(GL_TEXTURE_2D, textureID);

		int width, height, nrChannels;
		unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);

		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		stbi_image_free(data);

		// 텍스처 샘플링/필터링 설정
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

		// VAO, VBO, EBO, texture 생성
		glGenVertexArrays(3, VAOs);
		glGenBuffers(3, VBOs);
		glGenBuffers(1, &EBO);
		glGenTextures(2, textures);

		// 첫 번째 객체 정의 : 바닥 --------------------------------------------------
		glBindVertexArray(VAOs[0]);
		// 바닥 점들의 위치와 컬러, 텍스처 좌표를 정의한다.
		float floor_s = 3.0f, floor_t = 3.0f;
		GLfloat floor_vertices[] = {
			1.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, floor_s, floor_t,  // 우측 상단
			-1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, floor_t,  // 좌측 상단
			-1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,   // 좌측 하단
			1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, floor_s, 0.0f   // 우측 하단
		};

		// 삼각형으로 그릴 인덱스를 정의한다.
		GLuint floor_indices[] = {
			0, 1, 2,	// 첫번째 삼각형
			0, 2, 3		// 두번째 삼각형
		};

		// VBO를 생성하여 vertices 값들을 복사
		glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(floor_vertices), floor_vertices, GL_STATIC_DRAW);

		// VBO를 나누어서 각 버텍스 속성으로 연결
		// 위치 속성 (location = 0)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// 컬러 속성 (location = 1)
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// 텍스처 좌표 속성 (location = 2)
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		// EBO를 생성하고 indices 값들을 복사
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(floor_indices), floor_indices, GL_STATIC_DRAW);

		// VBO 및 버텍스 속성을 다 했으니 VBO와 VAO를 unbind한다.
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		// 텍스처 객체 만들고 바인딩 (grass)
		load_texture(textures[0], "../src/wall.jpg");

		// 두 번째 객체 정의 : 박스 --------------------------------------------------
		glBindVertexArray(VAOs[1]);
		// 박스 점들의 위치와 컬러, 텍스처 좌표를 정의한다.
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
			// 뒷면
			-0.25f, 0.5f, -0.25f,	1.0f, 0.0f, 0.0f,	box_s, box_t,		normal[0][0], normal[0][1], normal[0][2],
			0.25f, 0.0f, -0.25f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f,			normal[0][0], normal[0][1], normal[0][2],
			-0.25f, 0.0f, -0.25f,	1.0f, 0.0f, 0.0f,	box_s, 0.0f,		normal[0][0], normal[0][1], normal[0][2],

			0.25f, 0.0f, -0.25f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f,			normal[0][0], normal[0][1], normal[0][2],
			-0.25f, 0.5f, -0.25f,	1.0f, 0.0f, 0.0f,	box_s, box_t,		normal[0][0], normal[0][1], normal[0][2],
			0.25f, 0.5f, -0.25f,	1.0f, 0.0f, 0.0f,	0.0f, box_t,		normal[0][0], normal[0][1], normal[0][2],
			// 우측면
			0.25f, 0.0f, -0.25f,	0.0f, 1.0f, 0.0f,	box_s, 0.0f,		normal[1][0], normal[1][1], normal[1][2],
			0.25f, 0.5f, -0.25f,	0.0f, 1.0f, 0.0f,	box_s, box_t,		normal[1][0], normal[1][1], normal[1][2],
			0.25f, 0.0f, 0.25f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f,			normal[1][0], normal[1][1], normal[1][2],

			0.25f, 0.0f, 0.25f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f,			normal[1][0], normal[1][1], normal[1][2],
			0.25f, 0.5f, -0.25f,	0.0f, 1.0f, 0.0f,	box_s, box_t,		normal[1][0], normal[1][1], normal[1][2],
			0.25f, 0.5f, 0.25f,		0.0f, 1.0f, 0.0f,	0.0f, box_t,		normal[1][0], normal[1][1], normal[1][2],
			// 정면
			0.25f, 0.0f, 0.25f,		0.0f, 0.0f, 1.0f,	box_s, 0.0f,		normal[2][0], normal[2][1], normal[2][2],
			0.25f, 0.5f, 0.25f,		0.0f, 0.0f, 1.0f,	box_s, box_t,		normal[2][0], normal[2][1], normal[2][2],
			-0.25f, 0.0f, 0.25f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f,			normal[2][0], normal[2][1], normal[2][2],

			-0.25f, 0.0f, 0.25f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f,			normal[2][0], normal[2][1], normal[2][2],
			0.25f, 0.5f, 0.25f,		0.0f, 0.0f, 1.0f,	box_s, box_t,		normal[2][0], normal[2][1], normal[2][2],
			-0.25f, 0.5f, 0.25f,	0.0f, 0.0f, 1.0f,	0.0f, box_t,		normal[2][0], normal[2][1], normal[2][2],
			// 좌측면
			-0.25f, 0.0f, 0.25f,	1.0f, 0.0f, 1.0f,	box_s, 0.0f,		normal[3][0], normal[3][1], normal[3][2],
			-0.25f, 0.5f, 0.25f,	1.0f, 0.0f, 1.0f,	box_s, box_t,		normal[3][0], normal[3][1], normal[3][2],
			-0.25f, 0.0f, -0.25f,	1.0f, 0.0f, 1.0f,	0.0f, 0.0f,			normal[3][0], normal[3][1], normal[3][2],

			-0.25f, 0.0f, -0.25f,	1.0f, 0.0f, 1.0f,	0.0f, 0.0f,			normal[3][0], normal[3][1], normal[3][2],
			-0.25f, 0.5f, 0.25f,	1.0f, 0.0f, 1.0f,	box_s, box_t,		normal[3][0], normal[3][1], normal[3][2],
			-0.25f, 0.5f, -0.25f,	1.0f, 0.0f, 1.0f,	0.0f, box_t,		normal[3][0], normal[3][1], normal[3][2],
			// 바닥면
			-0.25f, 0.0f, 0.25f,	1.0f, 1.0f, 0.0f,	box_s, 0.0f,		normal[4][0], normal[4][1], normal[4][2],
			0.25f, 0.0f, -0.25f,	1.0f, 1.0f, 0.0f,	0.0f, box_t,		normal[4][0], normal[4][1], normal[4][2],
			0.25f, 0.0f, 0.25f,		1.0f, 1.0f, 0.0f,	0.0f, 0.0f,			normal[4][0], normal[4][1], normal[4][2],

			0.25f, 0.0f, -0.25f,	1.0f, 1.0f, 0.0f,	0.0f, box_t,		normal[4][0], normal[4][1], normal[4][2],
			-0.25f, 0.0f, 0.25f,	1.0f, 1.0f, 0.0f,	box_s, 0.0,			normal[4][0], normal[4][1], normal[4][2],
			-0.25f, 0.0f, -0.25f,	1.0f, 1.0f, 0.0f,	box_s, box_t,		normal[4][0], normal[4][1], normal[4][2],
			// 윗면
			-0.25f, 0.5f, -0.25f,	0.0f, 1.0f, 1.0f,	0.0f, box_t,		normal[5][0], normal[5][1], normal[5][2],
			0.25f, 0.5f, 0.25f,		0.0f, 1.0f, 1.0f,	box_s, 0.0f,		normal[5][0], normal[5][1], normal[5][2],
			0.25f, 0.5f, -0.25f,	0.0f, 1.0f, 1.0f,	box_s, box_t,		normal[5][0], normal[5][1], normal[5][2],

			0.25f, 0.5f, 0.25f,		0.0f, 1.0f, 1.0f,	box_s, 0.0f,		normal[5][0], normal[5][1], normal[5][2],
			-0.25f, 0.5f, -0.25f,	0.0f, 1.0f, 1.0f,	0.0f, box_t,		normal[5][0], normal[5][1], normal[5][2],
			-0.25f, 0.5f, 0.25f,	0.0f, 1.0f, 1.0f,	0.0f, 0.0f,			normal[5][0], normal[5][1], normal[5][2],
		};

		// VBO를 생성하여 vertices 값들을 복사
		glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(box_vertices), box_vertices, GL_STATIC_DRAW);

		// VBO를 나누어서 각 버텍스 속성으로 연결
		// 위치 속성 (location = 0)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// 컬러 속성 (location = 1)
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// 텍스처 좌표 속성 (location = 2)
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
		// normal vector 속성 (location = 3)
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
		glEnableVertexAttribArray(3);

		// VBO 및 버텍스 속성을 다 했으니 VBO와 VAO를 unbind한다.
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		// 텍스처 객체 만들고 바인딩 
		load_texture(textures[1], "../src/brick.jpg");

		// light box
		glBindVertexArray(VAOs[2]);
		GLfloat Light_vertices[] = {
			// position			
			// 뒷면
			-0.25f, 0.5f, -0.25f,
			0.25f, 0.0f, -0.25f,
			-0.25f, 0.0f, -0.25f,

			0.25f, 0.0f, -0.25f,
			-0.25f, 0.5f, -0.25f,
			0.25f, 0.5f, -0.25f,
			// 우측면
			0.25f, 0.0f, -0.25f,
			0.25f, 0.5f, -0.25f,
			0.25f, 0.0f, 0.25f,	

			0.25f, 0.0f, 0.25f,	
			0.25f, 0.5f, -0.25f,
			0.25f, 0.5f, 0.25f,	
			// 정면
			0.25f, 0.0f, 0.25f,	
			0.25f, 0.5f, 0.25f,	
			-0.25f, 0.0f, 0.25f,

			-0.25f, 0.0f, 0.25f,
			0.25f, 0.5f, 0.25f,	
			-0.25f, 0.5f, 0.25f,
			// 좌측면
			-0.25f, 0.0f, 0.25f,
			-0.25f, 0.5f, 0.25f,
			-0.25f, 0.0f, -0.25f,

			-0.25f, 0.0f, -0.25f,
			-0.25f, 0.5f, 0.25f,
			-0.25f, 0.5f, -0.25f,
			// 바닥면
			-0.25f, 0.0f, 0.25f,
			0.25f, 0.0f, -0.25f,
			0.25f, 0.0f, 0.25f,	

			0.25f, 0.0f, -0.25f,
			-0.25f, 0.0f, 0.25f,
			-0.25f, 0.0f, -0.25f,
			// 윗면
			-0.25f, 0.5f, -0.25f,
			0.25f, 0.5f, 0.25f,	
			0.25f, 0.5f, -0.25f,

			0.25f, 0.5f, 0.25f,	
			-0.25f, 0.5f, -0.25f,
			-0.25f, 0.5f, 0.25f,
		};

		// VBO를 생성하여 vertices 값들을 복사
		glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Light_vertices), Light_vertices, GL_STATIC_DRAW);

		// VBO를 나누어서 각 버텍스 속성으로 연결
		// 위치 속성 (location = 0)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// VBO 및 버텍스 속성을 다 했으니 VBO와 VAO를 unbind한다.
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	
	// 렌더링 virtual 함수를 작성해서 오버라이딩한다.
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

		// 카메라 매트릭스 계산
		float distance = 2.f;
		vmath::vec3 eye((float)cos(currentTime * 0.1f) * distance, 1.f, (float)sin(currentTime * 0.1f) * distance);
		// vmath::vec3 eye(0.f, 2.f, 3.f);
		vmath::vec3 center(0.0, 0.0, 0.0);
		vmath::vec3 up(0.0, 1.0, 0.0);
		vmath::mat4 lookAt = vmath::lookat(eye, center, up);
		float fov = 50.f;// (float)cos(currentTime)*20.f + 50.0f;
		vmath::mat4 projM = vmath::perspective(fov, info.windowWidth / info.windowHeight, 0.1f, 1000.0f);

		// 바닥 그리기 ---------------------------------------
		glUseProgram(shader_programs[0]);
		glUniformMatrix4fv(uniform_transform1, 1, GL_FALSE, projM * lookAt * vmath::scale(1.5f));
		glUniform1i(glGetUniformLocation(shader_programs[0], "texture1"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glBindVertexArray(VAOs[0]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// 정육면체 그리기 --------------------------------------- 1
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

		// 정육면체 그리기 --------------------------------------- 2
		shininess = 1;
		vmath::mat4 model_2 = vmath::translate(-1.f, 0.f, 0.f) * model;
		glUniformMatrix4fv(uniform_transform2, 1, GL_FALSE, model_2);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// 정육면체 그리기 --------------------------------------- 3
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

// DECLARE_MAIN의 하나뿐인 인스턴스
DECLARE_MAIN(my_application)