#pragma once

#include <iostream>
#include <pthread.h>

#include "Log.hpp"
#include "PlayerManager.hpp"

using namespace std;

#define MATCH_LEVEL 101

class Hall
{
private:
	PlayerManager pm;
	vector<vector<uint32_t>> match_pool;
	int match_num;//匹配池人数
	pthread_mutex_t match_lock;
	pthread_cond_t match_cond;
public:
	Hall(): match_pool(MATCH_LEVEL), match_num(0)
	{
	}
	void LockMatchPool()
	{
		pthread_mutex_lock(&match_lock);
	}
	void UnlockMatchPool()
	{
		pthread_mutex_unlock(&match_lock);
	}
	void ServiceWait()
	{
		pthread_cond_wait(&match_cond, &match_lock);
	}
	void ServiceWakeup()
	{
		pthread_cond_signal(&match_cond);
	}
	int MatchNum()
	{
		return match_num;
	}
	uint32_t Register(string& name, string& passwd)
	{
		pm.InsertPlayer(name, passwd);	
	}
	uint32_t Login(uint32_t& id, string& passwd)
	{
		pm.SearchPlayer(id, passwd);
	}
	static void* MatchService(void *arg)
	{
		pthread_detach(pthread_self());//线程分离
		Hall* hp = (Hall*)arg;
		while (1)
		{
			hp->LockMatchPool();
			while(hp->MatchNum() < 2)
			{
				LOG(INFO, "服务线程等待......");
				hp->ServiceWait();
			}
			LOG(INFO, "服务线程被唤醒......");
		}
	}
	void InitHall()
	{
		pthread_mutex_init(&match_lock, NULL);
		pthread_cond_init(&match_cond, NULL);

		pthread_t tid;
		pthread_create(&tid, NULL, MatchService, this);
	}
	~Hall()
	{
		pthread_mutex_destroy(&match_lock);
		pthread_cond_destroy(&match_cond);
	}
};

