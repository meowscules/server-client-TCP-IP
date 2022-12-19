#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <fstream>
#include <experimental/filesystem>
#include <string>

#pragma warning(disable: 4996)

void sendFile(SOCKET* sock, const std::string& fileName);

int main(int argc, char* argv[]) {
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		std::cout << "Error" << std::endl;
		exit(1);
	}

	int PORT = atoi(argv[2]);
	
	SOCKADDR_IN addr;
	addr.sin_addr.s_addr = inet_addr(argv[1]);
	addr.sin_port = htons(PORT);
	addr.sin_family = AF_INET;

	SOCKET Connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
		std::cout << "Error: failed connect to server.\n";
		return 1;
	}
	std::cout << "Connected!\n";

	//sendFile(&Connection, argv[4]);

	system("pause");
	return 0;
}

void sendFile(SOCKET* sock, const std::string& fileName)
{
	std::fstream file;
	file.open(fileName, std::fstream::in | std::fstream::binary);

	if (file.is_open()) {
		int fileSize = std::experimental::filesystem::file_size(fileName);

		char* bytes = new char[fileSize];

		file.read(bytes, fileSize);

		std::cout << "size: " << fileSize << std::endl;
		std::cout << "name: " << fileName << std::endl;
		//std::cout << "data: " << bytes << std::endl;

		//int packet = fileSize / 5;

		send(*sock, std::to_string(fileSize).c_str(), 16, 0);
		send(*sock, fileName.c_str(), 32, 0);
		
		send(*sock, bytes, fileSize, 0);

		delete[] bytes;
	}
	else
		std::cout << "File openning error \n";

	file.close();
}
