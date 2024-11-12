#pragma once

#include "Foundation/Entity/Entity.h"
#include "Graphics/PrimitiveShape.h"
#include "Components.h"

using namespace Banshee;

class PointLight : public Entity
{
public:
	PointLight(const glm::vec3& _pos = glm::vec3(1.0f), const glm::vec4& _color = glm::vec4(1.0f), const float _radius = 10.0f)
	{
		auto meshComp = AddComponent<PrimitiveMeshComponent>(PrimitiveShape::CubeShape, ShaderType::Unlit);
		meshComp->SetTintColor(glm::vec4(_color.x, _color.y, _color.z, 1.0f));
		AddComponent<PointLightComponent>(_color, _radius);
		m_Transform = AddComponent<TransformComponent>();
		m_Transform->SetPosition(_pos);
	}

private:
	std::shared_ptr<TransformComponent> m_Transform;
};