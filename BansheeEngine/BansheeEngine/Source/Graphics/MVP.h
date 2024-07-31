#pragma once

#include <glm/glm.hpp>
#include "Foundation/Platform.h"

namespace Banshee
{
	struct ViewProjMatrix
	{
		ViewProjMatrix() noexcept = default;
		ViewProjMatrix(const glm::mat4& _view, const glm::mat4& _proj) noexcept :
			m_View{ _view },
			m_Proj{ _proj }
		{}

		glm::mat4 m_View{ glm::mat4(1.0f) };
		glm::mat4 m_Proj{ glm::mat4(1.0f) };
	};

	struct PushConstant
	{
		PushConstant(const glm::mat4& _model, const uint16 _texId, const uint16 _hasCustomTexture) noexcept :
			m_Model{ _model },
			m_TextureIndex{ _texId },
			m_HasCustomTexture{ _hasCustomTexture }
		{}

		const glm::mat4 m_Model{ glm::mat4(1.0f) };
		const uint32 m_TextureIndex{ 0 };
		const uint32 m_HasCustomTexture{ 0 };
	};
} // End of Banshee namespace