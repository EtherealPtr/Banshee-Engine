#pragma once

#include "Component.h"
#include "Foundation/Platform.h"
#include "Foundation/DLLConfig.h"
#include "Foundation/Systems/SortingLayerSystem.h"
#include <vector>

namespace Banshee
{
	class MeshComponent : public Component
	{
	public:
		BANSHEE_ENGINE MeshComponent(const uint8 _shape = 0) :
			m_Shape(_shape),
			m_TexId(0),
			m_Color(3, 1.0f),
			m_SortingLayer{0, 0}
		{}

		BANSHEE_ENGINE void SetColor(const float _r, const float _g, const float _b) noexcept;
		BANSHEE_ENGINE void SetTexture(const char* _pathToTexture);
		BANSHEE_ENGINE void SetSortLayer(const uint32 _layerId, const int32 _priority) noexcept;
		const SortingLayer& GetSortLayer() const noexcept { return m_SortingLayer; }
		const std::vector<float>& GetColor() const noexcept { return m_Color; }
		const uint8 GetShape() const noexcept { return m_Shape; }
		const uint32 GetTexId() const noexcept { return m_TexId; }

	private:
		uint8 m_Shape;
		uint32 m_TexId;
		std::vector<float> m_Color;
		SortingLayer m_SortingLayer;
	};
} // End of Banshee namespace