#pragma once

#include "Foundation/Platform.h"
#include "Graphics/Vertex.h"
#include "Material.h"
#include <vector>

namespace Banshee
{
	struct Mesh
	{
		Mesh() noexcept :
			indexOffset{ 0 },
			vertices{},
			indices{},
			material{},
			localTransform(1.0f),
			m_MaterialIndex(0),
			m_TexId(0),
			m_HasTexture(false)
		{}

		~Mesh() = default;

		void SetTexId(const uint16 _texId) noexcept { m_TexId = _texId; m_HasTexture = true; }
		bool HasTexture() const noexcept { return m_HasTexture; }
		uint16 GetTexId() const noexcept { return m_TexId; }
		uint32 GetMaterialIndex() const noexcept { return m_MaterialIndex; }
		uint32 indexOffset;   // Offset into the index buffer
		std::vector<Vertex> vertices{};
		std::vector<uint32> indices{};
		Material material;
		glm::mat4 localTransform;

	private:
		static uint32 SetNextMaterialIndex() noexcept
		{
			static uint32 nextIndex = 0;
			return nextIndex++;
		}

	private:
		uint32 m_MaterialIndex;
		uint16 m_TexId;
		bool m_HasTexture;
	};
} // End of Banshee namespace