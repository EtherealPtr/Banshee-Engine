#include "ExampleScene.h"
#include "DirectionalLight.h"
#include <Banshee.h>
#include <array>

class ClientApp : public Banshee::Application
{
public:
	ClientApp() :
		m_DirectionalLight{ glm::vec3(0.5f, -0.5f, -1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) },
		m_Ground{},
		m_CustomModel{ glm::vec3(0.0f, 1.0f, 0.0f) }
	{
		const std::array<glm::vec3, 3> cubePositions
		{
			glm::vec3(-4.0f, 0.5f, -1.0f),
			glm::vec3(5.0f, 1.0f, 1.0f),
			glm::vec3(2.0f, 0.5f, 5.0f)
		};

		for (size_t i = 0; i < m_Cubes.size(); ++i)
		{
			m_Cubes[i].SetPosition(cubePositions[i]);
		}
	}

	ClientApp(const ClientApp&) = delete;
	ClientApp& operator=(const ClientApp&) = delete;
	ClientApp(ClientApp&&) = delete;
	ClientApp& operator=(ClientApp&&) = delete;

private:
	DirectionalLight m_DirectionalLight;
	ExampleQuad m_Ground;
	ExampleCustomModel m_CustomModel;
	std::array<ExampleCube, 3> m_Cubes;
};

std::unique_ptr<Banshee::Application> CreateApplication()
{
	return std::make_unique<ClientApp>();
}