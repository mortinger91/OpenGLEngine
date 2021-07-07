#version 330 core

// takes data from the bound vertex buffer
layout(location = 0) in vec3 position; 
//layout(location = 1) in vec3 color;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

// output to the fragment shader
//out vec3 v_Color;
out vec3 v_Normal;
out vec4 v_Vertex;
out vec2 v_TexCoord;

// uniform: inputs from the cpu
uniform mat4 u_MVP;
uniform mat4 u_MV;

// execute this for every vertex we are drawing
void main()
{
	// position of the vertex on the screen in NDC coordinates
	gl_Position = u_MVP * vec4(position, 1.f);
	//v_Color = color;
	// TODO: move mat3(transpose(inverse(u_MV))) to a uniform
	v_Normal = mat3(transpose(inverse(u_MV))) * normal;
	v_Vertex = u_MV * vec4(position, 1.f);
	v_TexCoord = texCoord;
};