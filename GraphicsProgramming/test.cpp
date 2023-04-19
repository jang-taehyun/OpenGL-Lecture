#include <sb7.h>
#include <vmath.h>
#include <shader.h>

class tmp : public sb7::application
{
private:
	GLuint ShaderProgram;				// shader program
	GLuint VertexArrayObject;			// vertex array object(VAO)
	GLuint VertexBufferObject;			// vertex buffer object(VBO)

public:
	tmp() : ShaderProgram(0), VertexBufferObject(0), VertexArrayObject(0) {}

	GLuint CompileShader()
	{
		GLuint vertexShader;
		GLuint fragmentShader;

		vertexShader = sb7::shader::load("./test_vs.glsl", GL_VERTEX_SHADER);
		fragmentShader = sb7::shader::load("./test_fs.glsl", GL_FRAGMENT_SHADER);

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

		// define cube position and color
		const GLfloat vertices[] = {
							// front-upward
							0.25f, 0.25f, 0.25f, 1.f, 0.f, 0.f,			// v1 position and color
							-0.25f, 0.25f, 0.25f, 1.f, 0.f, 0.f,		// v2 position and color
							0.25f, -0.25f, 0.25f, 1.f, 0.f, 0.f,		// v3 position and color

							// front-downward
							-0.25f, 0.25f, 0.25f, 1.f, 0.f, 0.f,		// v1 position and color
							-0.25f, -0.25f, 0.25f, 1.f, 0.f, 0.f,		// v2 position and color
							0.25f, -0.25f, 0.25f, 1.f, 0.f, 0.f,		// v3 position and color

							// right-upward
							0.25f, 0.25f, 0.25f, 0.f, 0.f, 1.f,			// v1 position and color
							0.25f, -0.25f, -0.25f, 0.f, 0.f, 1.f,		// v2 position and color
							0.25f, 0.25f, -0.25f, 0.f, 0.f, 1.f,		// v3 position and color

							// right-downward
							0.25f, 0.25f, 0.25f, 0.f, 0.f, 1.f,			// v1 position and color
							0.25f, -0.25f, 0.25f, 0.f, 0.f, 1.f,		// v2 position and color
							0.25f, -0.25f, -0.25f, 0.f, 0.f, 1.f,		// v3 position and color

							// up-upward
							0.25f, 0.25f, 0.25f, 0.f, 1.f, 0.f,			// v1 position and color
							0.25f, 0.25f, -0.25f, 0.f, 1.f, 0.f,		// v2 position and color
							-0.25f, 0.25f, -0.25f, 0.f, 1.f, 0.f,		// v3 position and color

							// up-downward
							0.25f, 0.25f, 0.25f, 0.f, 1.f, 0.f,			// v1 position and color
							-0.25f, 0.25f, -0.25f, 0.f, 1.f, 0.f,		// v2 position and color
							-0.25f, 0.25f, 0.25f, 0.f, 1.f, 0.f,		// v3 position and color

							// back-upward
							-0.25f, 0.25f, -0.25f, 1.f, 1.f, 0.f,		// v1 position and color
							0.25f, 0.25f, -0.25f, 1.f, 1.f, 0.f,		// v2 position and color
							0.25f, -0.25f, -0.25f, 1.f, 1.f, 0.f,		// v3 position and color

							// back-downward
							-0.25f, 0.25f, -0.25f, 1.f, 1.f, 0.f,		// v1 position and color
							0.25f, -0.25f, -0.25f, 1.f, 1.f, 0.f,		// v2 position and color
							-0.25f, -0.25f, -0.25f, 1.f, 1.f, 0.f,		// v3 position and color

							// left-upward
							-0.25f, 0.25f, 0.25f, 1.f, 0.f, 1.f,		// v1 position and color
							-0.25f, 0.25f, -0.25f, 1.f, 0.f, 1.f,		// v2 position and color
							-0.25f, -0.25f, -0.25f, 1.f, 0.f, 1.f,		// v3 position and color

							// left-downward
							-0.25f, 0.25f, 0.25f, 1.f, 0.f, 1.f,		// v1 position and color
							-0.25f, -0.25f, -0.25f, 1.f, 0.f, 1.f,		// v2 position and color
							-0.25f, -0.25f, 0.25f, 1.f, 0.f, 1.f,		// v3 position and color

							// down-upward
							-0.25f, -0.25f, -0.25f, 0.f, 1.f, 1.f,		// v1 position and color
							0.25f, -0.25f, -0.25f, 0.f, 1.f, 1.f,		// v2 position and color
							0.25f, -0.25f, 0.25f, 0.f, 1.f, 1.f,		// v3 position and color

							// down-downward
							-0.25f, -0.25f, -0.25f, 0.f, 1.f, 1.f,		// v1 position and color
							0.25f, -0.25f, 0.25f, 0.f, 1.f, 1.f,		// v2 position and color
							-0.25f, -0.25f, 0.25f, 0.f, 1.f, 1.f		// v3 position and color
		};

		// generate VBO and move data to VBO
		glGenBuffers(1, &VertexBufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// define connecting setting and connect VBO to vertex attributes
		// position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
		glEnableVertexAttribArray(0);
		// color
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		// unbind VBO and VAO
		glBindBuffer(GL_ARRAY_BUFFER, 0);		// VBO unbind
		glBindVertexArray(0);					// VAO unbind
	}

	// rendering loop
	virtual void render(double currentTime)
	{
		glEnable(GL_CULL_FACE);

		// clean framebuffer
		const GLfloat background[] = { 0.f,		// R
									   0.f,		// G
									   0.f,		// B
									   1.f		// A
		};
		glClearBufferfv(GL_COLOR, 0, background);

		// active shader program
		glUseProgram(ShaderProgram);

		// define matrix //
		// define rotate matrix about xyz axis
		float angle = currentTime * 50.f;
		vmath::mat4 RotateMat = vmath::rotate(angle, 0.f, 1.f, 0.f);
		// define translate matrix about sin(currentTime)
		float moveDistanceX = sin(currentTime);
		float moveDistanceY = 0.f;
		float moveDistanceZ = cos(currentTime);
		vmath::mat4 TranslateMat = vmath::translate(moveDistanceX, moveDistanceY, moveDistanceZ);
		// define view matrix
		vmath::mat4 ViewMat = vmath::lookat(
										vmath::vec3(0.f, -1.f, 3.f),
										vmath::vec3(0.f, 0.f, 0.f),
										vmath::vec3(0.f, 1.f, 0.f)
		);
		// define projection matrix about camera
		vmath::mat4 projectionMat = vmath::perspective(50.0f, info.windowWidth / info.windowHeight, 0.1f, 1000.0f);

		// calculate transMat
		vmath::mat4 TransMat = projectionMat * ViewMat * TranslateMat * RotateMat;

		// connet uniform variable to matrix //

		// get uniform variable 'transMat' location
		GLint TransMatLocation = glGetUniformLocation(ShaderProgram, "transMat");
		// deliver to 'transMat' variable
		glUniformMatrix4fv(TransMatLocation, 1, GL_FALSE, TransMat);

		// bind VAO
		glBindVertexArray(VertexArrayObject);

		// render
		glDrawArrays(GL_TRIANGLES, 0, 3 * 2 * 6);

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