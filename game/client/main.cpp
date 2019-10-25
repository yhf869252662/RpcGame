#include <iostream>

#include "game.hpp"

using namespace std;

static void LoginMenu()
{
	cout << "************************************" << endl;
	cout << "******  1.登录       2.注册    *****" << endl;
	cout << "******               0.退出    *****" << endl;
	cout << "************************************" << endl;
	cout << "请选择: >";
}

static void Usage(string proc)
{
	cout << "Usage: " << proc << " ip port" << endl;
}

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		Usage(argv[0]);
		return 1;
	}
	string ip = argv[1];
	int port = atoi(argv[2]);

	string name;
	string passwd;
	uint32_t id;
	int select = 0;
	while (1)
	{
		LoginMenu();
		cin >> select;
		switch(select)
		{
			case 1:

				break;
			case 2:
				{
					Register(ip, port, name, passwd, id);
					if (id >= 10000)
					{
						cout << "注册成功！请记住您的Id: [" << id << "]" << endl;
						cout << "请直接登录" <<endl;
					}
					else
					{
						cout << "注册失败！请稍后再试"<<endl;
					}
				}
				break;
			case 0:
				cout << "客户端退出......."<<endl;
				exit(2);
				break;
			default:
				break;
		}
	}


	return 0;
}
