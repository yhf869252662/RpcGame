#pragma once

#include <iostream>
#include <string.h>

using namespace std;

#include "Player.hpp"

#define BLACK 'X'
#define WHITE 'O'
#define SIZE 5

class Room
{
private:
	uint32_t one;  //'X'
	uint32_t two;  //'O'
	char piece[2];
	uint32_t current; //该谁走
	char board[SIZE][SIZE];
	char result; //X黑旗赢,O白旗赢,E平局,N正在对局
public:
	Room()
	{
	}
	Room(uint32_t& _one, uint32_t& _two): one(_one),two(_two)
	{
		piece[0] = 'X';
		piece[1] = 'O';
		memset(board, ' ', sizeof(board));
		current = one;//默认玩家一先走
		result = 'N';
	}
	bool IsMyTurn(uint32_t& id)
	{
		//cout << "current: " << current << "id: " << id << endl;
		return id == current ? true : false;
	}
	char Piece(uint32_t& id)
	{
		return id == one ? piece[0] : piece[1];
	}
	char GameResult()
	{
		return result;
	}
	void Board(string& _board)
	{
		for(int i=0; i<SIZE; ++i)
		{
			for(int j=0; j<SIZE; ++j)
			{
				_board.push_back(board[i][j]);
			}
		}
	}
	void Step(uint32_t& id, int& x, int& y)
	{
		if(current == id)
		{
			int pos = (id == one ? 0 : 1);
			board[x][y] = piece[pos];
			current = (current == one ? two : one);
			result = Judge();
		}
	}
	char Judge()
	{	
		//判断行
		for(int i=0; i<SIZE; ++i)
		{
			int count = 0;
			for(int j=0; j<SIZE; ++j)
			{
				count += board[i][j];
			}
			if(count/SIZE == board[i][0])
			{
				return board[i][0];
			}
		}

		//判断列
		for(int j=0; j<SIZE; ++j)
		{
			int count = 0;
			for(int i=0; i<SIZE; ++i)
			{
				count += board[i][j];
			}
			if(count/SIZE == board[0][j])
			{
				return board[0][j];
			}
		}

		//判断第一条对角线
	    int count = 0;
		for(int i=0, j=0; i<SIZE && j<SIZE; ++i, ++j)
		{
			count += board[i][j];
		}
		if (count/SIZE == board[0][0])
		{
			return board[0][0];
		}
		
		//判断第二条对角线
		count = 0;
		for(int i=0, j=SIZE-1; i<SIZE && j>=0; ++i, --j)
		{
			count += board[i][j];
		}
		if(count/SIZE == board[SIZE-1][0])
		{
			return board[SIZE-1][0];
		}

		//还没下完
		for(int i=0; i<SIZE; ++i)
		{
			for(int j=0; j<SIZE; ++j)
			{
				if(board[i][j] == ' ')
					return 'N';
			}
		}

		return 'E';
	}
	~Room()
	{
	}

};
