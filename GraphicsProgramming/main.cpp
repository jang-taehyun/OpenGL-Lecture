/*
* 하드코딩의 단점 : mesh마다 다른 program을 만들어야 하기때문에 비효율적이다.
* vertex attribute는 fragment shader에서 접근 불가능
* 
* VAO는 설정만 저장하고, VBO는 변경된 data를 저장한다.
*/

#define STB_IMAGE_IMPLEMENTATION
#include <sb7.h>
#include <vmath.h>
#include <shader.h>
#include "stb_image.h"

class tmp : public sb7::application
{
private:
	GLuint ShaderProgram;				// shader program
	GLuint VertexArrayObject;			// vertex array object(VAO)
	GLuint VertexBufferObject;			// vertex buffer object(VBO)
	GLuint ElementBufferObject;			// element buffer object(EBO)
	GLuint texture;						// texture

public:
	tmp() : ShaderProgram(0), VertexArrayObject(0), VertexBufferObject(0) { }

	GLuint CompileShader()
	{
		GLuint vertexShader;
		GLuint fragmentShader;

		vertexShader = sb7::shader::load("./vs.glsl", GL_VERTEX_SHADER);
		fragmentShader = sb7::shader::load("./fs.glsl", GL_FRAGMENT_SHADER);

		GLuint shaderProgram;
		shaderProgram = glCreateProgram();

		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);

		glLinkProgram(shaderProgram);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		return shaderProgram;
	}

	// application initialize
	virtual void startup()
	{
		ShaderProgram = CompileShader();

		glGenVertexArrays(1, &VertexArrayObject);
		glBindVertexArray(VertexArrayObject);

		// define triangle position and color
		const GLfloat vertices[] = {
							// position				// color			// texture position
							0.25f, 0.25f, 0.5f,		1.f, 0.f, 0.f,		1.f, 1.f,		// v0 position and color
							-0.25f, 0.25f, 0.5f,	0.f, 1.f, 0.f,		0.f, 1.f,		// v1 position and color
							-0.25f, -0.25f, 0.5f,	0.f, 0.f, 1.f,		0.f, 0.f,		// v2 position and color
							0.25f, -0.25f, 0.5f,	1.f, 1.f, 0.f,		1.f, 0.f		// v3 position and color		
		};

		// define triangle's index
		GLuint indices[] = {
							0, 1, 2,								// first triangle
							0, 2, 3									// second triangle
		};
		
		// generate VBO and move data to VBO
		glGenBuffers(1, &VertexBufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// define connecting setting and connect VBO to vertex attributes
		// position (location = 0)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
		glEnableVertexAttribArray(0);
		// color (location = 1)
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		// texture position (location = 2)
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

		// genearate EBO and move data to EBO
		glGenBuffers(1, &ElementBufferObject);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBufferObject);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// generate texture object and bind texture object
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		// load texture image
		int width, height, nrChannels;
		unsigned char* data = stbi_load("../src/wall.jpg", &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			// create mipmap
			glGenerateMipmap(GL_TEXTURE_2D);
		}

		// de-allocate texture image
		stbi_image_free(data);

		// Set texture sampling and filtering
		// texture wrapping setting
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// texture filtering setting
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// unbind VBO and VAO
		glBindTexture(GL_TEXTURE_2D, 0);			// texture unbind
		glBindVertexArray(0);						// VAO unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);			// VBO unbind
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);	// EBO unbind
	}

	// rendering loop
	virtual void render(double currentTime)
	{
		// clean framebuffer
		const GLfloat background[] = { 0.f,		// R
									   0.f,		// G
									   0.f,		// B
									   1.f		// A
		};
		glClearBufferfv(GL_COLOR, 0, background);

		// active shader program
		glUseProgram(ShaderProgram);
		
		// define rotate matrix about z axis
		float angle = currentTime * 50.f;
		vmath::mat4 RotateMat = vmath::rotate(angle, 0.f, 0.f, 1.f);
		// get uniform variable 'rotMat' location
		GLint RotMatLocation = glGetUniformLocation(ShaderProgram, "rotMat");
		// deliver to 'rotMat' variable
		glUniformMatrix4fv(RotMatLocation, 1, GL_FALSE, RotateMat);

		glUniform1i(glGetUniformLocation(ShaderProgram, "texture1"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		// bind VAO
		glBindVertexArray(VertexArrayObject);

		// render
		// glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// unbind VAO
		glBindVertexArray(0);

		// deactive shader program
		glUseProgram(0);
	}

	virtual void shutdown()
	{
		glDeleteVertexArrays(1, &VertexArrayObject);
		glDeleteProgram(ShaderProgram);
	}
};

DECLARE_MAIN(tmp)