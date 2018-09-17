/*
	packet.h
	- a packet declaration with operating functions
	- update: network functions added

	Author @ Juan Lee (juanlee@kaist.ac.kr)
*/

#ifndef _PACKET_H_
#define _PACKET_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include <pthread.h>

/* Constraints */
#define MAX_BUFFER_LENGTH 1024
#define LISTENQ 1024

/* Typedef Structure */
typedef struct sockaddr SA;

/* Socket helper functions */
int open_listenfd(char *port);
int open_clientfd(char *hostname, char *port);

#endif