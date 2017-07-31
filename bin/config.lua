
x12cfg = {
	cfgname = "testserver",						--服务名称
	service = {									--服务数组
		{
			family = "x12_udp",					--协议族
			name = "udpserver",					--全局唯一服务名称
			action = "listen",					--协议动作
			address = "192.168.1.105",			--监听地址
			port = 9090,						--监听端口
			inc = {								--包含文件
				"./bin/inc1.lua",				--包含脚本(程序会直接执行)
				"./bin/inc2.lua"
			}
		},
		{
			family = "x12_plugin_py",			--协议族
			pludir = {							--插件地址
				"./bin/py_plugin"				
			}
		},
		{
			family = "x12_coap_udp_l",
			name = "coapserver",
			action = "listen",
			port = 9095,
			msgcb = {"./bin/route.lua", "msgcb"},
			inc = {								--包含文件
				"./bin/inc1.lua",				--包含脚本(程序会直接执行)
				"./bin/inc2.lua"
			},
			route = {							--路由链表
				{
					"/time",
					"./bin/route.lua", 
					"time"
				},
				{
					"/login/:uid/:pwd",
					"./bin/route.lua", 
					"login"
				}
			}
		},
		{
			family = "x12_coap_udp_l",
			name = "coapconn",
			action = "connection",
			address = "192.168.1.242",
			port = 9090,
			inc = {								--初始化以前执行脚本
				"./bin/test1.lua",
				"./bin/test2.lua"
			},
			done = {
				"./bin/test.lua",				--初始化以后执行脚本
				"./bin/test2.lua"
			}
		}
	}
};




