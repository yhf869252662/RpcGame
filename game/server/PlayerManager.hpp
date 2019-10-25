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
	
	~PlayerManager()
	{
		pthread_mutex_destroy(&lock);
	}
};
