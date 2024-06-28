#pragma once

#include "LightData.h"
#include "Foundation/Entity/EntityManager.h"
#include "Components.h"
#include "Graphics/PrimitiveShape.h"

namespace Banshee
{
    class Light
    {
    public:
        Light(const glm::vec3& _location, const glm::vec3& _color = glm::vec3(1.0f)) noexcept :
            m_LightData(_location, _color)
        {
            m_Entity = EntityManager::Instance().CreateEntity();
            const std::shared_ptr<MeshComponent>& meshComponent = m_Entity->AddComponent<MeshComponent>(PrimitiveShape::Cube);
            meshComponent->SetShaderType(ShaderType::Unlit);
            m_Transform = m_Entity->GetTransform();
            m_Transform.get()->SetPosition(m_LightData.location);
        }

        const glm::vec3& GetLocation() const noexcept { return m_LightData.location; }
        const glm::vec3& GetColor() const noexcept { return m_LightData.color; }

    private:
        LightData m_LightData;
        std::shared_ptr<Entity> m_Entity;
        std::shared_ptr<TransformComponent> m_Transform;
    };
} // End of Banshee namespace
