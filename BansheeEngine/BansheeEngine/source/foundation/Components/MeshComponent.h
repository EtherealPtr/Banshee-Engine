#pragma once

#include "Component.h"
#include "Foundation/Platform.h"
#include "Foundation/DLLConfig.h"
#include "Foundation/Systems/SortingLayerSystem.h"
#include <vector>
#include <string>

namespace Banshee
{
	class MeshComponent : public Component
	{
	public:
		BANSHEE_ENGINE MeshComponent(const uint8 _shape = 0) :
			m_Shape(_shape),
			m_TexId(0),
			m_Color(3, 1.0f),
			m_SortingLayer{0, 0},
			m_ModelPath(""),
			m_HasModel(false),
			m_HasCustomTexture(false)
		{}

		BANSHEE_ENGINE MeshComponent(const char* _path) :
			m_Shape(0),
			m_TexId(0),
			m_Color(3, 1.0f),
			m_SortingLayer{ 0, 0 },
			m_ModelPath(_path),
			m_HasModel(true),
			m_HasCustomTexture(false)
		{}

		BANSHEE_ENGINE void SetColor(const float _r, const float _g, const float _b) noexcept;
		BANSHEE_ENGINE void SetTexture(const char* _pathToTexture);
		BANSHEE_ENGINE void SetSortLayer(const uint32 _layerId, const int32 _priority) noexcept;
		const SortingLayer& GetSortLayer() const noexcept { return m_SortingLayer; }
		const std::vector<float>& GetColor() const noexcept { return m_Color; }
		uint8 GetShape() const noexcept { return m_Shape; }
		uint16 GetTexId() const noexcept { return m_TexId; }
		std::string GetModelPath() const;
		bool HasModel() const noexcept { return m_HasModel; }
		bool HasCustomTexture() const noexcept { return m_HasCustomTexture; }

	private:
		uint8 m_Shape;
		uint16 m_TexId;
		std::vector<float> m_Color;
		SortingLayer m_SortingLayer;
		std::string m_ModelPath;
		bool m_HasModel;
		bool m_HasCustomTexture;
	};
} // End of Banshee namespace