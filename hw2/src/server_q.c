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
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sched.h>
#include <signal.h>

/* Needed for wait(...) */
#include <sys/types.h>
#include <sys/wait.h>

/* Needed for semaphores */
#include <semaphore.h>

/* Include struct definitions and other libraries that need to be
 * included by both client and server */
#include "common.h"

#define BACKLOG_COUNT 100
#define USAGE_STRING				\
	"Missing parameter. Exiting.\n"		\
	"Usage: %s <port_number>\n"

/* 4KB of stack for the worker thread */
#define STACK_SIZE (4096)

/* START - Variables needed to protect the shared queue. DO NOT TOUCH */
sem_t * queue_mutex;
sem_t * queue_notify;
/* END - Variables needed to protect the shared queue. DO NOT TOUCH */

/* Max number of requests that can be queued */
#define QUEUE_SIZE 500

struct queue {
	int head;
	int tail;
	struct request items[QUEUE_SIZE];
	int size;
};


/* Add a new request <request> to the shared queue <the_queue> */
int add_to_queue(struct request to_add, struct queue * the_queue)
{
	int retval = 0;
	/* QUEUE PROTECTION INTRO START --- DO NOT TOUCH */
	sem_wait(queue_mutex);
	/* QUEUE PROTECTION INTRO END --- DO NOT TOUCH */

	/* WRITE YOUR CODE HERE! */
	/* MAKE SURE NOT TO RETURN WITHOUT GOING THROUGH THE OUTRO CODE! */
	int next_write=the_queue->tail+1;
	if(next_write==QUEUE_SIZE){
		next_write=0;
	}
	if(next_write==the_queue->head){
		retval=-1;
	}else{
	
		the_queue->items[next_write]=to_add;
		the_queue->tail+=1;
		if(the_queue->tail==QUEUE_SIZE){
			the_queue->tail=0;
		}
		retval=0;
		the_queue->size+=1;
	}
	/* QUEUE PROTECTION OUTRO START --- DO NOT TOUCH */
	sem_post(queue_mutex);
	sem_post(queue_notify);
	/* QUEUE PROTECTION OUTRO END --- DO NOT TOUCH */
	return retval;
}

/* Add a new request <request> to the shared queue <the_queue> */
struct request get_from_queue(struct queue * the_queue)
{
	struct request retval;
	/* QUEUE PROTECTION INTRO START --- DO NOT TOUCH */
	sem_wait(queue_notify);
	sem_wait(queue_mutex);
	/* QUEUE PROTECTION INTRO END --- DO NOT TOUCH */

	/* WRITE YOUR CODE HERE! */
	/* MAKE SURE NOT TO RETURN WITHOUT GOING THROUGH THE OUTRO CODE! */
	
	if(the_queue->head==the_queue->tail){
		printf("ERROR: TRIED TO GET FROM EMPTY QUEUE");
	       
	}else{
		retval=the_queue->items[the_queue->head];
		the_queue->head+=1;
		the_queue->size-=1;
		if(the_queue->head==QUEUE_SIZE){
			the_queue->head=0;
		}
	}
	/* QUEUE PROTECTION OUTRO START --- DO NOT TOUCH */
	sem_post(queue_mutex);
	/* QUEUE PROTECTION OUTRO END --- DO NOT TOUCH */
	return retval;
}

/* Implement this method to correctly dump the status of the queue
 * following the format Q:[R<request ID>,R<request ID>,...] */
void dump_queue_status(struct queue * the_queue)
{
	int i;
	/* QUEUE PROTECTION INTRO START --- DO NOT TOUCH */
	sem_wait(queue_mutex);
	/* QUEUE PROTECTION INTRO END --- DO NOT TOUCH */

	/* WRITE YOUR CODE HERE! */
	/* MAKE SURE NOT TO RETURN WITHOUT GOING THROUGH THE OUTRO CODE! */


	int end=the_queue->size;
	printf("Q:[");
	int start=the_queue->head;
	int id;
	int j=0;
	
	for (i=start;(j<end);i++){
		i=i%QUEUE_SIZE;
		j++;
		id=the_queue->items[i].req_id;
		printf("R%d",id);
		if(j!=end){
			printf(",");
		}
	}
	printf("]\n");
	/* QUEUE PROTECTION OUTRO START --- DO NOT TOUCH */
	sem_post(queue_mutex);
	/* QUEUE PROTECTION OUTRO END --- DO NOT TOUCH */
}

