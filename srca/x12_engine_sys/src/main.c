#include "x12sys.h"

void* x12_handle = NULL;
void* pyp_handle = NULL;

void exit_handler(int _s)
{
	if(x12_handle)
	{
		void (*x12_python_call_reslease)(void);
		x12_python_call_reslease = dlsym(x12_handle, "x12_python_call_reslease");
	
		x12_python_call_reslease();
		dlclose(x12_handle);
	}

	if(pyp_handle)
	{
		dlclose(pyp_handle);
	}

	exit(1);
}

int main(int argc, char *argv[])
{
	struct sigaction sigIntHandler;  
	sigIntHandler.sa_handler = exit_handler;  
	sigemptyset(&sigIntHandler.sa_mask);  
	sigIntHandler.sa_flags = 0;  
	sigaction(SIGINT, &sigIntHandler, NULL); 

	if(argc > 1 && strcmp(argv[1], "-cfg") == 0)
	{

		char *file = "./bin/config.lua";
		char *name = "x12cfg";
		
		x12_handle = dlopen("./bin/x12.so", RTLD_LAZY);
		pyp_handle = dlopen("./bin/x12_python.so", RTLD_LAZY);

		if(!x12_handle)
		{
			printf("load x12.so fail!\n");
			return 0;
		}

		if(!pyp_handle)
		{
			printf("load x12_python.so fail!\n");
			return 0;
		}

		char (*x12_cfg_lua_init)(char*, char*);
		x12_cfg_lua_init = dlsym(x12_handle, "x12_cfg_lua_init");
		
		void (*x12_listen_evens_forever)(void);
		x12_listen_evens_forever = dlsym(x12_handle, "x12_listen_evens_forever");
		
		
		if(argc > 2)
		{
			file = argv[2];
		}

		if(argc > 3)
		{
			name = argv[3];
		}

		x12_cfg_lua_init(file, name);
		x12_listen_evens_forever();
		
	}
	else
	{
		int port = 9095;

		printf("%d\n", port);
		port = htons(port);
		printf("%d\n", port);

		uint16_t port1 = 9095;
		port1 = htons(port1);
		printf("%d\n", port1);
	}

	return 0;
}
