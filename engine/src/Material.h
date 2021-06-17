#pragma once

#include <memory>
#include "Texture.h"

class Material
{
public:
	Material(const std::string& texturePath, float _m_Shininess, float _m_Specular, float _m_Diffuse, float _m_Ambient);
	~Material();
	float m_Shininess, m_Specular, m_Diffuse, m_Ambient;

	void Bind() const;

private:
	std::unique_ptr<Texture> m_Texture;

};