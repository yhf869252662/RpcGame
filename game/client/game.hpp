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
        cout<<"********************************"<<endl;
        cout<<"***1.五子棋        0.退出 ******"<<endl;
        cout<<"********************************"<<endl;
        cout<<"请选择：";
}
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
