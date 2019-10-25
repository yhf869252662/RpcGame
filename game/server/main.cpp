#include <iostream>
#include <rpc_server.h>
#include <string>
using namespace rest_rpc;
using namespace rpc_service;
#include <fstream>
#include "Hall.hpp"

using namespace std;

Hall GameHall;

uint32_t RpcRegister(connection* conn, string name, string passwd)//conn里面应该包含socket
{
    return GameHall.Register(name, passwd);
}

int main()
{
	rpc_server server(8081, 4);//绑定端口号,底层服务线程数
	LOG(INFO, "服务器初始化完成......");

    server.register_handler("RpcRegister", RpcRegister);
	LOG(INFO, "所有方法注册完毕......");

	server.run();
	LOG(INFO, "服务器启动成功......");

	GameHall.InitHall();
	
	string str;
	cin >> str;

	return 0;
}
