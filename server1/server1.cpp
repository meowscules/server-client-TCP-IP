#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <fstream>
#include "server1.h"

#pragma warning(disable: 4996)

void recvFile(SOCKET* sock);

int main(int argc, char* argv[]) {
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		std::cout << "Error" << std::endl;
		exit(1);
	}

	int PORT = atoi(argv[2]);

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr(argv[1]);
	addr.sin_port = htons(PORT);
	addr.sin_family = AF_INET;

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN);

	SOCKET newConnection;
	newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);

	if (newConnection == 0) {
		std::cout << "Error #2\n";
	}
	else {
		std::cout << "Client Connected!\n";
		//recvFile(&newConnection);
	}



	system("pause");
	return 0;
}


void recvFile(SOCKET* sock) {
	char fileSizeStr[16];
	char fileName[32];

	recv(*sock, fileSizeStr, 16, 0);
	int fileSize = atoi(fileSizeStr);
	char* bytes = new char[fileSize];

	recv(*sock, fileName, 32, 0);

	std::fstream file;
	file.open(fileName, std::ios_base::out | std::ios_base::binary);

	std::cout << "size: " << fileSize << std::endl;
	std::cout << "name: " << fileName << std::endl;

	if (file.is_open()) {

		recv(*sock, bytes, fileSize, 0);
		//std::cout << "data: " << bytes << std::endl;

		file.write(bytes, fileSize);
		std::cout << "OK!\n";	
	}
	else
	{std::cout << "File openning error\n";}
	
	int a = 1;
	delete[] bytes;
	file.close();
}