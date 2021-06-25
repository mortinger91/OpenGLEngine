#pragma once

#include <memory>
#include <string>
#include "Texture.h"
#include "Shader.h"

class Material
{
public:
	Material(const std::string& texturePath, float _m_Shininess, float _m_Specular, float _m_Diffuse, float _m_Ambient, const std::string& shaderPathVert, const std::string& shaderPathFrag);
	~Material();

	std::unique_ptr<Shader> m_Shader;

	void Bind() const;
	void UpdateUniforms(float shininess, float specular, float diffuse, float ambient);

private:
	std::unique_ptr<Texture> m_Texture;
	float m_Shininess, m_Specular, m_Diffuse, m_Ambient;
	void SetUniforms() const;
};