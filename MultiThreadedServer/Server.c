#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include "Transfer.h" 
/*
int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
void *(*start_routine) (void *), void *arg);

Compile and link with -pthread.

The pthread_create() function starts a new thread in the calling
process.  The new thread starts execution by invoking
start_routine(); arg is passed as the sole argument of
start_routine().
*/
#include <pthread.h>

void *socket_handler();
void *connection_handler(void *);

int main(void) {
	pthread_t thread_id;
	pthread_create(&thread_id, NULL, socket_handler, NULL);

	while (1) {
	}

	system("pause");
	return 0;
}

void *socket_handler() {
	int iResult = 0;

	struct sockaddr_in serv_addr;
	memset(&serv_addr, '0', sizeof(serv_addr));

	//Address family (must be AF_INET)
	serv_addr.sin_family = AF_INET;
	//IP port
	serv_addr.sin_port = htons(8000);
	//IP address
	/*struct sockaddr_in {
	short sin_family;
	unsigned short sin_port;
	struct in_addr sin_addr;
	char sin_zero[8];
	};
	*/
	//The in_addr structure represents an IPv4 Internet address.
	/*If an application does not care what local address is assigned,
	specify the constant value INADDR_ANY for an IPv4 local address.
	*/
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	int serv_socket = 0;
	//If no error occurs, socket returns a descriptor referencing the
	//new socket. Otherwise, a value of INVALID_SOCKET is returned, and a specific error code can be retrieved by calling WSAGetLastError.
	serv_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (serv_socket < 0) {
		printf("Could not create server socket!\n");
		exit(1);
	}

	printf("Server socket created!\n");
	/*int bind(
	_In_  SOCKET s,
	_In_  const struct sockaddr *name,
	_In_  int namelen
	);
	*/
	//name [in]
	//A pointer to a sockaddr structure of the local address to assign to the bound socket.
	iResult = bind(serv_socket, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	if (iResult != 0) {
		printf("Could not bind server socket!\n");
		exit(1);
	}

	printf("Server starts to listen!\n");
	//If no error occurs, listen returns zero.
	if (listen(serv_socket, 20)) {
		printf("Server listening failed!\n");
		exit(1);
	}

	struct sockaddr_in client_addr;
	socklen_t length = sizeof(client_addr);

	printf("\nServer starts to accept!\n");

	pthread_t thread_id;

	int connected_socket;
	while (connected_socket = accept(serv_socket, (struct sockaddr*)&client_addr, &length)) {
		if (pthread_create(&thread_id, NULL, connection_handler, (void*) &connected_socket) < 0) {
			printf("Could not create thread!\n");
			continue;
		}
	}
}

void *connection_handler(void *connected_socket) {
	int sock = *(int*)connected_socket;
	send_file(sock, "Data.out");
}

