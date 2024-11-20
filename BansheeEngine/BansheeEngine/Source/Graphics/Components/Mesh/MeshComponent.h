#pragma once

#include "Foundation/Components/Component.h"
#include "Foundation/DLLConfig.h"
#include "Graphics/ShaderType.h"
#include <glm/glm.hpp>
#include <optional>

namespace Banshee
{
	class MeshComponent : public Component
	{
	public:
		BANSHEE_ENGINE MeshComponent(const ShaderType _shaderType) noexcept;
		BANSHEE_ENGINE ~MeshComponent() = default;

		virtual void SetTintColor(const glm::vec4& _tintColor) noexcept { m_TintColor = _tintColor; }
		virtual void SetVertexBufferId(const uint32 _vertexBufferId) noexcept { m_VertexBufferId = _vertexBufferId; }
		uint32 GetVertexBufferId() const noexcept { return m_VertexBufferId; }
		ShaderType GetShaderType() const noexcept { return m_ShaderType; }
		const std::optional<glm::vec4>& GetTintColor() const noexcept { return m_TintColor; }

	protected:
		uint32 m_VertexBufferId;
		ShaderType m_ShaderType;
		std::optional<glm::vec4> m_TintColor;
	};
} // End of Banshee namespace