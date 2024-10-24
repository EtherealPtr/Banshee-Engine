#include "ExampleModel.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include <Banshee.h>

class ClientApp : public Banshee::Application
{
public:
	ClientApp() :
		m_CustomModel{},
		m_CustomModel2{},
		m_DirectionalLight{},
		m_PointLight{}
	{}

	ClientApp(const ClientApp&) = delete;
	ClientApp& operator=(const ClientApp&) = delete;
	ClientApp(ClientApp&&) = delete;
	ClientApp& operator=(ClientApp&&) = delete;

private:
	ExampleCustomModel m_CustomModel;
	ExampleCustomModel m_CustomModel2;
	DirectionalLight m_DirectionalLight;
	PointLight m_PointLight;
};

std::unique_ptr<Banshee::Application> CreateApplication()
{
	return std::make_unique<ClientApp>();
}