struct clargs{
	struct queue * q;
	//struct timespec rec;
	int socket;
};
struct timespec reciept;
/* Main logic of the worker thread */
/* IMPLEMENT HERE THE MAIN FUNCTION OF THE WORKER */
int worker_main(void *args){
	struct clargs* targs=(struct clargs*)args;
	struct queue *queue=targs->q;
	int conn_socket=targs->socket;
	struct request curreq;
	struct timespec start,completion;
	struct response clientres;
	while(1){
		curreq=get_from_queue(queue);
		clock_gettime(CLOCK_MONOTONIC,&start);
		get_elapsed_busywait(curreq.req_len.tv_sec,curreq.req_len.tv_nsec);
		clock_gettime(CLOCK_MONOTONIC,&completion);
		clientres.res_id=curreq.req_id;
		write(conn_socket,(struct response *) &clientres,sizeof(struct response));
		int id=curreq.req_id;
		double sent_time=TSPEC_TO_DOUBLE(curreq.timestamp);
		double sent_len=TSPEC_TO_DOUBLE(curreq.req_len);
		double rectime=TSPEC_TO_DOUBLE(reciept);
		double starttime=TSPEC_TO_DOUBLE(start);
		double comptime=(TSPEC_TO_DOUBLE(completion));
		printf("R%d:%.6f,%.6f,%.6f,%.6f,%.6f\n",id,sent_time,sent_len,rectime,starttime,comptime);
		dump_queue_status(queue);
	}
	return 0;
}
/* Main function to handle connection with the client. This function
 * takes in input conn_socket and returns only when the connection
 * with the client is interrupted. */
void handle_connection(int conn_socket)
{
	
	struct queue * the_queue;
	
	/* The connection with the client is alive here. Let's
	 * initialize the shared queue. */
	the_queue=(struct queue*)malloc(sizeof(struct queue));
	/* IMPLEMENT HERE ANY QUEUE INITIALIZATION LOGIC */
	the_queue->head=0;
	the_queue->tail=0;
	/* Queue ready to go here. Let's start the worker thread. */
	void * babystack=malloc(STACK_SIZE);
	struct clargs reqhand;
	reqhand.q=the_queue;
	reqhand.socket=conn_socket;
	//how to share memory with child process for recieved timestamp?
	clone(&worker_main,babystack+STACK_SIZE,(CLONE_THREAD | CLONE_VM | CLONE_SIGHAND),(void *)&reqhand);
	/* IMPLEMENT HERE THE LOGIC TO START THE WORKER THREAD. */

	/* We are ready to proceed with the rest of the request
	 * handling logic. */

	/* REUSE LOGIC FROM HW1 TO HANDLE THE PACKETS */
	int client_size;
	client_size=sizeof(struct request);
	int data;
	
	struct request clientreq;
	while(1){
		data=read(conn_socket, &clientreq,client_size);
		if(data<=0){
			break;
		}
		clock_gettime(CLOCK_MONOTONIC,&reciept);
		add_to_queue(clientreq,the_queue);
	}
	free(the_queue);
	
	
	/* PERFORM ORDERLY DEALLOCATION AND OUTRO HERE */
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

	/* Initialize queue protection variables. DO NOT TOUCH. */
	queue_mutex = (sem_t *)malloc(sizeof(sem_t));
	queue_notify = (sem_t *)malloc(sizeof(sem_t));
	retval = sem_init(queue_mutex, 0, 1);
	if (retval < 0) {
		ERROR_INFO();
		perror("Unable to initialize queue mutex");
		return EXIT_FAILURE;
	}
	retval = sem_init(queue_notify, 0, 0);
	if (retval < 0) {
		ERROR_INFO();
		perror("Unable to initialize queue notify");
		return EXIT_FAILURE;
	}
	/* DONE - Initialize queue protection variables. DO NOT TOUCH */

	/* Ready to handle the new connection with the client. */
	handle_connection(accepted);

	free(queue_mutex);
	free(queue_notify);

	close(sockfd);
	return EXIT_SUCCESS;

}
