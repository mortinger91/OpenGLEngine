#pragma once

#include <memory>
#include <string>
#include "Texture.h"
#include "Shader.h"

class Material
{
public:
	Material(const std::string& texturePath, float _m_Shininess, float _m_Specular, float _m_Diffuse, float _m_Ambient, std::shared_ptr<Shader> shader);
	~Material();

	std::shared_ptr<Shader> m_Shader;

	void Bind() const;
	void UpdateUniforms(float shininess, float specular, float diffuse, float ambient);

	float m_Shininess, m_Specular, m_Diffuse, m_Ambient;
private:
	Texture m_Texture;
	void SetUniforms() const;
};
