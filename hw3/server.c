/*
	server.c
	- server program for FTP Service

	Author @ Juan Lee (juanlee@kaist.ac.kr)
*/

#include "packet.h"

#define SERVER_PATH "Server_files/"

void* handleClient(void* argument);

int main(int argc, char* argv[]){
	int listenfd, *connfd;
	socklen_t clientlen;
	struct sockaddr_storage clientaddr;
	pthread_t tid;

	// open listen fd
	if((listenfd = open_listenfd(argv[1])) < 0){
		fprintf(stderr, "Listen Failed\n");
		return -1;
	}

	printf("Server Start\n");

	// main loop of server
	clientlen = sizeof(struct sockaddr_storage);
	while(1){
		connfd = (int*)calloc(sizeof(int), 1);
		if((*connfd = accept(listenfd, (SA*)&clientaddr, &clientlen)) < 0){
			fprintf(stderr, "Accept Failed\n");
			continue;
		}
		printf("Accept %d\n", *connfd);

		pthread_create(&tid, NULL, handleClient, connfd);
	}

	return 0;
}

void* handleClient(void* argument){
	// variables
	char c;
	char filename[MAX_BUFFER_LENGTH] = {0};
	char absname[MAX_BUFFER_LENGTH] = {0};
	char buf[MAX_BUFFER_LENGTH] = {0};
	long size = 0, nRead = 0, total = 0;
	int fd, i, connfd;
	struct stat fstat;

	// detach thread
	connfd = *((int*)argument);
	pthread_detach(pthread_self());
	free(argument);

	while(1){
		if(read(connfd, &c, 1) == 0){
			printf("Connection Loss\n");
			break;
		}

		switch(c){
		case 'U':
			read(connfd, filename, MAX_BUFFER_LENGTH); // filename
			read(connfd, (char*)&size, sizeof(long)); // size
			sprintf(absname, "%s%s", SERVER_PATH, filename);

			if((fd = open(absname, O_CREAT | O_TRUNC | O_RDWR, 0644)) < 0){
				fprintf(stderr, "Open Failed\n");
				break;
			}

			total = 0;
			while(size - total){
				nRead = read(connfd, buf, sizeof(buf));
				write(fd, buf, nRead);

				total += nRead;
			}
			close(fd);

			printf("Upload Finished by %d\n", connfd);
			break;
		case 'D':
			read(connfd, filename, MAX_BUFFER_LENGTH); // filename

			// filesize
			sprintf(absname, "%s%s", SERVER_PATH, filename);
			stat(absname, &fstat);
			write(connfd, (char*)&fstat.st_size, sizeof(long));

			// filedata
			if((fd = open(absname, O_RDONLY)) < 0){
				fprintf(stderr, "Open Failed\n");
				break;
			}

			total = 0;
			while(size - total){
				nRead = read(fd, buf, sizeof(buf));
				write(connfd, buf, nRead);

				total += nRead;
			}
			close(fd);

			printf("Download Finished by %d\n", connfd);
			break;
		default:
			break;
		}

		for(i=0; i<MAX_BUFFER_LENGTH; i++){
			filename[i] = 0;
			absname[i] = 0;
			buf[i] = 0;
		}
	}

	close(connfd);
	return NULL;
}