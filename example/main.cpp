#include "inireader.h"
#include <iostream>


int main()
{

	lxr::ini::IniReader ini_reader("./conf.ini");

	std::string ip = ini_reader["server"]["ip"].toString();
	
	int port = ini_reader["server"]["port"].toInt();
	
	float timeout = ini_reader["server"]["timeout"].toFloat(50.0);

	std::cout << "ip:" << ip << std::endl;
	std::cout << "port:" << port << std::endl;
	std::cout << "timeout:" << timeout << std::endl;

	return 0;
}
