#include <Banshee.h>
#include "Player.h"
#include "DummyObject.h"
#include "Light.h"

class ClientApp : public Banshee::Application
{
public:
	ClientApp() :
		m_Player(),
		m_Light(),
		m_DummyObject()
	{}

private:
	Player m_Player;
	Light m_Light;
	DummyObjectOne m_DummyObject;
};

std::unique_ptr<Banshee::Application> CreateApplication()
{
	return std::make_unique<ClientApp>();
}