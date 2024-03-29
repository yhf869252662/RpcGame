#pragma once

#include <iostream>
#include<string>
#include<stdlib.h>
#include <rpc_client.hpp>
#include <chrono>
#include <fstream>
#include "codec.h"
using namespace std;
using namespace std::chrono_literals;

using namespace rest_rpc;
using namespace rest_rpc::rpc_service;

#define SIZE 5
	
uint32_t Register(const string& ip, const int& port, string& name, string& passwd, uint32_t& id)
{
	cout << "请输入昵称: >";
	cin >> name;
	while (1)
	{
		cout << "请输入密码: >";
		cin >> passwd;
		string pw;
		cout << "请再次输入密码: >";
		cin >> pw;
		if (pw == passwd)
			break;
		else
			cout << "两次密码不同，请重新输入！"<<endl;
	}
	
	try {
		rpc_client client(ip, port);
		bool r = client.connect();
		if (!r) {
			std::cout << "connect timeout" << std::endl;
			return 2;
		}
        id = client.call<uint32_t>("RpcRegister", name, passwd);
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
    return 0;

}
uint32_t Login(const string& ip, const int& port)
{
	uint32_t id;
	string passwd;
	cout << "请输入Id: >";
	cin >> id;
	cout << "请输入密码: >";
	cin >> passwd;
	
	uint32_t ret = 0;

	try {
		rpc_client client(ip, port);
		bool r = client.connect();
		if (!r) {
			std::cout << "connect timeout" << std::endl;
			return 2;
		}
        ret = client.call<uint32_t>("RpcLogin", id, passwd);
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}

	if(ret < 10000)
	{
		cout << "用户名或密码错误" << endl;
	}

	return ret;
}
void GameMenu()
{
	//printf("\ec");
    cout<<"********************************"<<endl;
    cout<<"***1.五子棋        0.退出 ******"<<endl;
    cout<<"********************************"<<endl;
    cout<<"请选择：";
}
bool PushMatchPool(const string& ip, const int& port, uint32_t& id)
{
	try {
		rpc_client client(ip, port);
		bool r = client.connect();
		if (!r) {
			std::cout << "connect timeout" << std::endl;
			return 2;
		}
        return client.call<uint32_t>("RpcMatchAndWait", id);
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}
bool PopMatchPool(const string& ip, const int& port, uint32_t& id)
{
	try {
		rpc_client client(ip, port);
		bool r = client.connect();
		if (!r) {
			std::cout << "connect timeout" << std::endl;
			return 2;
		}
        return client.call<uint32_t>("RpcPopMatchPool", id);
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}
uint32_t CheckReady(const string& ip, const int& port, uint32_t& id)
{
	try {
		rpc_client client(ip, port);
		bool r = client.connect();
		if (!r) {
			std::cout << "connect timeout" << std::endl;
			return 2;
		}
        return client.call<uint32_t>("RpcPlayerReady", id);
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}
bool Match(const string& ip, const int& port, uint32_t& id)
{
	PushMatchPool(ip, port, id);
	int count = 20;

	while (1)
	{
		int ret = CheckReady(ip, port, id);
		//cout << "ret = " << ret << endl;
		
		switch (ret)
		{
		case 3:
			//匹配成功
			return true;
			break;
		case 2:
			//还在匹配中
			printf("匹配中------%2d\r", count--);//倒计时
			fflush(stdout);
			if(count <= 0)
			{
				cout << "匹配超时!" <<endl;
				PopMatchPool(ip, port, id);
				return false;
			}
			sleep(1);
			break;
		case 1:
			return false;
			//匹配失败
			break;
		default:
			break;
		}
	}
	return false;

}
uint32_t GetRoomId(const string& ip, const int& port, uint32_t& id)
{
	try {
		rpc_client client(ip, port);
		bool r = client.connect();
		if (!r) {
			std::cout << "connect timeout" << std::endl;
			return 2;
		}
        return client.call<uint32_t>("RpcRoomId", id);
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}
char GetMyPiece(const string& ip, const int& port, uint32_t& room_id, uint32_t& id)
{
	try {
		rpc_client client(ip, port);
		bool r = client.connect();
		if (!r) {
			std::cout << "connect timeout" << std::endl;
			return 2;
		}
        return client.call<char>("RpcPlayerPiece", room_id, id);
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}
bool IsMyTurn(const string& ip, const int& port, uint32_t& room_id, uint32_t& id)
{
	try {
		rpc_client client(ip, port);
		bool r = client.connect();
		if (!r) {
			std::cout << "connect timeout" << std::endl;
			return 2;
		}
        return client.call<bool>("RpcIsMyTurn", room_id, id);
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}
int Step(const string& ip, const int& port, uint32_t& room_id, uint32_t& id, int& x, int& y)
{
	try {
		rpc_client client(ip, port);
		bool r = client.connect();
		if (!r) {
			std::cout << "connect timeout" << std::endl;
			return 2;
		}
        client.call<void>("RpcStep", room_id, id, x, y);
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}
char Judge(const string& ip, const int& port, uint32_t& room_id)
{
	try {
		rpc_client client(ip, port);
		bool r = client.connect();
		if (!r) {
			std::cout << "connect timeout" << std::endl;
			return 2;
		}
        return client.call<char>("RpcJudge", room_id);
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}
int GetBoard(const string& ip, const int& port, uint32_t& room_id, string& board)
{
	try {
		rpc_client client(ip, port);
		bool r = client.connect();
		if (!r) {
			std::cout << "connect timeout" << std::endl;
			return 2;
		}
        board = client.call<string>("RpcBoard", room_id);
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}
void ShowBoard(string& board)
{
	cout <<"  | 1 | 2 | 3 | 4 | 5 |" <<endl;
	cout <<"-----------------------" <<endl;
	for (int i=0; i<SIZE; ++i)
	{
		cout << i+1;
		for (int j=0; j<SIZE; ++j)
		{
			cout << " | " << board[i*SIZE+j];
		}
		cout << " |";
		cout << endl;
		if (i != SIZE-1)
			cout <<"-----------------------" <<endl;
	}
	cout <<"-----------------------" <<endl;
	cout << endl;
}
bool PosIsRight(string& board, int& x, int& y)
{
	if(board[x*SIZE+y] != ' ')
		return false;
	return true;
}
void PlayGobang(const string& ip, const int& port, uint32_t& id)
{
	//system("clear");
	uint32_t room_id = GetRoomId(ip, port, id);
	char piece = GetMyPiece(ip, port, room_id, id);
	string board;

	char ret;
	while((ret = Judge(ip, port, room_id)) == 'N')
	{

		printf("\ec");//清屏
		cout << "房间号:[" << room_id << "] 您执棋子[" << piece << "]" << endl;

		GetBoard(ip, port, room_id, board);
		ShowBoard(board);
		while(!IsMyTurn(ip, port, room_id, id))
		{
			//printf("\ec");
			//cout << "房间号:[" << room_id << "] 您执棋子[" << piece << "]" << endl;
			//ShowBoard(board);
			printf("\033[1A");//先回到上一行
			printf("\033[K");//清除该行
			cout << "等待对方思考......" <<endl;
			sleep(1);
			//continue;
		}
		if((ret = Judge(ip, port, room_id)) != 'N')
		{
			break;
		}
		printf("\ec");
		cout << "房间号:[" << room_id << "] 您执棋子[" << piece << "]" << endl;
		GetBoard(ip, port, room_id, board);
		ShowBoard(board);
		cout << "请落子(x y)> ";
		int x, y;
		cin >> x >> y;
		x -= 1;//对应数组位置
		y -= 1;
		while (x < 0 || x > 4 || y < 0 || y > 4 || !PosIsRight(board, x, y))
		{
			printf("\033[1A");//先回到上一行
			printf("\033[K");//清除该行
			cout << "落子有误，请重新输入!" << endl;
			cout << "请落子(x y)> ";
			cin >> x >> y;
			x -= 1;
			y -= 1;
		}
		Step(ip, port, room_id, id, x, y);
		printf("\ec");
		GetBoard(ip, port, room_id, board);
		ShowBoard(board);
	}
	
	//判断输赢
	if(ret == 'E')
	{
		printf("\ec");
		GetBoard(ip, port, room_id, board);
		ShowBoard(board);
		cout << "平局，请再接再厉吧" << endl;
	}
	else if(ret == piece)
	{
		printf("\ec");
		GetBoard(ip, port, room_id, board);
		ShowBoard(board);
		cout << "恭喜你赢了！！！" << endl;
	}
	else
	{
		printf("\ec");
		GetBoard(ip, port, room_id, board);
		ShowBoard(board);
		cout << "很遗憾，你输了，请再接再厉吧" << endl;
	}
}
//void PlayGobang(const string& ip, const int& port, uint32_t& id)
//{
//    int x,y;
//    char result = 'N';
//    string board;
//    uint32_t room_id = GetRoomId(ip, port, id);
//    cout<<"你的房间号是："<<room_id<<endl;
//    if(room_id < 1024){
//        return;
//    }
//    char piece = GetMyPiece(ip, port, room_id, id);
//    cout<<"你所执棋子是："<<piece<<endl;
//    for( ; ; ){
//        GetBoard(ip, port, room_id, board);//获取棋盘
//        ShowBoard(board);//展示棋盘
//        //判断该谁走
//        if(result = Judge(ip, port, room_id) != 'N'){
//            break;
//        }
//        if(!IsMyTurn(ip, port, room_id, id)){
//            cout<<"对方正在思考......"<<endl;
//            sleep(1);
//            continue;
//        }
//        cout<<"请输入你的落子位置：>";
//        cin>>x>>y;
//        if(x >= 1 && x<=5 && y>=1 && y<=5){
//            if(!PosIsRight(board, x, y)){
//                cout<<"你输入的位置已经被占用，请重新输入！"<<endl;
//            }
//            else{
//                Step(ip, port, room_id, id, x, y);
//                result = Judge(ip, port, room_id);
//                if(result != 'N'){
//                    break;
//                }
//            }
//            }
//        else{
//            cout<<"你输入的位置有误，请重新输入！"<<endl;
//        }
//    }
//    GetBoard(ip, port, room_id, board);//获取棋盘
//    ShowBoard(board);//展示棋盘
//    if(result == 'E'){
//        cout<<"平局，在来一局吧!"<<endl;
//    }
//    else if(result == piece){
//        cout<<"恭喜，你赢了！"<<endl;
//    }
//    else{
//        cout<<"好遗憾，你输了，在来一局吧！"<<endl;
//    }
//}
void Game(const string& ip, const int& port, uint32_t& id)
{
	int select = 0;
	volatile bool quit = false;

	while (!quit)
	{
		GameMenu();
		cin >> select;

		switch(select)
		{
		case 1:
			{
				if(Match(ip, port, id))	
				{
					PlayGobang(ip, port, id);
				}
				else
				{
					cout << "匹配失败，请稍后重试......" << endl;
				}
			}
			break;
		case 0:
			quit = true;
			break;
		default:
			cout << "选择有误，请重新选择!" << endl;
			break;
		}
	}
}
