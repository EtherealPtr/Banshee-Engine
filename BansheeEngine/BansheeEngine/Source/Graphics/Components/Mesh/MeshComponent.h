#pragma once

#include "Foundation/Components/Component.h"
#include "Foundation/DLLConfig.h"
#include "Graphics/ShaderType.h"
#include <glm/glm.hpp>
#include <string>

namespace Banshee
{
	class MeshComponent : public Component
	{
	public:
		BANSHEE_ENGINE MeshComponent(const ShaderType _shaderType) noexcept;
		BANSHEE_ENGINE ~MeshComponent() = default;

		void SetTintColor(const glm::vec3& _tintColor) noexcept { m_TintColor = _tintColor; }
		void SetVertexBufferId(const uint32 _vertexBufferId) noexcept { m_VertexBufferId = _vertexBufferId; }
		uint32 GetVertexBufferId() const noexcept { return m_VertexBufferId; }
		ShaderType GetShaderType() const noexcept { return m_ShaderType; }
		const glm::vec3& GetTintColor() const noexcept { return m_TintColor; }

	protected:
		uint32 m_VertexBufferId;
		ShaderType m_ShaderType;
		glm::vec3 m_TintColor;
	};
} // End of Banshee namespace