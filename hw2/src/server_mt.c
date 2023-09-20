/*******************************************************************************
* Simple FIFO Order Server Implementation
*
* Description:
*     A server implementation designed to process client requests in First In,
*     First Out (FIFO) order. The server binds to the specified port number
*     provided as a parameter upon launch.
*
* Usage:
*     <build directory>/server <port_number>
*
* Parameters:
*     port_number - The port number to bind the server to.
*
* Author:
*     Renato Mancuso
*
* Affiliation:
*     Boston University
*
* Creation Date:
*     September 10, 2023
*
* Notes:
*     Ensure to have proper permissions and available port before running the
*     server. The server relies on a FIFO mechanism to handle requests, thus
*     guaranteeing the order of processing. For debugging or more details, refer
*     to the accompanying documentation and logs.
*
*******************************************************************************/
#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Include struct definitions and other libraries that need to be
 * included by both client and server */
#include "common.h"

#define BACKLOG_COUNT 100
#define USAGE_STRING				\
	"Missing parameter. Exiting.\n"		\
	"Usage: %s <port_number>\n"

/* Main function to handle connection with the client. This function
 * takes in input conn_socket and returns only when the connection
 * with the client is interrupted. */
static void handle_connection(int conn_socket)
{
	int worker_main(void *arg){
		(void)arg;
		struct timespec curtime;
		clock_gettime(CLOCK_MONOTONIC, &curtime);
		double current=curtime.tv_sec+((double)curtime.tv_nsec/1e9);
		printf("[#WORKER#] %.6f Worker Thread Alive!",current);
		while(1){
			get_elapsed_busywait(1,0);
			clock_gettime(CLOCK_MONOTONIC, &curtime);
			current=curtime.tv_sec+((double)curtime.tv_nsec/1e9);
			printf("[#WORKER#] %.6f Still Alive!\n",current);
			sleep(1);
		}
		return 0;
	}
	//declare variables for reading from the socket
	int client_size;
	client_size=sizeof(struct request);
	int data;
	//declare time variables for server output
	struct timespec reciept, completion;
	struct request clientreq;
	/*open infinite while loop as long as socket connection is alive*/
	
	int res=clone(&worker_main,malloc(4096)+4096, CLONE_THREAD | CLONE_VM | CLONE_SIGHAND, (void *) &data);
	printf("%d",res);
	while(1){
		//get data from connection, and start a timer noted by the time of
		//request reciept
		data=read(conn_socket,&clientreq ,client_size);
		clock_gettime(CLOCK_MONOTONIC, &reciept);
		//read puts the number of bytes read into data. if data==0 the
		//client's requests are empty. if data==-1 an error occurred from
		//the connection no longer being live. if so, exit the loop.
		if((data)<=0){
	    
			break;
 	    
		}
		//if data was recieved, start busywait using request
		get_elapsed_busywait(clientreq.req_len.tv_sec,clientreq.req_len.tv_nsec);

		clock_gettime(CLOCK_MONOTONIC, &completion);
		//stop timer noted by the time of request completion.
		struct response clientres;
		//create and send server response
		clientres.res_id=clientreq.req_id;
		write(conn_socket,(struct response *) &clientres, sizeof(struct response));
		//create, calculate, and print server output
		int id;
		double sentsec,lensec,recsec,compsec;
		id=clientreq.req_id;
		sentsec=clientreq.timestamp.tv_sec+clientreq.timestamp.tv_nsec*1e-9;
		lensec=clientreq.req_len.tv_sec+clientreq.req_len.tv_nsec*1e-9;
		recsec=reciept.tv_sec+reciept.tv_nsec*1e-9;
		compsec=completion.tv_sec+completion.tv_nsec*1e-9;
		printf("R%d:%.6f,%.6f,%.6f,%.6f\n",id,sentsec,lensec,recsec,compsec);
	}

	return;
}

/* Template implementation of the main function for the FIFO
 * server. The server must accept in input a command line parameter
 * with the <port number> to bind the server to. */
int main (int argc, char ** argv) {
	int sockfd, retval, accepted, optval;
	in_port_t socket_port;
	struct sockaddr_in addr, client;
	struct in_addr any_address;
	socklen_t client_len;

	/* Get port to bind our socket to */
	if (argc > 1) {
		socket_port = strtol(argv[1], NULL, 10);
		printf("INFO: setting server port as: %d\n", socket_port);
	} else {
		ERROR_INFO();
		fprintf(stderr, USAGE_STRING, argv[0]);
		return EXIT_FAILURE;
	}

	/* Now onward to create the right type of socket */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0) {
		ERROR_INFO();
		perror("Unable to create socket");
		return EXIT_FAILURE;
	}

	/* Before moving forward, set socket to reuse address */
	optval = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (void *)&optval, sizeof(optval));

	/* Convert INADDR_ANY into network byte order */
	any_address.s_addr = htonl(INADDR_ANY);

	/* Time to bind the socket to the right port  */
	addr.sin_family = AF_INET;
	addr.sin_port = htons(socket_port);
	addr.sin_addr = any_address;
	
	/* Attempt to bind the socket with the given parameters */
	retval = bind(sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));

	if (retval < 0) {
		ERROR_INFO();
		perror("Unable to bind socket");
		return EXIT_FAILURE;
	}

	/* Let us now proceed to set the server to listen on the selected port */
	retval = listen(sockfd, BACKLOG_COUNT);

	if (retval < 0) {
		ERROR_INFO();
		perror("Unable to listen on socket");
		return EXIT_FAILURE;
	}

	/* Ready to accept connections! */
	printf("INFO: Waiting for incoming connection...\n");
	client_len = sizeof(struct sockaddr_in);
	accepted = accept(sockfd, (struct sockaddr *)&client, &client_len);

	if (accepted == -1) {
		ERROR_INFO();
		perror("Unable to accept connections");
		return EXIT_FAILURE;
	}

	/* Ready to handle the new connection with the client. */

	handle_connection(accepted);

	close(sockfd);
	return EXIT_SUCCESS;

}
