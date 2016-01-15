#include "root.h"

namespace byhj
{
	Root::Root()
	{
	}

	Root::~Root()
	{
	}

	 Root* Root::getInstance()
	{
		static Root * pInstance = new Root();
		return pInstance;
	}
}