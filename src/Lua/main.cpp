
extern "C"
{
  #include "lua/lua.h"  
  #include "lua/lauxlib.h"  
  #include "lua/lualib.h"  
}


#include <iostream>

#pragma comment(lib, "lua53.lib")

int main()
{
	
	lua_State *pLuaState = luaL_newstate();

	//put in stack
	lua_pushstring(pLuaState, "This is a test");
	lua_pushnumber(pLuaState, 100);

	//get value
	if (lua_isstring(pLuaState, 1)) {
	    std::cout << lua_tostring(pLuaState, 1) << std::endl;
	}
	if (lua_isnumber(pLuaState, 2)) {
	    std::cout << lua_tonumber(pLuaState, 2) << std::endl;
	}

	lua_close(pLuaState);

	lua_State *pHelloState = luaL_newstate();
	if (pHelloState == NULL) {
	    std::cerr << "can not create a new lua state" << std::endl;
	}
	//load a lua file
    auto ret1 = luaL_loadfile(pHelloState, "hello.lua");
	if (ret1) {
	    std::cerr << "load file error" << std::endl;
	}

	//run the luad file
	auto ret2 = lua_pcall(pHelloState, 0, 0, 0);
	if (ret2) {
		std::cerr << "run file error" << std::endl;
	}

	//-1 is the stack top
	lua_getglobal(pHelloState, "str");
	auto str = lua_tostring(pHelloState, -1);
	std::cout << "str:" << str  << std::endl;

	//read the table
	lua_getglobal(pHelloState, "tbl");
	lua_getfield(pHelloState, -1, "name"	);
	str = lua_tostring(pHelloState, -1);
	std::cout << "name:" << str << std::endl;

    //read function
	lua_getglobal(pHelloState, "add");
	lua_pushnumber(pHelloState, 1);
	lua_pushnumber(pHelloState, 2);
	int ret3 = lua_pcall(pHelloState, 2, 1, 0); //two number, one return
	if (ret3)  {
		const char *pErrorMsg = lua_tostring(pHelloState, -1);
		std::cout << pErrorMsg << std::endl;
		lua_close(pHelloState);
		return 0;
	}
	if (lua_isnumber(pHelloState, -1))        //È¡ÖµÊä³ö  
	{
		double fValue = lua_tonumber(pHelloState, -1);
		std::cout << "Result is " << fValue << std::endl;
	}
	lua_close(pHelloState);

	return 0;
}