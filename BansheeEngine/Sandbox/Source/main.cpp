#include <Banshee.h>
#include "Player.h"
#include "DummyObject.h"

using namespace Banshee;

class ClientApp : public Application
{
public:
	ClientApp()
	{
		
	}

private:
	DummyObjectOne m_DummyOne;
	Player m_Player;
	DummyObjectTwo m_DummyTwo;
};

Application* CreateApplication()
{
	return new ClientApp();
}