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
	
uint32_t Register(string& ip, int& port, string& name, string& passwd, uint32_t& id)
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
