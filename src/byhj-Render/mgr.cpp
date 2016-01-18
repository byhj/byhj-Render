#include "mgr.h"

namespace byhj
{
	Mgr::Mgr()
	{
	}

	Mgr::~Mgr()
	{

	}

	void Mgr::setDir(std::string dir)
	{
		m_dir = dir;
	}

	std::string Mgr::getDir() const
	{
		return m_dir;
	}
}