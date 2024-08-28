#pragma once

#include "Foundation/Platform.h"
#include <glm/glm.hpp>

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
		PushConstant(const glm::mat4& _model, const uint16 _texId) noexcept :
			m_Model{ _model },
			m_TextureIndex{ _texId }
		{}

		const glm::mat4 m_Model{ glm::mat4(1.0f) };
		const uint32 m_TextureIndex{ 0 };
	};
} // End of Banshee namespace