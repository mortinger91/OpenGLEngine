namespace Cube
{
	// 3 x position, 3 x color, 3 x normal, 2 x texture coordinates
	static std::vector<glm::vec3> positions =
	{
		// front 
		glm::vec3( 1.0f,   1.0f,   1.0f),
		glm::vec3(-1.0f,   1.0f,   1.0f),
		glm::vec3(-1.0f,  -1.0f,   1.0f),
		glm::vec3( 1.0f,  -1.0f,   1.0f),

		// right
		glm::vec3( 1.0f,   1.0f,  -1.0f),
		glm::vec3( 1.0f,   1.0f,   1.0f),
		glm::vec3( 1.0f,  -1.0f,   1.0f),
		glm::vec3( 1.0f,  -1.0f,  -1.0f),
	
		// left
		glm::vec3(-1.0f,   1.0f,   1.0f),
		glm::vec3(-1.0f,   1.0f,  -1.0f),
		glm::vec3(-1.0f,  -1.0f,  -1.0f),
		glm::vec3(-1.0f,  -1.0f,   1.0f),
		
		// bot
		glm::vec3( 1.0f,  -1.0f,   1.0f),
		glm::vec3(-1.0f,  -1.0f,   1.0f),
		glm::vec3(-1.0f,  -1.0f,  -1.0f),
		glm::vec3( 1.0f,  -1.0f,  -1.0f),
		
		// top	
		glm::vec3( 1.0f,   1.0f,  -1.0f),
		glm::vec3(-1.0f,   1.0f,  -1.0f),
		glm::vec3(-1.0f,   1.0f,   1.0f),
		glm::vec3( 1.0f,   1.0f,   1.0f),
		
		// back
		glm::vec3(-1.0f,   1.0f,  -1.0f),
		glm::vec3( 1.0f,   1.0f,  -1.0f),
		glm::vec3( 1.0f,  -1.0f,  -1.0f),
		glm::vec3(-1.0f,  -1.0f,  -1.0f)
	};

	static std::vector<glm::vec3> colors =
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

	static std::vector<glm::vec3> normals =
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

	static std::vector<glm::vec2> texturesUnique =
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

	static std::vector<glm::vec2> texturesUVs =
	{
		// front
		glm::vec2(0.5f, 0.75f),  
		glm::vec2(0.25f, 0.75f), 
		glm::vec2(0.25f, 0.5f), 
		glm::vec2(0.5f, 0.5f),  
		
		// right
		glm::vec2(0.75f, 0.75f),  
		glm::vec2(0.5f, 0.75f), 
		glm::vec2(0.5f, 0.5f), 
		glm::vec2(0.75f, 0.5f), 
		
		// left
		glm::vec2(0.25f, 0.75f),  
		glm::vec2(0.f, 0.75f), 
		glm::vec2(0.f, 0.5f), 
		glm::vec2(0.25f, 0.5f),  
		
		// bot
		glm::vec2(0.5f, 0.5f),  
		glm::vec2(0.25f, 0.5f), 
		glm::vec2(0.25f, 0.25f), 
		glm::vec2(0.5f, 0.25f),
		
		// top
		glm::vec2(0.5f, 1.f),
		glm::vec2(0.25f, 1.f),
		glm::vec2(0.25f, 0.75f),
		glm::vec2(0.5f, 0.75f),
		
		// back
		glm::vec2(1.f, 0.75f),  
		glm::vec2(0.75f, 0.75f), 
		glm::vec2(0.75f, 0.5f), 
		glm::vec2(1.f, 0.5f)
	};

	static std::vector<unsigned int> indices =
	{
		0, 1, 3,  1, 2, 3, // front
		4, 5, 7,  5, 6, 7, // left
		8, 9, 11, 9, 10, 11, // right
		12, 13, 15, 13, 14, 15, // bot
		16, 17, 19, 17, 18, 19, // top
		20, 21, 23, 21, 22, 23 // back
	};
}