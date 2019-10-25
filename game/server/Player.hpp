#pragma once

#include <iostream>
#include <string>

using namespace std;

typedef enum{
	OFFLINE = 0,
	ONLINE,
	MATCHING,
	PLAYING
}status_t;

class Player
{
private:
	string name;
	string passwd;
	uint32_t id;
	int win;
	int lose;
	int tie;//平局
	status_t st;//用户当前状态
	uint32_t room_id;

public:
	Player()
	{
	}
	Player(string& _name,string& _passwd,uint32_t& _id) : name(_name), passwd(_passwd), id(_id)
	{
		win = 0;
		lose = 0;
		tie = 0;
		st = OFFLINE;//刚注册完毕，是离线状态
		room_id = 0;
	}
	const string& Passwd()
	{
		return passwd;
	}
	int Status()
	{
		return st;
	}
	int Rate()//当前用户胜率
	{
		int total = win + lose;
		if (total == 0)
			return 0;
		return win*100/total;//n% n在0--100之间
	}
	uint32_t Room()
	{
		return room_id;
	}
	void Online()
	{
		st = ONLINE;
	}
	void Matching()
	{
		st = MATCHING;
	}
	void Playing()
	{
		st = PLAYING;
	}
	void SetRoom(uint32_t& _roomid)
	{
		room_id = _roomid;
	}
	~Player()
	{
	}
};
