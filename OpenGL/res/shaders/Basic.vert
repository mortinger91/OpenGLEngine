#version 330 core

// takes data from the bound vertex buffer
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

// output to the fragment shader the texture coordinates
out vec2 v_TexCoord;

// takes this uniform as input from the cpu
uniform mat4 u_MVP;

// execute this for every vertex we are drawing
void main()
{
	// position of the vertex on the screen in NDC coordinates
	gl_Position = u_MVP * position;
	v_TexCoord = texCoord;
};