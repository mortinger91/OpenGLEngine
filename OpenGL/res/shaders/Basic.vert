#version 330 core

// takes data from the bound vertex buffer
layout(location = 0) in vec4 position; // the position variable has attribute position 0
//layout(location = 1) in vec2 texCoord; // the texture position variable has attribute position 1
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;

// output to the fragment shader the texture coordinates
//out vec2 v_TexCoord; // output a color to the fragment shader
out vec3 v_Color; // output a color to the fragment shader
out vec3 v_Normal;

// takes this uniform as input from the cpu
uniform mat4 u_MVP;

// execute this for every vertex we are drawing
void main()
{
	// position of the vertex on the screen in NDC coordinates
	gl_Position = u_MVP * position;
//	v_TexCoord = texCoord;
	v_Color = color;
	v_Normal = normal;
};