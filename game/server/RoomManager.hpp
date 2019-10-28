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
    void Step(uint32_t& room_id, uint32_t& id, int& x, int& y)
	{
		Room& r = room_set[room_id];
		Lock();
		r.Step(id, x, y);
		Unlock();
	}
    bool IsMyTurn(uint32_t& room_id, uint32_t& id)
	{
		Room& r = room_set[room_id];
		Lock();
		bool ret = r.IsMyTurn(id);
		Unlock();
		return ret;
	}
    char GetPlayerPiece(uint32_t& room_id, uint32_t& id)
	{
		Room& r = room_set[room_id];
		Lock();
		char ret = r.Piece(id);
		Unlock();
		return ret;
	}
    char Judge(uint32_t& room_id, uint32_t& id)
	{
		Room& r = room_set[room_id];
		Lock();
		char ret = r.GameResult();
		Unlock();
		return ret;
	}
    void GetBoard(uint32_t& room_id, string& board)
	{
		Room& r = room_set[room_id];
		Lock();
		r.Board(board);
		Unlock();
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
