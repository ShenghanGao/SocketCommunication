#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include <sys/socket.h>

#define BUFFER_SIZE 1024

void send_file(int connected_socket, char *filename) {
        char buffer[BUFFER_SIZE];
        memset(buffer, '0', sizeof(buffer));

        FILE *fp;
        fp = fopen(filename, "r");
        if (fp == NULL) {
        	printf("Could not open to read!\n");
	}
        else {
        	int file_block_length = 0;
                printf("Entering the read while block!\n");
                while ((file_block_length = fread(buffer, sizeof(char), BUFFER_SIZE, fp)) > 0) {
                	printf("file_block_length = %d\n", file_block_length);

                        if (send(connected_socket, buffer, file_block_length, 0) < 0) {
                        	printf("Sending file failed!\n");
                                break;
                        }

                        memset(buffer, '0', sizeof(buffer));
		}
			fclose(fp);
			printf("File transmitted!\n");
	}
}

