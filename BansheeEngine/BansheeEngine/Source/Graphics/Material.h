#pragma once

#include "Foundation/Platform.h"
#include "Graphics/ShaderType.h"
#include <glm/glm.hpp>

namespace Banshee
{
	class Material
	{
	public:
		Material(const glm::vec3& _diffuse = glm::vec3(1.0f), const glm::vec3& _specular = glm::vec3(1.0f), float _shininess = 32.0f, const ShaderType _shaderType = ShaderType::Standard) noexcept :
			m_DiffuseColor{ glm::vec4{_diffuse, 1.0f} },
			m_SpecularColor{ glm::vec4{_specular, 1.0f} },
			m_Shininess{ _shininess },
			m_ShaderType{ _shaderType }
		{}

		void SetDiffuseColor(const glm::vec3& _color) noexcept { m_DiffuseColor = glm::vec4(_color, 1.0f); }
		void SetSpecularColor(const glm::vec3& _color) noexcept { m_SpecularColor = glm::vec4(_color, 1.0f); }
		void SetShininess(const float _value) noexcept { m_Shininess = _value; }
		void SetShaderType(const ShaderType _shaderType) noexcept { m_ShaderType = _shaderType; }
		const glm::vec4& GetDiffuseColor() const noexcept { return m_DiffuseColor; }
		const glm::vec4& GetSpecularColor() const noexcept { return m_SpecularColor; }
		float GetShininess() const noexcept { return m_Shininess; }
		ShaderType GetShaderType() const noexcept { return m_ShaderType; }

	private:
		glm::vec4 m_DiffuseColor;
		glm::vec4 m_SpecularColor;
		float m_Shininess;
		ShaderType m_ShaderType;
	};
} // End of Banshee namespace
