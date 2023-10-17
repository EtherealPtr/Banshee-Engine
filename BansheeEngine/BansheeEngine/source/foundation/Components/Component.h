#pragma once

namespace Banshee
{
	class Component
	{
	public:
		Component();
		virtual ~Component() = default;

		void SetDirty(const bool _dirty) { m_Dirty = _dirty; }
		bool GetDirty() const { return m_Dirty; }

	protected:
		bool m_Dirty;
	};
} // End of Banshee namespace