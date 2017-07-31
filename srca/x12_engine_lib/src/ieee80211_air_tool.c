/*
 *
 * Copyright (C) 刘晓龙
 * Copyright (C) 昆明黑海科技有限公司
 *
 */


/*
 * 
 * 80211空气广播工具
 *
 * 作者:Crow
 * 时间:2017-05-01
 * 最后修改时间:2017-05-01
 *      
 */

#include "ieee80211_air_tool.h"

unsigned char* ieee80211_air_tool_scan_gettag_index(const unsigned char *_data, int _datalen, const unsigned char _tag, int *_taglen, int _index)
{
	if(!_data || !_taglen)
	{
		return NULL;
	}

	const unsigned char *data80211 = _data;

	if(data80211[0] != 0x00)
	{
		return NULL;
	}

	unsigned short headlen = 0;
	memcpy(&headlen, data80211 + 2, 2);

	unsigned char msgtype = 0;
	memcpy(&msgtype, data80211 + headlen, 1);

	if(msgtype != 0x40)
	{
		return NULL;
	}

	int i;
	unsigned char tagnum;
	unsigned char taglen;
	int index = 0;
	
	const unsigned char *parament = data80211 + headlen + 1 + 1 + 2 + 6 + 6 + 6 + 1 + 1;
	int paramentlen = _datalen - headlen - 1 - 1 - 2 - 6 - 6 - 6 - 1 - 1;

	for(i = 0; i < paramentlen; i++)
	{
		memcpy(&tagnum, parament + i, sizeof(tagnum));

		i++;
		memcpy(&taglen, parament + i, sizeof(taglen));
		
		if(tagnum != _tag)
		{
			i += taglen;
			taglen = 0;
		}
		else
		{

			if(index == _index)
			{
				i++;
				break;
			}
			else
			{
				index++;
			}
		}
	}

	unsigned char *tagmsg = (unsigned char *)malloc(taglen);
	memcpy(tagmsg, parament + i, taglen);

	*_taglen = taglen;

	if(taglen == 0)
	{
		return NULL;
	}

	return tagmsg;
}

int ieee80211_air_tool_scan_gettagcount(const unsigned char *_data, int _datalen, const unsigned char _tag)
{
	if(!_data)
	{
		return -1;
	}

	const unsigned char *data80211 = _data;

	if(data80211[0] != 0x00)
	{
		return -1;
	}

	unsigned short headlen = 0;
	memcpy(&headlen, data80211 + 2, 2);

	unsigned char msgtype = 0;
	memcpy(&msgtype, data80211 + headlen, 1);

	if(msgtype != 0x40)
	{
		return -1;
	}

	int i;
	unsigned char tagnum;
	int taglen;
	
	const unsigned char *parament = data80211 + headlen + 1 + 1 + 2 + 6 + 6 + 6 + 1 + 1;
	int paramentlen = _datalen - headlen - 1 - 1 - 2 - 6 - 6 - 6 - 1 - 1;

	int count = 0;

	for(i = 0; i < paramentlen; i++)
	{
		memcpy(&tagnum, parament + i, sizeof(tagnum));

		i++;
		memcpy(&taglen, parament + i, sizeof(taglen));
		
		if(tagnum != _tag)
		{
			i += taglen;
			taglen = 0;
		}
		else
		{
			count++;
		}
	}

	return count;
}

unsigned char* ieee80211_air_tool_scan_gettag_merge(const unsigned char *_data, int _datalen, const unsigned char _tag, int *_taglen)
{
	if(!_data || !_taglen)
	{
		*_taglen = 0;
		return NULL;
	}

	const unsigned char *data80211 = _data;

	if(data80211[0] != 0x00)
	{
		*_taglen = 0;
		return NULL;
	}

	unsigned short headlen = 0;
	memcpy(&headlen, data80211 + 2, 2);

	unsigned char msgtype = 0;
	memcpy(&msgtype, data80211 + headlen, 1);

	if(msgtype != 0x40)
	{
		*_taglen = 0;
		return NULL;
	}

	int i;
	unsigned char tagnum;
	unsigned char taglen;
	
	const unsigned char *parament = data80211 + headlen + 1 + 1 + 2 + 6 + 6 + 6 + 1 + 1;
	int paramentlen = _datalen - headlen - 1 - 1 - 2 - 6 - 6 - 6 - 1 - 1;

	int tagmsgsize = 0;
	int datablock = 256;
	unsigned char *tagmsg;
	int tagmsglen = 0;

	tagmsgsize += datablock;
	tagmsg = (unsigned char *)malloc(tagmsgsize);

	for(i = 0; i < paramentlen; i++)
	{
		memcpy(&tagnum, parament + i, sizeof(tagnum));
		i++;
		memcpy(&taglen, parament + i, sizeof(taglen));
		
		if(tagnum != _tag)
		{
			i += taglen;
			taglen = 0;
		}
		else
		{
			i++;
		
			if(tagmsgsize < tagmsglen + taglen)
			{
				tagmsgsize += datablock;
				tagmsg = (unsigned char *)realloc(tagmsg, tagmsgsize);
			}

			memcpy(tagmsg + tagmsglen, parament + i, taglen);
			tagmsglen += taglen;
			
			i += taglen - 1;
		}
	}

	*_taglen = tagmsglen;

	if(tagmsglen == 0)
	{
		return NULL;
	}

	return tagmsg;
}

