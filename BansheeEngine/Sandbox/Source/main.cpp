#include <Banshee.h>
#include "Player.h"
#include "DummyObject.h"

class ClientApp : public Banshee::Application
{
public:
	ClientApp() : 
		m_Player(),
		m_DummyOne(),
		m_DummyTwo()
	{}

private:
	Player m_Player;
	DummyObjectOne m_DummyOne;
	DummyObjectTwo m_DummyTwo;
};

std::unique_ptr<Banshee::Application> CreateApplication()
{
	return std::make_unique<ClientApp>();
}