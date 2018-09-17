/*
	client.c
	- client program for FTP Service

	Author @ Juan Lee (juanlee@kaist.ac.kr)
*/

#include "packet.h"

#define CLIENT_PATH "Client_files/"

void LIST();
void UPLOAD(int fd, char* file);
void DOWNLOAD(int fd, char* file);

int main(int argc, char* argv[]){
	int clientfd, i;
	char buf[MAX_BUFFER_LENGTH] = {0};

	if((clientfd = open_clientfd(argv[1], argv[2])) < 0){
		fprintf(stderr, "Connect Failed\n");
		return -1;
	}

	printf("Connected to FTP Server with %d\n", clientfd);

	// main loop for client
	printf(">> ");
	while(fgets(buf, MAX_BUFFER_LENGTH-1, stdin) != NULL){
		if(strncmp(buf, "LIST\n", 5) == 0){
			LIST();
		}
		else if(strncmp(buf, "UPLOAD ", 7) == 0){
			UPLOAD(clientfd, buf+7);
		}
		else if(strncmp(buf, "DOWNLOAD ", 9) == 0){
			DOWNLOAD(clientfd, buf+9);
		}
		else{
			if(buf[0] != '\n')
				printf("Wrong Command!\n");
		}

		for(i=0; i<MAX_BUFFER_LENGTH; i++){
			buf[i] = 0;
		}
		printf(">> ");
	}
	close(clientfd);

	return 0;
}

void LIST(){
	char buf[MAX_BUFFER_LENGTH] = {0};
	sprintf(buf, "ls %s", CLIENT_PATH);

	system(buf);
}

void UPLOAD(int fd, char* file){
	char abs[MAX_BUFFER_LENGTH] = {0}, buf[MAX_BUFFER_LENGTH] = {0};
	struct stat fstat;
	int file_fd;
	long nRead, total;

	*(file + strlen(file) - 1) = 0; // remove \n

	sprintf(abs, "%s%s", CLIENT_PATH, file); // make absolute path

	// open file
	if((file_fd = open(abs, O_RDONLY)) < 0){
		fprintf(stderr, "Open Failed\n");
		return;
	}

	// sending request
	write(fd, "U", 1);

	// sending filename
	write(fd, file, MAX_BUFFER_LENGTH);

	// sending size as 8 byte(long)
	stat(abs, &fstat);
	write(fd, (char*)&fstat.st_size, sizeof(long));

	// sending file data
	total = 0;
	while(fstat.st_size - total){
		nRead = read(file_fd, buf, MAX_BUFFER_LENGTH);
		write(fd, buf, nRead);

		total += nRead;
	}
	close(file_fd);

	printf("Upload Finished!\n");
}

void DOWNLOAD(int fd, char* file){
	char abs[MAX_BUFFER_LENGTH] = {0}, buf[MAX_BUFFER_LENGTH] = {0};
	long size = 0, nRead = 0, total = 0;
	int file_fd;

	*(file + strlen(file) - 1) = 0; // remove \n

	sprintf(abs, "%s%s.new", CLIENT_PATH, file); // make absolute path
	
	// open file
	if((file_fd = open(abs, O_CREAT | O_TRUNC | O_RDWR, 0644)) < 0){
		fprintf(stderr, "Open Failed\n");
		return;
	}

	// sending request
	write(fd, "D", 1);

	// sending filename
	write(fd, file, MAX_BUFFER_LENGTH);

	// sending size as 8 byte(long)
	read(fd, (char*)&size, sizeof(long));

	total = 0;
	while(size - total){
		nRead = read(fd, buf, sizeof(buf));
		write(file_fd, buf, nRead);

		total += nRead;
	}

	close(file_fd);

	printf("Download Finished!\n");
}