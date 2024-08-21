#pragma once

#include "Foundation/Components/Component.h"
#include "Foundation/DLLConfig.h"
#include "Graphics/PrimitiveShape.h"
#include "Graphics/ShaderType.h"
#include <glm/glm.hpp>
#include <string>

namespace Banshee
{
	class MeshComponent : public Component
	{
	public:
		BANSHEE_ENGINE MeshComponent(const std::string_view _modelPath, const ShaderType _shaderType = ShaderType::Standard, const glm::vec3& _col = glm::vec3(1.0f));
		BANSHEE_ENGINE MeshComponent(const PrimitiveShape _basicShape, const ShaderType _shaderType = ShaderType::Standard, const glm::vec3& _col = glm::vec3(1.0f));

		void SetVertexBufferId(const uint32 _vertexBufferId) noexcept { m_VertexBufferId = _vertexBufferId; }
		uint32 GetVertexBufferId() const noexcept { return m_VertexBufferId; }
		ShaderType GetShaderType() const noexcept { return m_ShaderType; }
		const glm::vec3& GetDiffuseColor() const noexcept { return m_DiffuseColor; }
		PrimitiveShape GetBasicShape() const noexcept { return m_BasicShape; }
		const std::string_view GetModelName() const noexcept { return m_ModelName; }
		const std::string GetModelPath() const;
		bool HasModel() const noexcept { return m_HasModel; }

	private:
		uint32 m_VertexBufferId;
		ShaderType m_ShaderType;
		glm::vec3 m_DiffuseColor;
		PrimitiveShape m_BasicShape;
		std::string m_ModelName;
		bool m_HasModel;
	};
} // End of Banshee namespace