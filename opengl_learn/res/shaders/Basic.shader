#shader vertex
#version 330 core
//glVertexAttribPointer(0
layout(location = 0) in vec4 position;
//glVertexAttribPointer(1
layout(location = 1) in vec2 texCoord;

//output texture form vertex shader into fragment shader
out vec2 v_TexCoord;
// model view projection
uniform mat4 u_MVP; // we set this variable in the OpenGL code.

void main()
{
	gl_Position = u_MVP * position;
	v_TexCoord = texCoord;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;// the input variable from the vertex shader (same name and same type)

//uniform vec4 u_Color;
uniform sampler2D u_Texture;


void main()
{
//for class 17
	vec4 texColor = texture(u_Texture, v_TexCoord);
	color = texColor;

//for class 8
	//color = vec4(0.2, 0.3, 0.8, 1.0);
	
//for class 15 
	//color = u_Color;
};
