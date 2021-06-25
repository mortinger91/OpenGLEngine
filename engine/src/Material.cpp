#include "Material.h"

// TODO: reformat code, use builder pattern to make initialization easier
// setup class as an object created once, that can be "assigned" to multiple meshes with no copies  
Material::Material(const std::string& texturePath, float _m_Shininess, float _m_Specular, float _m_Diffuse, float _m_Ambient, std::shared_ptr<Shader> shader)
	: m_Shininess(_m_Shininess),
	  m_Specular(_m_Specular),
	  m_Diffuse(_m_Diffuse),
	  m_Ambient(_m_Ambient),
	  m_Texture(texturePath)
	  //,m_Shader(shader)
{
	m_Shader = shader;
}

Material::~Material()
{

}

void Material::Bind() const
{
	m_Shader->Bind();
	m_Shader->SetUniform1i("u_Texture", 0);
	m_Texture.Bind();
	SetUniforms();
}

void Material::SetUniforms() const
{
	m_Shader->SetUniform1i("useTexture", 1);
	m_Shader->SetUniform4fv("ambient", { m_Ambient, m_Ambient, m_Ambient, 1 });
	m_Shader->SetUniform4fv("diffuse", { m_Diffuse, m_Diffuse, m_Diffuse, 1 });
	m_Shader->SetUniform4fv("specular", { m_Specular, m_Specular, m_Specular, 1 });
	m_Shader->SetUniform1fv("shininess", m_Shininess);
}

void Material::UpdateUniforms(float shininess, float specular, float diffuse, float ambient)
{
	m_Shininess = (shininess != 0.f) ? shininess : m_Shininess;
	m_Specular = (specular != 0.f) ? specular : m_Specular;
	m_Diffuse = (diffuse != 0.f) ? diffuse : m_Diffuse;
	m_Ambient = (ambient != 0.f) ? ambient : m_Ambient;
}