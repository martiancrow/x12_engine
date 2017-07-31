/*
 *
 * Copyright (C) 刘晓龙
 * Copyright (C) 昆明黑海科技有限公司
 *
 */


/*
 * 
 * nl80211设备封装
 *
 * 作者:Crow
 * 时间:2017-03-17
 * 最后修改时间:2017-03-17
 *      
 */

#ifdef _OS_LINUX_

#include "driver_nl80211.h"

int getmac_handle(struct nl_msg *_msg, void* _arg)
{
    struct genlmsghdr *gnlh = nlmsg_data(nlmsg_hdr(_msg));
    struct nlattr *tb_msg[NL80211_ATTR_MAX + 1];
    uint8_t *mac = _arg;

    nla_parse(tb_msg, NL80211_ATTR_MAX, genlmsg_attrdata(gnlh, 0), genlmsg_attrlen(gnlh, 0), NULL);

    if (tb_msg[NL80211_ATTR_MAC]) 
    {
        memcpy(mac, nla_data(tb_msg[NL80211_ATTR_MAC]), 6);
    }
    
    return NL_SKIP;
}

int ack_handle(struct nl_msg *_msg, void* _arg)
{
	int *ret = _arg;
	*ret = 0;
	return NL_STOP;
}

int finish_handle(struct nl_msg *_msg, void* _arg)
{
	int *ret = _arg;
	*ret = 0;
	return NL_SKIP;
}

int error_handle(struct sockaddr_nl *_nla, struct nlmsgerr *_err, void* _arg)
{
	int *ret = _arg;
	*ret = _err->error;
	return NL_STOP;
}

int handle_cmd(nl80211_t *_state, struct nl_msg *_msg, struct nl_cb *_cb)
{

	if(!_state)
	{
		goto nla_put_failure;
	}

	if(!_msg)
	{
		goto nla_put_failure;
	}

	int err;

	if(_cb)
	{
		nl_cb_err(_cb, NL_CB_CUSTOM, error_handle, &err);
		nl_cb_set(_cb, NL_CB_FINISH, NL_CB_CUSTOM, finish_handle, &err);
		nl_cb_set(_cb, NL_CB_ACK, NL_CB_CUSTOM, ack_handle, &err);
	}

	err = nl_send_auto_complete(_state->nl80211_sock, _msg);

	if(err < 0)
	{
		goto nla_put_failure;
	}

	if(_cb)
	{
		while(err > 0)
		{
			nl_recvmsgs(_state->nl80211_sock, _cb);
		}
	}

	nlmsg_free(_msg);
	nl_socket_free(_state->nl80211_sock);

    free(_state);
    _state = NULL;

	return err;

nla_put_failure:
	if(_msg != NULL)
	{
		nlmsg_free(_msg);
	}

	if(_cb != NULL)
	{
		nl_cb_put(_cb);
	}

	if(_state)
	{
		nl_socket_free(_state->nl80211_sock);
		free(_state);
		_state = NULL;
	}


	return -1;
}

nl80211_t* nl80211_socket_creat(void)
{
	nl80211_t *state;
	state = (nl80211_t *)malloc(sizeof(nl80211_t));
	memset(state, 0, sizeof(nl80211_t));

	state->nl80211_sock = nl_socket_alloc();
	
	if(!state->nl80211_sock)
	{
		goto nla_put_failure;
	}

	nl_socket_set_buffer_size(state->nl80211_sock, 2048, 2048);
	if(genl_connect(state->nl80211_sock))
	{
		goto nla_put_failure;	
	}

	state->nl80211_id = genl_ctrl_resolve(state->nl80211_sock, "nl80211");
	if(state->nl80211_id < 0)
	{
		goto nla_put_failure;
	}

	return state;

nla_put_failure:
	if(!state->nl80211_sock)
	{
		nl_socket_free(state->nl80211_sock);
	}

	if(!state)
	{
		free(state);
		state = NULL;
	}

	return NULL;

}

struct nl_msg* nl80211_scan_msg_alloc(nl80211_t *_state, const char *_devname)
{
	if(!_state)
	{
		goto nla_put_failure;
	}

	struct nl_msg *scan_msg;
	scan_msg = nlmsg_alloc();

	if(!scan_msg)
	{
		goto nla_put_failure;
	}

	genlmsg_put(scan_msg, 0, 0, _state->nl80211_id, 0, 0, NL80211_CMD_TRIGGER_SCAN, 0);

	int devidx = if_nametoindex(_devname);
	NLA_PUT_U32(scan_msg, NL80211_ATTR_IFINDEX, devidx);

	return scan_msg;

nla_put_failure:
	return NULL;

}

struct nl_msg* nl80211_scan_msg_add_ssid(struct nl_msg *_msg, uint8_t *_ssid_data, uint8_t _ssid_len)
{
	if(!_msg)
	{
		goto nla_put_failure;
	}

	if(_ssid_len > 32)
	{
		goto nla_put_failure;
	}

