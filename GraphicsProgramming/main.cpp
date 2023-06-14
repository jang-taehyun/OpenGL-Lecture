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
	GLuint shader_programs[5];
	GLuint VAOs[4], VBOs[3], EBOs[2];
	GLuint textures[3];
	GLuint FBO;								// frame buffer
	GLuint FBO_texture;
	GLuint RBO;								// render buffer

	std::vector<vmath::vec3> boxPositions;

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

	void load_texture(GLuint textureID, char const* filename, GLenum GL_ColorFormat = GL_RGB, GLenum GL_InputFormat = GL_RGB)						// -> *filename 변경 불가능 && fileane은 변경 가능
	{
		glBindTexture(GL_TEXTURE_2D, textureID);

		int width, height, nrChannels;
		unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);

		if (data) {
			// 앞에 format : 넘길 데이터의 채널
			// 뒤에 format : 
			glTexImage2D(GL_TEXTURE_2D, 0, GL_ColorFormat, width, height, 0, GL_InputFormat, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		stbi_image_free(data);

		// 텍스처 샘플링/필터링 설정
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	virtual void startup()
	{
		stbi_set_flip_vertically_on_load(true);

		shader_programs[0] = compile_shader("./texture_vs.glsl", "./texture_fs.glsl");
		shader_programs[1] = compile_shader("./vs.glsl", "./fs.glsl");
		shader_programs[2] = compile_shader("./simple_color_vs.glsl", "./simple_color_fs.glsl");
		shader_programs[3] = compile_shader("./framebuffer_vs.glsl", "./framebuffer_fs.glsl");
		shader_programs[4] = compile_shader("./screen_filtering_vs.glsl", "./screen_filtering_fs.glsl");

		// VAO, VBO, EBO, texture 생성
		glGenVertexArrays(4, VAOs);
		glGenBuffers(3, VBOs);
		glGenBuffers(2, EBOs);
		glGenTextures(3, textures);

		glGenFramebuffers(1, &FBO);
		glGenRenderbuffers(1, &RBO);

		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glGenTextures(1, &FBO_texture);
		glBindTexture(GL_TEXTURE_2D, FBO_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, info.windowWidth, info.windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FBO_texture, 0);

		glBindRenderbuffer(GL_RENDERBUFFER, RBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, info.windowWidth, info.windowHeight);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			glfwTerminate();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// 첫 번째 객체 정의 : 바닥 --------------------------------------------------
		glBindVertexArray(VAOs[0]);
		// 바닥 점들의 위치와 컬러, 텍스처 좌표를 정의한다.
		float floor_s = 5.0f, floor_t = 5.0f;
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
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(floor_indices), floor_indices, GL_STATIC_DRAW);

		// VBO 및 버텍스 속성을 다 했으니 VBO와 VAO를 unbind한다.
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		// 텍스처 객체 만들고 바인딩
		load_texture(textures[0], "../src/wall.jpg");
		// 텍스처 객체 만들고 바인딩 
		load_texture(textures[1], "../src/container2.png", GL_RGBA, GL_RGBA);
		// 텍스처 객체 만들고 바인딩 
		load_texture(textures[2], "../src/container2_specular.png", GL_RGBA, GL_RGBA);

		// 두 번째 객체 정의 : 박스 --------------------------------------------------
		glBindVertexArray(VAOs[1]);
		// 박스 점들의 위치와 컬러, 텍스처 좌표를 정의한다.
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

		// light box
		glBindVertexArray(VAOs[2]);
		// 피라미드 점들의 위치와 컬러, 텍스처 좌표를 정의한다.
		GLfloat pyramid_vertices[] = {
			1.0f, 0.0f, -1.0f,    // 우측 상단
			-1.0f, 0.0f, -1.0f,   // 좌측 상단
			-1.0f, 0.0f, 1.0f,    // 좌측 하단
			1.0f, 0.0f, 1.0f,     // 우측 하단
			0.0f, 1.0f, 0.0f,      // 상단 꼭지점
			0.0f, -1.0f, 0.0f,      // 하단 꼭지점
		};

		// 삼각형으로 그릴 인덱스를 정의한다.
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

		// VBO를 생성하여 vertices 값들을 복사
		glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid_vertices), pyramid_vertices, GL_STATIC_DRAW);

		// VBO를 나누어서 각 버텍스 속성으로 연결
		// 위치 속성 (location = 0)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// EBO를 생성하고 indices 값들을 복사
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pyramid_indices), pyramid_indices, GL_STATIC_DRAW);

		// VBO 및 버텍스 속성을 다 했으니 VBO와 VAO를 unbind한다.
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		// 박스 10개 포지션 설정
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

		//  Post-processing용 스크린 정의 --------------------------------------------------
		GLfloat screenVertices[] = {
			//NDC xy좌표 //텍스처 UV
			-1.0f, 1.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f,
			1.0f, -1.0f, 1.0f, 0.0f,

			-1.0f, 1.0f, 0.0f, 1.0f,
			1.0f, -1.0f, 1.0f, 0.0f,
			1.0f, 1.0f, 1.0f, 1.0f
		};

		
		glBindVertexArray(VAOs[3]);
		glBindBuffer(GL_ARRAY_BUFFER, VAOs[3]);

		glBufferData(GL_ARRAY_BUFFER, sizeof(screenVertices), screenVertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	// 렌더링 virtual 함수를 작성해서 오버라이딩한다.
	virtual void render(double currentTime)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);

		const GLfloat color[] = { (float)sin(currentTime) * 0.5f + 0.5f, (float)cos(currentTime) * 0.5f + 0.5f, 0.0f, 1.0f };
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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

		// 카메라 매트릭스 계산
		float distance = 2.f;
		vmath::vec3 eye((float)cos(currentTime * 0.1f) * distance, 1.0, (float)sin(currentTime * 0.1f) * distance);
		vmath::vec3 center(0.0, 0.0, 0.0);
		vmath::vec3 up(0.0, 1.0, 0.0);
		vmath::mat4 lookAt = vmath::lookat(eye, center, up);
		float fov = 50.f;
		vmath::mat4 projM = vmath::perspective(fov, (float)info.windowWidth / info.windowHeight, 0.1f, 1000.0f);

		// 정육면체 그리기 --------------------------------------- 1
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
		vmath::vec3 pointLightPos[2] = {
			vmath::vec3((float)sin(currentTime * 0.5f) * -1.f, -0.25f, (float)cos(currentTime * 0.5f) * -0.7f),
			vmath::vec3(-1.f, 0.f, 0.f)
		};

		glUniform3f(glGetUniformLocation(shader_programs[1], "dirLight.direction"), -1.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(shader_programs[1], "dirLight.ambient"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(shader_programs[1], "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
		glUniform3f(glGetUniformLocation(shader_programs[1], "dirLight.specular"), 0.5f, 0.5f, 0.5f);

		glUniform3fv(glGetUniformLocation(shader_programs[1], "pointLights[0].position"), 1, pointLightPos[0]);
		glUniform3f(glGetUniformLocation(shader_programs[1], "pointLights[0].ambient"), 0.1f, 0.1f, 0.1f);
		glUniform3f(glGetUniformLocation(shader_programs[1], "pointLights[0].diffuse"), 1.f, 1.f, 1.f);
		glUniform3f(glGetUniformLocation(shader_programs[1], "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(shader_programs[1], "pointLights[0].c1"), 0.045f);
		glUniform1f(glGetUniformLocation(shader_programs[1], "pointLights[0].c2"), 0.0075f);

		glUniform3fv(glGetUniformLocation(shader_programs[1], "pointLights[1].position"), 1, pointLightPos[1]);
		glUniform3f(glGetUniformLocation(shader_programs[1], "pointLights[1].ambient"), 0.1f, 0.1f, 0.1f);
		glUniform3f(glGetUniformLocation(shader_programs[1], "pointLights[1].diffuse"), 1.f, 1.f, 1.f);
		glUniform3f(glGetUniformLocation(shader_programs[1], "pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(shader_programs[1], "pointLights[1].c1"), 0.045f);
		glUniform1f(glGetUniformLocation(shader_programs[1], "pointLights[1].c2"), 0.0075f);

		glUniform3fv(glGetUniformLocation(shader_programs[1], "spotLight.position"), 1, eye);
		glUniform3fv(glGetUniformLocation(shader_programs[1], "spotLight.direction"), 1, center - eye);
		glUniform1f(glGetUniformLocation(shader_programs[1], "spotLight.cutOff"), (float)cos(vmath::radians(12.5)));
		glUniform1f(glGetUniformLocation(shader_programs[1], "spotLight.outerCutOff"), (float)cos(vmath::radians(15.5)));
		glUniform1f(glGetUniformLocation(shader_programs[1], "spotLight.c1"), 0.045f);
		glUniform1f(glGetUniformLocation(shader_programs[1], "spotLight.c2"), 0.0075f);
		glUniform3f(glGetUniformLocation(shader_programs[1], "spotLight.ambient"), 0.2f, 0.2f, 0.2f);
		glUniform3f(glGetUniformLocation(shader_programs[1], "spotLight.diffuse"), 0.8f, 0.8f, 0.8f);
		glUniform3f(glGetUniformLocation(shader_programs[1], "spotLight.specular"), 1.0f, 1.0f, 1.0f);

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
		// 광원 그리기 ---------------------------------------
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

		glUniform3fv(glGetUniformLocation(shader_programs[2], "color"), 1, lightColor);
		glUniformMatrix4fv(glGetUniformLocation(shader_programs[2], "projection"), 1, GL_FALSE, projM);
		glUniformMatrix4fv(glGetUniformLocation(shader_programs[2], "view"), 1, GL_FALSE, lookAt);
		glUniformMatrix4fv(glGetUniformLocation(shader_programs[2], "model"), 1, GL_FALSE, transform);

		glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);

		// 윈도우 Framebuffer 바인딩 ----------------------------------------
		glDisable(GL_DEPTH_TEST);
		// 버퍼들의 값 지우기
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		// FBO Texture를 쉐이더 프로그램에 연결
		glUseProgram(shader_programs[3]);
		//vmath::mat4 rotateM = vmath::rotate(angle, 0.0f, 1.0f, 0.0f);
		//glUniformMatrix4fv(uniform_transform1, 1, GL_FALSE, projM * lookAt * rotateM * vmath::scale(1.3f) * vmath::translate(0.0f, -0.25f, 0.0f));
		// glUniform1i(glGetUniformLocation(shader_programs[3], "texture1"), 0);
		glUniform1i(glGetUniformLocation(shader_programs[3], "screenTexture"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, FBO_texture);
		glBindVertexArray(VAOs[3]);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glUseProgram(0);

		// 기본 Framebuffer로 되돌리기
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

// DECLARE_MAIN의 하나뿐인 인스턴스
DECLARE_MAIN(my_application)