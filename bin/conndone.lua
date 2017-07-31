print("this is conndone");

while(1)
do
	math.randomseed(os.time());
	local msgid = math.random(1, 50000);

	print("msgid = " .. msgid);

	local ctx = {
		ver = 1,
		["type"] = "CON",
		code = "GET",
		msgid = msgid,
		option = {
			["uri-port"] = 9095,
			["uri-path"] = "/login/username/password"
		}
	};
		
	local ret = x12_coap_msg_send(DRVID, ctx);
		
	print("drvid = " .. DRVID .. ", name = " .. NAME .. ", ret = " .. ret);

	x12_coap_sleep(5);

end