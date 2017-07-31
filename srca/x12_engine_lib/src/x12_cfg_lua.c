/*
 *
 *Copyright (C) 刘晓龙
 *Copyright (C) 昆明黑海科技有限公司
 *
 *
 */

/*
 *x12 lua  配置模块
 *
 *
 *作者:刘晓龙
 *时间:2017-07-03
 *最后修改时间:2017-07-03
 */


#include "x12_cfg_lua.h"


int x12_cfg_lua_init(const char *_file, const char *_cfgname)
{

	if(!_file || !_cfgname)
	{
		return -1;
	}

	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	if(luaL_dofile(L, _file))
	{
		return -1;
	}

	lua_getglobal(L, _cfgname);			
		
	if(!lua_istable(L, -1))
	{
		return -1;
	}

	int cfgt_idx = lua_gettop(L);
	lua_pushnil(L);

	printf("this is system cfg:\n");

	int i;

	while(lua_next(L, cfgt_idx))
	{
		const char *cfg_key = lua_tostring(L, -2);

		if(strcmp(cfg_key, "pyplugin") == 0)
		{
			printf("\tpyplugin dir is %s\n", lua_tostring(L, -1));
		}
		else if(strcmp(cfg_key, "service") == 0)
		{
			//service 数组
			int sert_idx = lua_gettop(L);
			int ser_len = luaL_len(L, sert_idx);

			for(i = 1; i <= ser_len; i++)
			{
				printf("\tservice%d:\n", i);

				lua_pushnumber(L, i);
				lua_gettable(L, -2);
				
				//协议族
				int ser_it_idx = lua_gettop(L);
				lua_pushstring(L, "family");
				lua_gettable(L, -2);
					
				const char *family = lua_tostring(L, -1);

				lua_pop(L, 1);
	
				if(strcmp(family, "x12_coap_udp") == 0)
				{
					x12_lua_coap_kit_do(L);
				}
				else if(strcmp(family, "x12_plugin_py") == 0)
				{
					x12_python_call_plugin_luacfg_do(L);
				}
				else
				{
					printf("\t\tfamily is %s\n", family);
				}

				lua_pop(L, 1);
			}

		}

		lua_pop(L, 1);
	}

	lua_pop(L, 1);

	lua_close(L);

	return 0;
}

