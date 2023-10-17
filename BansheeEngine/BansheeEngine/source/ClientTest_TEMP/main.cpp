#include <Banshee.h>
#include "TestEntity.h"

using namespace Banshee;

class ClientApp : public Application
{
public:
	ClientApp()
	{
		
	}

private:
	Player m_Player;
};

Application* CreateApplication()
{
	return new ClientApp();
}
