
x12cfg = {
	--pyplugin = "./bin/py_plugin",			--python插件目录
	service = {								--服务数组
		{
			family = "x12_udp",					--协议族
			name = "udpserver",					--全局唯一服务名称
			action = "listen",					--协议动作
			address = "192.168.1.105",			--监听地址
			port = 9090,						--监听端口
			inc = {								--包含文件
				"./bin/test1.lua",				--包含脚本(程序会直接执行)
				"./bin/test2.lua"
			}
		},
		{
			family = "x12_coap_udp_l",
			name = "coapserver",
			action = "listen",
			port = 9095,
			msgcb = {"./bin/route.lua", "msgcb"},
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
			family = "x12_coap_udp",
			name = "coapconn",
			action = "connection",
			address = "192.168.1.242",
			port = 9095,
			msgcb = {"./bin/route.lua", "msgcb"},
			done = {
				"./bin/conndone.lua",				--初始化以后执行脚本
			}
		}
	}
};




