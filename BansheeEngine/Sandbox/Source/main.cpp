#include "ExampleModel.h"
#include "Light.h"
#include <Banshee.h>

class ClientApp : public Banshee::Application
{
public:
	ClientApp() :
		m_ExampleScene{},
		m_ExampleScene2{},
		m_Light{}
	{
		m_ExampleScene2.SetPosition(glm::vec3(20.0f, 0.0f, 0.0f));
	}

	ClientApp(const ClientApp&) = delete;
	ClientApp& operator=(const ClientApp&) = delete;
	ClientApp(ClientApp&&) = delete;
	ClientApp& operator=(ClientApp&&) = delete;

private:
	ExampleModel m_ExampleScene;
	ExampleModel m_ExampleScene2;
	Light m_Light;
};

std::unique_ptr<Banshee::Application> CreateApplication()
{
	return std::make_unique<ClientApp>();
}