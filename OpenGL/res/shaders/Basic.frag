#version 330 core

// input of the texture coordinates from the vertex shader 
//in vec2 v_TexCoord;
in vec3 v_Color;
in vec3 v_Normal;

// output this color for each pixel to the framebuffer
out vec4 color;

// takes this uniform as input from the cpu
uniform sampler2D u_Texture;
//uniform int u_UseTexture;
//uniform float u_ChangingColor;

// execute this for every fragment we are drawing
void main()
{
	// u_Texture = (sampler) Specifies the sampler to which the texture from which texels will be retrieved is bound.
	// v_TexCoord = (P) Specifies the texture coordinates at which texture will be sampled.
//	vec4 texColor = texture(u_Texture, v_TexCoord);
	//if (u_UseTexture == 1)
//		color = texColor;
	//else
        //color = vec4(1.0, 1.0, 1.0, 1.0);
		//color = vec4(u_ChangingColor, 0.0, 0.0 , 1.0);

	color = vec4(v_Color[0], v_Color[1], v_Color[2], 1.0f);

};