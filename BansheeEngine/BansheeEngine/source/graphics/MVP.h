#pragma once

#include <glm/glm.hpp>
#include "Foundation/Platform.h"

namespace Banshee
{
	struct ViewProjMatrix
	{
		ViewProjMatrix() noexcept = default;
		ViewProjMatrix(const glm::mat4& _view, const glm::mat4& _proj) noexcept :
			view(_view),
			proj(_proj)
		{}

		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);
	};

	struct PushConstant
	{
		PushConstant(const glm::mat4& _model, const uint32 _texId) noexcept :
			model(_model),
			textureIndex(_texId)
		{}

		glm::mat4 model = glm::mat4(1.0f);
		uint32 textureIndex = 0;
	};
} // End of Banshee namespace