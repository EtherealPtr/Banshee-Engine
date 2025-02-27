#pragma once

#include "Foundation/Platform.h"
#include "Graphics/ShaderType.h"
#include <glm/glm.hpp>

namespace Banshee
{
	class Material
	{
	public:
		Material(const glm::vec3& _diffuse = glm::vec3(1.0f), const glm::vec3& _specular = glm::vec3(1.0f), float _shininess = 8.0f, const ShaderType _shaderType = ShaderType::Standard) noexcept :
			m_DiffuseColor{ glm::vec4{_diffuse, 1.0f} },
			m_SpecularColor{ glm::vec4(_specular, _shininess) },
			m_ShaderType{ _shaderType }
		{}

		void SetDiffuseColor(const glm::vec4& _diffuseColor) noexcept { m_DiffuseColor = _diffuseColor; }
		void SetSpecularColor(const glm::vec4& _specularColor) noexcept { m_SpecularColor = _specularColor; }
		void SetShaderType(const ShaderType _shaderType) noexcept { m_ShaderType = _shaderType; }
		const glm::vec4& GetDiffuseColor() const noexcept { return m_DiffuseColor; }
		const glm::vec4& GetSpecularColor() const noexcept { return m_SpecularColor; }
		ShaderType GetShaderType() const noexcept { return m_ShaderType; }

	private:
		glm::vec4 m_DiffuseColor;
		glm::vec4 m_SpecularColor;
		ShaderType m_ShaderType;
	};
} // End of namespace
