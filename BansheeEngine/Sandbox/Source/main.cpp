#include <Banshee.h>
#include "Player.h"
#include "Ground.h"

using namespace Banshee;

class ClientApp : public Application
{
public:
	ClientApp()
	{
		
	}

private:
	Player m_Player;
	Ground m_Ground;
	Ground2 m_Ground2;
};

Application* CreateApplication()
{
	return new ClientApp();
}