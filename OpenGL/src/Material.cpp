#include "Material.h"

Material::Material(const std::string& texturePath, float _m_Shininess, float _m_Specular, float _m_Diffuse, float _m_Ambient)
	: m_Shininess(_m_Shininess),
	  m_Specular(_m_Specular),
	  m_Diffuse(_m_Diffuse),
	  m_Ambient(_m_Ambient)
{
	m_Texture = std::make_unique<Texture>(texturePath);
}

Material::~Material()
{

}

void Material::Bind() const
{
	m_Texture->Bind();
}
