#include "LightComponent.h"

namespace Banshee
{
	void LightComponent::OnNotify() noexcept
	{
		if (!m_NeedsUpdate)
		{
			m_NeedsUpdate = true;
		}
	}
} // End of Banshee namespace
