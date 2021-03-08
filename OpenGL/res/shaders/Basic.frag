#version 330 core

// output this color for each pixel to the framebuffer
layout(location = 0) out vec4 color;

// input of the texture coordinates from the fragment shader 
in vec2 v_TexCoord;

// takes this uniform as input from the cpu
uniform sampler2D u_Texture;

// execute this for every fragment we are drawing
void main()
{
	// u_Texture = (sampler) Specifies the sampler to which the texture from which texels will be retrieved is bound.
	// v_TexCoord = (P) Specifies the texture coordinates at which texture will be sampled.
	vec4 texColor = texture(u_Texture, v_TexCoord);
	//color = texColor;
    color = vec4(1.0, 1.0, 1.0, 1.0);;
};