char ieee80211_air_tool_scan_getsignal(const unsigned char *_data, int _datalen)
{
	if(!_data)
	{
		return 1;
	}

	const unsigned char *data80211 = _data;

	if(data80211[0] != 0x00)
	{
		return 1;
	}

	unsigned short headlen = 0;
	memcpy(&headlen, data80211 + 2, 2);

	unsigned char msgtype = 0;
	memcpy(&msgtype, data80211 + headlen, 1);

	if(msgtype != 0x40)
	{
		return 1;
	}

	char signal = 0;

	memcpy(&signal, data80211 + headlen - 2, sizeof(signal));

	return signal;
}

char ieee80211_air_tool_scan_getsa(const unsigned char *_data, int _datalen, unsigned char _sa[6])
{
	if(!_data || !_sa)
	{
		return -1;
	}

	const unsigned char *data80211 = _data;

	if(data80211[0] != 0x00)
	{
		return -1;
	}

	unsigned short headlen = 0;
	memcpy(&headlen, data80211 + 2, 2);

	unsigned char msgtype = 0;
	memcpy(&msgtype, data80211 + headlen, 1);

	if(msgtype != 0x40)
	{
		return -1;
	}

	memcpy(_sa, data80211 + headlen + 4 + 6, 6);

	return 0;
}

char ieee80211_air_tool_scan_getda(const unsigned char *_data, int _datalen, unsigned char _da[6])
{
	if(!_data || !_da)
	{
		return -1;
	}

	const unsigned char *data80211 = _data;

	if(data80211[0] != 0x00)
	{
		return -1;
	}

	unsigned short headlen = 0;
	memcpy(&headlen, data80211 + 2, 2);

	unsigned char msgtype = 0;
	memcpy(&msgtype, data80211 + headlen, 1);

	if(msgtype != 0x40)
	{
		return -1;
	}

	memcpy(_da, data80211 + headlen + 4 + 6 + 6, 6);

	return 0;
}

int ieee80211_air_tool_scan_senddata(const char *_driver, unsigned char *_ssid, unsigned char _ssidlen, char _tag, unsigned char *_tagdata, int _taglen)
{

#ifdef _OS_LINUX_

	if(!_driver)
	{
		return -1;
	}

	nl80211_t *state = nl80211_socket_creat();

	if(!state)
	{
		return -1;
	}

	void* scanmsg = nl80211_scan_msg_alloc(state, _driver);

	if(!scanmsg)
	{
		return -1;
	}

	if(_ssid)
	{
		nl80211_scan_msg_add_ssid(scanmsg, _ssid, _ssidlen);
	}
	else
	{
		nl80211_scan_msg_add_ssid(scanmsg, NULL, 0);
	}
    
	if(_tagdata)
	{

		unsigned char groupsize = 240;
		int group = _taglen / groupsize;
		int lastdata = _taglen % groupsize; 

		int i;

		unsigned char *ies;
		int blocksize = 256;
		int ieslen = 0;
		int iessize = 0;

		iessize += blocksize;
		ies = (unsigned char *)malloc(iessize);

		for(i = 0; i < group; i++)
		{
			if(iessize < i * groupsize + ieslen + 2)
			{
				iessize += blocksize;
				ies = (unsigned char *)realloc(ies, iessize);
			}

			ies[ieslen++] = _tag;
			ies[ieslen++] = groupsize;
			memcpy(ies + ieslen, _tagdata + (i * groupsize), groupsize);
			ieslen += groupsize;
		}

		if(iessize < lastdata + ieslen + 2)
		{
			iessize += blocksize;
			ies = (unsigned char *)realloc(ies, iessize);
		}

		ies[ieslen++] = _tag;
		ies[ieslen++] = lastdata;
		memcpy(ies + ieslen, _tagdata + (i * groupsize), groupsize);
		ieslen += lastdata;

		nl80211_scan_msg_add_ie(scanmsg, ies, ieslen);

		if(ies != NULL)
		{
			free(ies);
			ies = NULL;
		}

	}
	
	int ret = handle_cmd(state, scanmsg, NULL);
	
	return ret;

#else
	printf("this fun work on linux!\n");
	return 0;
#endif

}

