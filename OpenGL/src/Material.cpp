#include "Material.h"

Material::Material(const std::string& texturePath)
	: m_Shininess(100.f),
	  m_Specular(0.75f),
	  m_Diffuse(0.5f),
	  m_Ambient(0.2f)
{
	m_Texture = std::make_unique<Texture>(texturePath);
}

Material::Material()
	: m_Shininess(100.f),
	  m_Specular(0.75f),
	  m_Diffuse(0.5f),
	  m_Ambient(0.2f)
{

}

Material::~Material()
{

}

void Material::Bind() const
{
	m_Texture->Bind();
}
