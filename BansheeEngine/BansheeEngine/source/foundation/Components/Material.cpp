#include "Material.h"
#include "Foundation/ResourceManager/ResourceManager.h"

namespace Banshee
{
	uint16 Material::SetTexture(const char* _pathToTexture)
	{
		return ResourceManager::Instance().LoadImageResource(_pathToTexture);
	}
} // End of Banshee namespace