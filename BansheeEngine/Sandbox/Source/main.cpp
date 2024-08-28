#include "ExampleModel.h"
#include "Light.h"
#include <Banshee.h>

class ClientApp : public Banshee::Application
{
public:
	ClientApp() :
		m_CustomModel{},
		m_CustomModel2{},
		m_Cube{},
		m_Light{}
	{
		m_CustomModel2.SetPosition(glm::vec3(20.0f, 0.0f, 0.0f));
	}

	ClientApp(const ClientApp&) = delete;
	ClientApp& operator=(const ClientApp&) = delete;
	ClientApp(ClientApp&&) = delete;
	ClientApp& operator=(ClientApp&&) = delete;

private:
	ExampleCustomModel m_CustomModel;
	ExampleCustomModel m_CustomModel2;
	ExamplePrimitiveModel m_Cube;
	Light m_Light;
};

std::unique_ptr<Banshee::Application> CreateApplication()
{
	return std::make_unique<ClientApp>();
}