#pragma once

namespace Banshee
{
	class Subsystem
	{
	protected:
		virtual void Initialize() {};
		virtual void Shutdown() {};
	};

#define DECLARE_BANSHEE_SYSTEM(ClassType) static ClassType& Instance();
}; // End of Banshee namespace