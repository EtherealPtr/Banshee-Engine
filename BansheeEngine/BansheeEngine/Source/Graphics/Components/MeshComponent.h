#pragma once

#include "Foundation/Components/Component.h"
#include "Foundation/DLLConfig.h"
#include "Graphics/PrimitiveShape.h"
#include "Graphics/ShaderType.h"
#include <string>

namespace Banshee
{
	class MeshComponent : public Component
	{
	public:
		BANSHEE_ENGINE MeshComponent(std::string_view _modelPath, const ShaderType _shaderType = ShaderType::Standard);
		BANSHEE_ENGINE MeshComponent(const PrimitiveShape _basicShape, const ShaderType _shaderType = ShaderType::Standard);

		void SetVertexBufferId(const uint32 _vertexBufferId) noexcept { m_VertexBufferId = _vertexBufferId; }
		uint32 GetVertexBufferId() const noexcept { return m_VertexBufferId; }
		ShaderType GetShaderType() const noexcept { return m_ShaderType; }
		const std::string_view GetModelName() const noexcept { return m_ModelName; }
		const std::string GetModelPath() const;
		bool HasModel() const noexcept { return m_HasModel; }

	private:
		uint32 m_VertexBufferId;
		ShaderType m_ShaderType;
		std::string m_ModelName;
		bool m_HasModel;
	};
} // End of Banshee namespace