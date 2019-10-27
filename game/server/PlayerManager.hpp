#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <pthread.h>

#include "Player.hpp"
#include "Log.hpp"

using namespace std;

#define DEFAULT_ID 10000

class PlayerManager
{
private:
	std::unordered_map<uint32_t,Player> player_set;
	uint32_t assign_id;//分配账号
	pthread_mutex_t lock;
public:
	PlayerManager(): assign_id(DEFAULT_ID)
	{
		pthread_mutex_init(&lock,NULL);
	}
	void Lock()
	{
		pthread_mutex_lock(&lock);
	}
	void Unlock()
	{
		pthread_mutex_unlock(&lock);
	}
	uint32_t InsertPlayer(string& _name, string& _passwd)
	{
		Lock();
		uint32_t id = assign_id++;
		Player player(_name, _passwd, id);
		player_set.insert(make_pair(id, player));
		Unlock();
		//cout << "id = " << id << endl;
		LOG(INFO, "玩家注册成功......");
		return id;
	}
	void SetOnline(uint32_t& id)
	{
		Lock();
		player_set[id].Online();
		Unlock();
	}
	uint32_t SearchPlayer(uint32_t& id, string& passwd)
	{
		Lock();
		auto itpl = player_set.find(id);
		if(itpl == player_set.end())
		{
			LOG(WARNING, "用户不存在......");
			Unlock();
			return 1;
		}

		Player& pl = itpl->second;
		if (pl.Passwd() != passwd)
		{
			LOG(WARNING, "密码错误......");
			Unlock();
			return 2;
		}
		pl.Online();
		Unlock();
		LOG(INFO, "用户登录成功......");
		return id;
	}
	void SetPlayerStatus(uint32_t& one, uint32_t& two)
	{
		Lock();
		player_set[one].Playing();
		player_set[two].Playing();
		Unlock();
	}
	void SetMatch(uint32_t& id)
	{
		Lock();
		player_set[id].Matching();
		Unlock();
	}
	void SetPlayRoom(uint32_t& room_id, uint32_t& one, uint32_t& two)
	{
		Lock();
		player_set[one].SetRoom(room_id);
		player_set[two].SetRoom(room_id);
		Unlock();
	}
	int Ready(uint32_t& id)
	{
		Lock();
		int st = player_set[id].Status();
		Unlock();
		return st;
	}
	int GetRate(uint32_t& id)
	{
		Lock();
		int ra = player_set[id].Rate();
		Unlock();
		return ra;
	}
	int GetPlayerRoomId(uint32_t& id)
	{
		Lock();
		int room_id = player_set[id].Room();
		Unlock();
		return room_id;
	}
	~PlayerManager()
	{
		pthread_mutex_destroy(&lock);
	}
};
