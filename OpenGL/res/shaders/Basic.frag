#version 330 core

// input from the vertex shader
in vec3 v_Color;
in vec3 v_Normal;
in vec4 v_Vertex;

// output this color for each pixel to the framebuffer
out vec4 color;

// uniform: inputs from the cpu
// Assume light 0 is directional, light 1 is a point light.
// The actual light values are passed from the main OpenGL program.
// This could of course be fancier.  My goal is to illustrate a simple idea.
uniform int islight;
uniform vec3 light0dirn; 
uniform vec4 light0color; 
uniform vec4 light1posn; 
uniform vec4 light1color; 

// Now, set the material parameters.  These could be bound to
// a buffer.  But for now, I'll just make them uniform.
// I use ambient, diffuse, specular, shininess.
// But, the ambient is just additive and doesn't multiply the lights.  
uniform vec4 ambient; 
uniform vec4 diffuse; 
uniform vec4 specular; 
uniform float shininess; 

vec4 ComputeLight (const in vec3 direction, const in vec4 lightcolor, const in vec3 normal, const in vec3 halfvec, const in vec4 mydiffuse, const in vec4 myspecular, const in float myshininess)
{
	float nDotL = dot(normal, direction);  
	vec4 lambert = mydiffuse * lightcolor * max (nDotL, 0.0);

	float nDotH = dot(normal, halfvec);
	vec4 phong = myspecular * lightcolor * pow (max(nDotH, 0.0), myshininess);

	vec4 retval = lambert + phong;
	return retval;
}    

// execute this for every fragment we are drawing
void main()
{
	if(islight == 1)
		{
		// They eye is always at (0,0,0) looking down -z axis 
		// Also compute current fragment position and direction to eye 

		const vec3 eyepos = vec3(0,0,0);
		vec3 mypos = v_Vertex.xyz / v_Vertex.w ; // Dehomogenize current location
		vec3 eyedirn = normalize(eyepos - mypos);

		// Compute normal, needed for shading.
		vec3 normal = normalize(v_Normal);

		// Light 0, directional
		vec3 direction0 = normalize (light0dirn);
		vec3 half0 = normalize (direction0 + eyedirn);
		vec4 col0 = ComputeLight(direction0, light0color, normal, half0, diffuse, specular, shininess);

		// Light 1, point 
	    vec3 position = light1posn.xyz / light1posn.w;
	    vec3 direction1 = normalize (position - mypos); // no attenuation
	    vec3 half1 = normalize (direction1 + eyedirn);
	    vec4 col1 = ComputeLight(direction1, light1color, normal, half1, diffuse, specular, shininess);
	
		color = vec4(v_Color, 1.0f) * (ambient + col0 + col1);
	}
	else
	{
		color = vec4(v_Color, 1.0f);
	}
//    color = vec4(v_Color[0], v_Color[1], v_Color[2], 1.0f);
};