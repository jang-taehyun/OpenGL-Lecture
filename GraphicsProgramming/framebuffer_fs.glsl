#version 430 core							
											
in vec3 vsColor;							
in vec2 vsTexCoord;							
uniform sampler2D texture1;					
											
out vec4 fragColor;							
											
void main(void)								
{											
	fragColor = texture(texture1, vsTexCoord);
}											