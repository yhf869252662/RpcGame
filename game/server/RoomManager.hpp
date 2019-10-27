#pragma once

#include <iostream>
#include <unordered_map>
#include <pthread.h>

#include "Room.hpp"

using namespace std;

#define ROOM_ID 1024

class RoomManager
{
private:
	unordered_map<uint32_t, Room> room_set;
	uint32_t assgin_id;
	pthread_mutex_t lock;
public:
	RoomManager() : assgin_id(ROOM_ID)
	{
		pthread_mutex_init(&lock, NULL);
	}
	void Lock()
	{
		pthread_mutex_lock(&lock);
	}
	void Unlock()
	{
		pthread_mutex_unlock(&lock);
	}
	uint32_t CreateRoom(uint32_t& one, uint32_t& two)
	{
		Lock();
		Room proom(one, two);
		uint32_t roomid = assgin_id++;
		room_set.insert(make_pair(roomid, proom));
		Unlock();
		return roomid;
	}
	~RoomManager()
	{
		pthread_mutex_destroy(&lock);
	}
};
