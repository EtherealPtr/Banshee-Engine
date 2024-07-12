#pragma once

namespace Banshee
{
	class Observer
	{
	public:
		virtual void OnNotify() = 0;
	};
} // End of Banshee namespace