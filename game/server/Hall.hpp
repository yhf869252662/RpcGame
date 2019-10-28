#pragma once

#include <iostream>
#include <pthread.h>

#include "Log.hpp"
#include "PlayerManager.hpp"
#include "RoomManager.hpp"

using namespace std;

#define MATCH_LEVEL 101

class Hall
{
private:
	PlayerManager pm;
	RoomManager rm;
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
	void IncMatchNum()
	{
		++match_num;
	}
	void DecMatchNum()
	{
		--match_num;
	}
	void ReSetMatchNum()
	{
		match_num = 0;
	}
	uint32_t Register(string& name, string& passwd)
	{
		pm.InsertPlayer(name, passwd);	
	}
	uint32_t Login(uint32_t& id, string& passwd)
	{
		pm.SearchPlayer(id, passwd);
	}
	bool PushIdInMatchPool(uint32_t& id)
	{
		LOG(INFO, "用户放入匹配池中......");
		pm.SetMatch(id);
		LockMatchPool();
		int rate = pm.GetRate(id);
		auto& v = match_pool[rate];
		for(auto& i: v)
		{
			if (i == id)
			{
				UnlockMatchPool();
				return false;
			}
		}
		v.push_back(id);
		IncMatchNum();
		//有用户了尝试唤醒，看是否可以进行匹配
		ServiceWakeup();
		UnlockMatchPool();
		return true;
	}
	bool PopIdMatchPool(uint32_t& id)
	{
		LockMatchPool();
		int rate = pm.GetRate(id);
		auto& v = match_pool[rate];
		for (auto it=v.begin(); it!=v.end(); ++it)
		{
			if (*it == id)
			{
				v.erase(it);
				break;
			}
		}
		DecMatchNum();
		UnlockMatchPool();

		pm.SetOnline(id);
		return true;
	}
	int IsPlayerReady(uint32_t& id)
	{
		return pm.Ready(id);
	}
	void MatchPoolClear(uint32_t& id)
	{
		LOG(INFO, "匹配池清空......");
		for (auto it=match_pool.begin(); it!=match_pool.end(); ++it)
		{
			auto& v = *it;
			if (v.empty())
				continue;
			vector<uint32_t>().swap(v);//与将死的空数组交换
		}
		ReSetMatchNum();
		//剩下的一个用户重新等待
		if (id > 10000)
			pm.SetOnline(id);
	}
	void GetAllMatchId(vector<uint32_t>& id_list)
	{
		//遍历每一个等级的数组
		for (int i=MATCH_LEVEL-1; i>=0; --i)
		{
			auto& v = match_pool[i];
			if (v.empty())
				continue;
			//按等级由高到低存入匹配序列中
			for (auto it=v.begin(); it!=v.end(); ++it)
			{
				id_list.push_back(*it);
			}
		}
	}
	void GamePrepare(uint32_t& one, uint32_t& two)
	{
		uint32_t room_id = rm.CreateRoom(one, two);
		pm.SetPlayerStatus(one, two);
		pm.SetPlayRoom(room_id, one, two);
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
			
			uint32_t last = 0;
			//将等级相近的匹配
			vector<uint32_t> id_list;
			hp->GetAllMatchId(id_list);
			size_t num = id_list.size();
			//剩余一人等待再次匹配
			if (num & 1)
			{
				last = id_list[num-1];
				num -= 1;
			}
			else
			{
				last = 0;
			}
			
			for (auto i=0; i<num; i+=2)
			{
				uint32_t one = id_list[i];
				uint32_t two = id_list[i+1];
				hp->GamePrepare(one, two);
			}

			hp->MatchPoolClear(last);
			hp->UnlockMatchPool();
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

