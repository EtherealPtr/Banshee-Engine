#include <Banshee.h>
#include "Player.h"
#include "DummyObject.h"

class ClientApp : public Banshee::Application
{
public:
	ClientApp() : 
		m_Player()
	{}

private:
	Player m_Player;
};

std::unique_ptr<Banshee::Application> CreateApplication()
{
	return std::make_unique<ClientApp>();
}