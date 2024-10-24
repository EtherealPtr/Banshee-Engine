#pragma once

#include "Foundation/Entity/Entity.h"
#include "Graphics/PrimitiveShape.h"
#include "Components.h"

using namespace Banshee;

class PointLight : public Entity
{
public:
	PointLight()
	{
		AddComponent<PrimitiveMeshComponent>(PrimitiveShape::CubeShape, ShaderType::Unlit);
		AddComponent<PointLightComponent>();
		m_Transform = AddComponent<TransformComponent>();
		m_Transform->SetPosition(glm::vec3(0.0f, 2.0f, 0.0f));
	}

private:
	std::shared_ptr<TransformComponent> m_Transform;
};