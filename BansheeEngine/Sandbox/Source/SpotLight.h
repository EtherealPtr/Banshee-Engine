#pragma once

#include "Foundation/Entity/Entity.h"
#include "Graphics/PrimitiveShapeEnum.h"
#include "Components.h"

using namespace Banshee;

class SpotLight : public Entity
{
public:
	SpotLight(const glm::vec3& _pos = glm::vec3(1.0f), const glm::vec3& _direction = glm::vec3(0.0f, -1.0f, 0.0f), const glm::vec4& _color = glm::vec4(1.0f), const float _radius = 10.0f, const float _innerCutoffAngle = 12.5f, const float _outerCutoffAngle = 17.5f)
	{
		auto meshComp{ AddComponent<PrimitiveMeshComponent>(PrimitiveShapeEnum::PyramidShape, ShaderTypeEnum::Unlit) };
		meshComp->SetTintColor(glm::vec4(_color.x, _color.y, _color.z, 1.0f));
		AddComponent<SpotLightComponent>(_direction, _color, _radius, _innerCutoffAngle, _outerCutoffAngle);
		m_Transform = AddComponent<TransformComponent>();
		m_Transform->SetPosition(_pos);
	}

private:
	std::shared_ptr<TransformComponent> m_Transform;
};