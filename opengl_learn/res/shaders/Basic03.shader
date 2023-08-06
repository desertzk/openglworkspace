#shader vertex
#version 330 core
//glVertexAttribPointer(0
layout(location = 0) in vec4 position;
//glVertexAttribPointer(1
layout(location = 1) in vec2 texCoord;


// model view projection
uniform mat4 u_MVP;

void main()
{
	gl_Position = position;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;



//uniform vec4 u_Color;
uniform sampler2D u_Texture;


void main()
{


//for class 8
	color = vec4(0.2, 0.3, 0.8, 1.0);
	
//for class 15 
	//color = u_Color;
};