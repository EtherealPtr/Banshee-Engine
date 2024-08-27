#pragma once

#include "MeshComponent.h"
#include "Graphics/MeshData.h"
#include <vector>

namespace Banshee
{
	class CustomMeshComponent : public MeshComponent
	{
	public:
		BANSHEE_ENGINE CustomMeshComponent(const std::string_view _modelPath, const ShaderType _shaderType = ShaderType::Standard);

		void AddMeshData(const MeshData& _data) noexcept;
		const std::string_view GetModelName() const noexcept { return m_ModelName; }
		const std::string GetModelPath() const;
		const std::vector<MeshData>& GetMeshData() const noexcept { return m_Data; }

	private:
		std::string m_ModelName;
		std::vector<MeshData> m_Data;
	};
} // End of Banshee namespace

