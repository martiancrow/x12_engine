--sqlite3 = require "luasql.sqlite3";

msg = "lua value";

server = {
	address = "192.168.1.1",
	port = 9090,
	protocol = "x12_udp"
};

x12cfg = {
	server = {
		mode = "cfg",
		address = "192.168.1.105",
		port = 9090,
		protocol = "x12_udp",
		action = "listen",
		callfile = "test.lua"
	}
};


function add(x, y)
	return x + y;
end

print("this is lua test " .. _VERSION);
--chelloworld();
--print(cmsg);
--local  sum = add1(1, 2);
--print("sum = " .. sum);

