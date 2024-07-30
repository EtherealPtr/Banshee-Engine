#pragma once

#include "Foundation/DLLConfig.h"
#include "Foundation/Platform.h"
#include "Foundation/Components/Component.h"
#include <vector>
#include <memory>

namespace Banshee
{
	class Component;
	class TransformComponent;

	class Entity : public std::enable_shared_from_this<Entity>
	{
	public:
		BANSHEE_ENGINE explicit Entity(const uint32 _id) noexcept;
		BANSHEE_ENGINE ~Entity() noexcept = default;

		BANSHEE_ENGINE const uint32 GetUniqueId() const noexcept { return m_Id; }

		template<typename T, typename... Args>
		std::shared_ptr<T> AddComponent(Args&&... _args);

		template<typename T>
		std::shared_ptr<T> GetComponent() const noexcept;

		const std::shared_ptr<TransformComponent>& GetTransform() const noexcept { return m_TransformComponent; }

	private:
		uint32 m_Id;
		std::vector<std::shared_ptr<Component>> m_Components;
		std::shared_ptr<TransformComponent> m_TransformComponent;
	};

	template<typename T, typename... Args>
	std::shared_ptr<T> Entity::AddComponent(Args&&... _args)
	{
		static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

		auto component = std::make_shared<T>(std::forward<Args>(_args)...);
		component->SetOwner(shared_from_this());
		m_Components.emplace_back(component);

		if (auto transform = std::dynamic_pointer_cast<TransformComponent>(component))
		{
			m_TransformComponent = transform;
		}

		return component;
	}

	template<typename T>
	std::shared_ptr<T> Entity::GetComponent() const noexcept
	{
		static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

		for (const auto& component : m_Components)
		{
			if (auto castedComponent = std::dynamic_pointer_cast<T>(component))
			{
				return castedComponent;
			}
		}

		return nullptr;
	}
} // End of Banshee namespace