	struct nl_msg* ssid = nlmsg_alloc();
	if(!ssid)
	{
		goto nla_put_failure;
	}

	NLA_PUT(ssid, 0, _ssid_len, _ssid_data);
	nla_put_nested(_msg, NL80211_ATTR_SCAN_SSIDS, ssid);

	return _msg;

nla_put_failure:
	return NULL;
}

struct nl_msg* nl80211_scan_msg_add_ie(struct nl_msg *_msg, uint8_t *_ie_data, int _ie_len)
{
	if(!_msg)
	{
		goto nla_put_failure;
	}

	NLA_PUT(_msg, NL80211_ATTR_IE, _ie_len, _ie_data);

	return _msg;

nla_put_failure:
	return NULL;
}

int nl80211_get_mac(const char *_devname, uint8_t *_mac)
{
    if(!_devname || !_mac)
    {
        goto nla_put_failure;
    }

    nl80211_t *state = nl80211_socket_creat();

    if(!state)
    {
        return -1;
    }

    struct nl_msg *get_msg;
    get_msg = nlmsg_alloc();

    if(!get_msg)
    {
        goto nla_put_failure;
    }

    genlmsg_put(get_msg, 0, 0, state->nl80211_id, 0, 0, NL80211_CMD_GET_INTERFACE, 0);

    int devidx = if_nametoindex(_devname);
    NLA_PUT_U32(get_msg, NL80211_ATTR_IFINDEX, devidx);

    struct nl_cb *cb = nl_cb_alloc(NL_CB_DEFAULT);
    if (!cb) {
        goto nla_put_failure;
    }

    nl_cb_set(cb, NL_CB_VALID, NL_CB_CUSTOM, getmac_handle, _mac);

    int ret = handle_cmd(state, get_msg, cb);

    return ret;

nla_put_failure:

    if(!state)
    {
        free(state);
        state = NULL;
    }

    return -1;
}

int nl80211_interface_add(const char *_devname, const char *_newdevname, int _mode)
{
    if(!_devname || !_newdevname)
    {
        goto nla_put_failure;
    }

    nl80211_t *state = nl80211_socket_creat();

    if(!state)
    {
        return -1;
    }

    struct nl_msg *msg;
    msg = nlmsg_alloc();

    if(!msg)
    {
        goto nla_put_failure;
    }

    genlmsg_put(msg, 0, 0, state->nl80211_id, 0, 0, NL80211_CMD_NEW_INTERFACE, 0);

    int devidx = if_nametoindex(_devname);
    NLA_PUT_U32(msg, NL80211_ATTR_IFINDEX, devidx);

    //设置新设备名称及类型
    NLA_PUT_STRING(msg, NL80211_ATTR_IFNAME, _newdevname);
    NLA_PUT_U32(msg, NL80211_ATTR_IFTYPE, _mode);

    int ret = handle_cmd(state, msg, NULL);

    return ret;

nla_put_failure:
    
    if(!state)
    {
        free(state);
        state = NULL;
    }

    return -1;
}

int nl80211_interface_del_forname(const char *_devname)
{
    if(!_devname)
    {
        goto nla_put_failure;
    }

    nl80211_t *state = nl80211_socket_creat();

    if(!state)
    {
        return -1;
    }

    struct nl_msg *msg;
    msg = nlmsg_alloc();

    if(!msg)
    {
        goto nla_put_failure;
    }

    genlmsg_put(msg, 0, 0, state->nl80211_id, 0, 0, NL80211_CMD_DEL_INTERFACE, 0);

    int devidx = if_nametoindex(_devname);
    NLA_PUT_U32(msg, NL80211_ATTR_IFINDEX, devidx);

    int ret = handle_cmd(state, msg, NULL);

    return ret;

nla_put_failure:
    
    if(!state)
    {
        free(state);
        state = NULL;
    }

    return -1;
}

int nl80211_interface_del_foridx(int _idx)
{

    nl80211_t *state = nl80211_socket_creat();

    struct nl_msg *msg;
    msg = nlmsg_alloc();

    if(!msg)
    {
        goto nla_put_failure;
    }

    genlmsg_put(msg, 0, 0, state->nl80211_id, 0, 0, NL80211_CMD_DEL_INTERFACE, 0);

    NLA_PUT_U32(msg, NL80211_ATTR_IFINDEX, _idx);

    int ret = handle_cmd(state, msg, NULL);

    return ret;

nla_put_failure:
    
    return -1;
}

int nl80211_interface_dev_up(const char *_devname, short _flags)
{
    struct ifreq ifr;
    int skfd;
 
    if ((skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
    {
        return -1;
    } 
    
    // Fetch interface flags
    memset(&ifr, 0, sizeof ifr);
    strncpy(ifr.ifr_name, _devname, sizeof(ifr.ifr_name) - 1);
    ifr.ifr_flags = IFF_UP + _flags;
    
    if(ioctl(skfd, SIOCSIFFLAGS, &ifr) < 0) 
    {
        close(skfd);
        return -1;
    }

    close(skfd);

    return 0;
}

#endif
