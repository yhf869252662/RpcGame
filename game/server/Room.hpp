#pragma once

#include <iostream>

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

	}
	~Room()
	{
	}

};
