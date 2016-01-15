#include "root.h"

namespace byhj
{
	Root::Root()
	{
	}

	Root::~Root()
	{
	}

	App* Root::init()
	{

	}


	 Root* Root::getInstance()
	{
		static Root * pInstance = new Root();
		return pInstance;
	}
}