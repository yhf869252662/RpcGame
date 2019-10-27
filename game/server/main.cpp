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
uint32_t RpcLogin(connection* conn, uint32_t id, string passwd)//conn里面应该包含socket
{
    return GameHall.Login(id, passwd);
}
uint32_t RpcMatchAndWait(connection* conn, uint32_t id)//conn里面应该包含socket
{
    return GameHall.PushIdInMatchPool(id);
}
uint32_t RpcPopMatchPool(connection* conn, uint32_t id)//conn里面应该包含socket
{
    return GameHall.PopIdMatchPool(id);
}
uint32_t RpcPlayerReady(connection* conn, uint32_t id)//conn里面应该包含socket
{
    return GameHall.IsPlayerReady(id);
}

int main()
{
	rpc_server server(8081, 4);//绑定端口号,底层服务线程数
	LOG(INFO, "服务器初始化完成......");

    server.register_handler("RpcRegister", RpcRegister);
    server.register_handler("RpcLogin", RpcLogin);
    server.register_handler("RpcMatchAndWait", RpcMatchAndWait);
    server.register_handler("RpcPopMatchPool", RpcPopMatchPool);
    server.register_handler("RpcPlayerReady", RpcPlayerReady);
	LOG(INFO, "所有方法注册完毕......");

	server.run();
	LOG(INFO, "服务器启动成功......");

	GameHall.InitHall();
	
	string str;
	cin >> str;

	return 0;
}
