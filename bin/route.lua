
function time()

	print("this is time test");

	local date = os.date("%Y-%m-%d %H:%M:%S");
	
	local ctx = {
		ver = 1,
		["type"] = "ACK",
		code = "Content",
		msgid = HEAD["msgid"],
		payload = date,
		option = {
			["uri-port"] = 9095
		}
	};
	
	local ret = x12_coap_msg_send(senderid, ctx);
	
	print("ret = " .. ret);

end

function msgcb()
	print("this is msgcb");

	print(POST);
end

function login()
	print("this is login");	
	print("uid = " .. GET["uid"] .. ", pwd = " .. GET["pwd"]);
	print("code = " .. HEAD["code"] .. ", msgid = " .. HEAD["msgid"]);
	print("uri = " .. OPTION["uri-path"] .. ", port = " .. OPTION["uri-port"]);
	print("");

	local date = os.date("%Y-%m-%d %H:%M:%S");


	
	local ret = x12_coap_response(date);
	
	print("ret = " .. ret);
end	

