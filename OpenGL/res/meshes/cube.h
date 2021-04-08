namespace Cube
{
	// 3 x position, 3 x color, 3 x normal, 2 x texture coordinates
	std::vector<glm::vec3> positions =
	{
		// back 
		glm::vec3( 1.0f,   1.0f,   1.0f),
		glm::vec3(-1.0f,   1.0f,   1.0f),
		glm::vec3(-1.0f,  -1.0f,   1.0f),
		glm::vec3( 1.0f,  -1.0f,   1.0f),

		// right
		glm::vec3( 1.0f,   1.0f,   1.0f),
		glm::vec3( 1.0f,  -1.0f,   1.0f),
		glm::vec3( 1.0f,  -1.0f,  -1.0f),
		glm::vec3( 1.0f,   1.0f,  -1.0f),
	
		// left
		glm::vec3(-1.0f,   1.0f,   1.0f),
		glm::vec3(-1.0f,  -1.0f,   1.0f),
		glm::vec3(-1.0f,  -1.0f,  -1.0f),
		glm::vec3(-1.0f,   1.0f,  -1.0f),
		
		// bot
		glm::vec3( 1.0f,  -1.0f,   1.0f),
		glm::vec3(-1.0f,  -1.0f,   1.0f),
		glm::vec3( 1.0f,  -1.0f,  -1.0f),
		glm::vec3(-1.0f,  -1.0f,  -1.0f),
		
		// top	
		glm::vec3( 1.0f,   1.0f,   1.0f),
		glm::vec3(-1.0f,   1.0f,   1.0f),
		glm::vec3( 1.0f,   1.0f,  -1.0f),
		glm::vec3(-1.0f,   1.0f,  -1.0f),
		
		// front
		glm::vec3( 1.0f,   1.0f,  -1.0f),
		glm::vec3(-1.0f,   1.0f,  -1.0f),
		glm::vec3(-1.0f,  -1.0f,  -1.0f),
		glm::vec3( 1.0f,  -1.0f,  -1.0f)
	};

	std::vector<glm::vec3> colors =
	{           
		glm::vec3(0.5f,  0.3f,  0.1f),    
		glm::vec3(0.5f,  0.3f,  0.1f),    
		glm::vec3(0.5f,  0.3f,  0.1f),    
		glm::vec3(0.5f,  0.3f,  0.1f),    
				   		   
		glm::vec3(0.7f,  0.7f,  0.7f),    
		glm::vec3(0.7f,  0.7f,  0.7f),    
		glm::vec3(0.7f,  0.7f,  0.7f),    
		glm::vec3(0.7f,  0.7f,  0.7f),    
	     	   		       
		glm::vec3(1.0f,  0.5f,  0.31f),   
		glm::vec3(1.0f,  0.5f,  0.31f),   
		glm::vec3(1.0f,  0.5f,  0.31f),   
		glm::vec3(1.0f,  0.5f,  0.31f),   
 	   		       
		glm::vec3(1.f,   1.f,   0.01f),   
		glm::vec3(1.f,   1.f,   0.01f),   
		glm::vec3(1.f,   1.f,   0.01f),   
		glm::vec3(1.f,   1.f,   0.01f),   
	 	   		       
		glm::vec3(1.f,   0.01f,   1.f),   
		glm::vec3(1.f,   0.01f,   1.f),   
		glm::vec3(1.f,   0.01f,   1.f),   
		glm::vec3(1.f,   0.01f,   1.f),   
   	   		       
		glm::vec3(0.01f,  1.f,   1.f),    
		glm::vec3(0.01f,  1.f,   1.f),    
		glm::vec3(0.01f,  1.f,   1.f),    
		glm::vec3(0.01f,  1.f,   1.f)
	};

	std::vector<glm::vec3> normals =
	{
		glm::vec3(0.0f,  0.0f,  1.0f),
		glm::vec3(0.0f,  0.0f,  1.0f),
		glm::vec3(0.0f,  0.0f,  1.0f),
		glm::vec3(0.0f,  0.0f,  1.0f),
		   
		glm::vec3(1.0f,  0.0f,  0.0f),
		glm::vec3(1.0f,  0.0f,  0.0f),
		glm::vec3(1.0f,  0.0f,  0.0f),
		glm::vec3(1.0f,  0.0f,  0.0f),
		
		glm::vec3(-1.0f,  0.0f,  0.0f),
		glm::vec3(-1.0f,  0.0f,  0.0f),
		glm::vec3(-1.0f,  0.0f,  0.0f),
		glm::vec3(-1.0f,  0.0f,  0.0f),
		
		glm::vec3(0.0f, -1.0f,  0.0f),
		glm::vec3(0.0f, -1.0f,  0.0f),
		glm::vec3(0.0f, -1.0f,  0.0f),
		glm::vec3(0.0f, -1.0f,  0.0f),
		
		glm::vec3(0.0f,  1.0f,  0.0f),
		glm::vec3(0.0f,  1.0f,  0.0f),
		glm::vec3(0.0f,  1.0f,  0.0f),
		glm::vec3(0.0f,  1.0f,  0.0f),
		
		glm::vec3(0.0f,  0.0f, -1.0f),
		glm::vec3(0.0f,  0.0f, -1.0f),
		glm::vec3(0.0f,  0.0f, -1.0f),
		glm::vec3(0.0f,  0.0f, -1.0f)
	};

	std::vector<glm::vec2> textures =
	{
		glm::vec2(0.f, 0.f),  
		glm::vec2(1.f, 0.f), 
		glm::vec2(1.f, 1.f), 
		glm::vec2(0.f, 1.f),  
		
		glm::vec2(0.f, 0.f),
		glm::vec2(1.f, 0.f),
		glm::vec2(1.f, 1.f),
		glm::vec2(0.f, 1.f),
		
		glm::vec2(0.f, 0.f),
		glm::vec2(1.f, 0.f),
		glm::vec2(1.f, 1.f),
		glm::vec2(0.f, 1.f),
		
		glm::vec2(0.f, 0.f),
		glm::vec2(1.f, 0.f),
		glm::vec2(1.f, 1.f),
		glm::vec2(0.f, 1.f),
		
		glm::vec2(0.f, 0.f),
		glm::vec2(1.f, 0.f),
		glm::vec2(1.f, 1.f),
		glm::vec2(0.f, 1.f),
		
		glm::vec2(0.f, 0.f),
		glm::vec2(1.f, 0.f),
		glm::vec2(1.f, 1.f),
		glm::vec2(0.f, 1.f)
	};

	std::vector<unsigned int> indices =
	{
		0, 1, 3,  2, 1, 3, // front
		4, 7, 5,  6, 7, 5, // left
		8, 9, 11, 10, 9, 11, // right
		12, 13, 14, 15, 13, 14, // bot
		16, 17, 18, 19, 17, 18, //top
		20, 21, 23, 22, 21, 23 // back
	};
}