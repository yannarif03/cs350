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
#include <mcheck.h>

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
	"Usage: %s -q <queue size> "		\
	"-w <workers: 1> "			\
	"-p <policy: FIFO> "			\
	"<port_number>\n"

#define sync_printf(...)			\
  do {						\
    sem_wait(printf_mutex);			\
    printf(__VA_ARGS__);			\
    sem_post(printf_mutex);			\
  } while(0)

#ifdef DEBUG
#define DEBUG_PRINTF(format,...) printf("DEBUG: " format,##__VA_ARGS__);
#else
#define DEBUG_PRINTF(format,...)
#endif
/* 4KB of stack for the worker thread */
#define STACK_SIZE (4096)

/* START - Variables needed to protect the shared queue. DO NOT TOUCH */
sem_t * queue_mutex;
sem_t * queue_notify;
/* END - Variables needed to protect the shared queue. DO NOT TOUCH */

sem_t * ret_mutex;
sem_t * img_mutex;
sem_t * arr_mutex;
sem_t * printf_mutex;

/* Max number of requests that can be queued */
#define QUEUE_MAX 1500
int QUEUE_SIZE;
struct image ** img_arr=NULL;
sem_t ** db_mutex=NULL;
int * counters=NULL;
volatile int num_imgs = 0;
struct connection_params{
	long queue_size;
	int thread_num;
	int policy;
};

struct meta_req {
	struct request req;
	struct timespec reciept;
};
struct queue {
	int head;
	int tail;
	struct meta_req items[QUEUE_MAX];
	int size;
};

/* Add a new request <request> to the shared queue <the_queue> */
int add_to_queue(struct meta_req to_add, struct queue * the_queue)
{
	int retval = 0;
	/* QUEUE PROTECTION INTRO START --- DO NOT TOUCH */
	sem_wait(queue_mutex);
	/* QUEUE PROTECTION INTRO END --- DO NOT TOUCH */

	/* WRITE YOUR CODE HERE! */
	/* MAKE SURE NOT TO RETURN WITHOUT GOING THROUGH THE OUTRO CODE! */
	int next_write;
	next_write=(the_queue->tail+1)%QUEUE_SIZE;
	if(next_write==the_queue->head){
		retval=-1;
	}else{

		if(the_queue->size==0){
			the_queue->head=the_queue->tail=0;
			the_queue->items[0]=to_add;
			the_queue->size+=1;
			retval=0;
		}else{
			the_queue->items[next_write]=to_add;
			the_queue->tail=(the_queue->tail+1)%QUEUE_SIZE;
			retval=0;
			the_queue->size+=1;

		}
		sem_post(queue_notify);
	}
	/* QUEUE PROTECTION OUTRO START --- DO NOT TOUCH */
	sem_post(queue_mutex);

	/* QUEUE PROTECTION OUTRO END --- DO NOT TOUCH */
	return retval;
}

/* struct meta_req get_from_queue_sjn(struct queue * the_queue) */
/* { */
/* 	struct meta_req retval; */
/* 	/\* QUEUE PROTECTION INTRO START --- DO NOT TOUCH *\/ */
/* 	sem_wait(queue_notify); */
/* 	sem_wait(queue_mutex); */
/* 	/\* QUEUE PROTECTION INTRO END --- DO NOT TOUCH *\/ */

/* 	/\* WRITE YOUR CODE HERE! *\/ */
/* 	/\* MAKE SURE NOT TO RETURN WITHOUT GOING THROUGH THE OUTRO CODE! *\/ */

/* 	if(the_queue->size==0){ */
/* 		printf("ERROR: TRIED TO GET FROM EMPTY QUEUE\n"); */

/* 	}else{ */
/* 		int sjn_ind=the_queue->head; */
/* 		double sjn_length=TSPEC_TO_DOUBLE(the_queue->items[the_queue->head].req.req_len);		 */
/* 		for(int i=0;i<the_queue->size;i++){ */

/* 			int index=(the_queue->head+i)%QUEUE_SIZE; */
/* 			double cur_length=TSPEC_TO_DOUBLE(the_queue->items[index].req.req_len); */
/* 			if(cur_length<sjn_length){ */
/* 				sjn_length=cur_length; */
/* 				sjn_ind=index; */
/* 			} */

/* 		} */
/* 		retval=the_queue->items[sjn_ind]; */

/* 		for(int i=sjn_ind;i!=the_queue->head;i=(i-1+QUEUE_SIZE)%QUEUE_SIZE){ */
/* 			the_queue->items[i]=the_queue->items[(i-1+QUEUE_SIZE)%QUEUE_SIZE]; */

/* 			//the_queue->items[(i-1+QUEUE_SIZE)%QUEUE_SIZE]=; */
/* 		} */
/* 		the_queue->head=(the_queue->head+1)%QUEUE_SIZE; */
/* 		the_queue->size-=1; */
/* 		if(the_queue->size==0){ */
/* 			the_queue->head=the_queue->tail=-1; */
/* 		} */
/* 		if(TSPEC_TO_DOUBLE(retval.req.req_len)==0){ */
/* 			printf("##ERROR## head: %d tail: %d size: %d ind: %d\n", the_queue->head, the_queue->tail, the_queue->size, sjn_ind); */
/* 		} */
/* 	} */
/* 	/\* QUEUE PROTECTION OUTRO START --- DO NOT TOUCH *\/ */
/* 	sem_post(queue_mutex); */
/* 	/\* QUEUE PROTECTION OUTRO END --- DO NOT TOUCH *\/ */
/* 	return retval; */
/* } */
/* Add a new request <request> to the shared queue <the_queue> */
struct meta_req get_from_queue_fifo(struct queue * the_queue)
{
	struct meta_req retval;
	/* QUEUE PROTECTION INTRO START --- DO NOT TOUCH */
	sem_wait(queue_notify);
	sem_wait(queue_mutex);
	/* QUEUE PROTECTION INTRO END --- DO NOT TOUCH */

	/* WRITE YOUR CODE HERE! */
	/* MAKE SURE NOT TO RETURN WITHOUT GOING THROUGH THE OUTRO CODE! */

	if(the_queue->size==0){
		DEBUG_PRINTF("ERROR: TRIED TO GET FROM EMPTY QUEUE\n");

	}else{
		retval=the_queue->items[the_queue->head];
		the_queue->head=(the_queue->head+1)%QUEUE_SIZE;
		the_queue->size-=1;
		if(the_queue->size==0){
			the_queue->head=the_queue->tail=-1;
		}
	}
	/* QUEUE PROTECTION OUTRO START --- DO NOT TOUCH */
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
	sync_printf("Q:[");
	int start=the_queue->head;
	int id;
	int j=0;

	for (i=start;(j<end);i++){
		i=i%QUEUE_SIZE;
		j++;
		id=the_queue->items[i].req.req_id;
		sync_printf("R%d",id);
		if(j!=end){
			sync_printf(",");
		}
	}
	sync_printf("]\n");

	/* QUEUE PROTECTION OUTRO START --- DO NOT TOUCH */
	sem_post(queue_mutex);
	/* QUEUE PROTECTION OUTRO END --- DO NOT TOUCH */
}

struct clargs{
	struct queue * q;
        int thread_id;
	int socket;
        int worker_done;
	int policy;
};
struct timespec reciept;
/* Main logic of the worker thread */
/* IMPLEMENT HERE THE MAIN FUNCTION OF THE WORKER */
int worker_main(void *args){

	struct clargs* targs=(struct clargs*)args;
	struct queue *queue=targs->q;
	int conn_socket=targs->socket;
	struct meta_req curreq;
	struct timespec start,completion;
	struct response clientres;
	while(!targs->worker_done | (queue->size!=0)){
		int my_arr;
		//sync_printf("starting get\n");

		if(targs->policy==0){

			curreq=get_from_queue_fifo(queue);
			my_arr=counters[curreq.req.img_id*2];
			counters[curreq.req.img_id*2]++;
			sem_post(queue_mutex);
		}
		//sync_printf("finished get\n");
		/*else if(targs->policy==1){
			curreq=get_from_queue_sjn(queue);
			}*/

		/* if(TSPEC_TO_DOUBLE(curreq.timestamp)==0){ */
		/*   continue; */
		/* } */
		if (targs->worker_done) {
			break;
		}
		clock_gettime(CLOCK_MONOTONIC,&start);
		/*IMPLEMENT IMAGE PROCESSING HERE*/
		enum img_opcode opcode=curreq.req.img_op;
		struct image * postop;
		//sync_printf("starting sem spin\n");
		sem_wait(db_mutex[curreq.req.img_id]);
		while (my_arr!=counters[(curreq.req.img_id*2)+1]){
			sem_post(db_mutex[curreq.req.img_id]);
			sem_wait(db_mutex[curreq.req.img_id]);
		}

		DEBUG_PRINTF("T%d Op on %ld\n", targs->thread_id, curreq.req.img_id);

		//do the operation
		//otherwise signal the mutex and wait again.
		//sync_printf("starting op on IMG %ld\n", curreq.req.img_id);
		sem_wait(img_mutex);
		struct image * cur_image = img_arr[curreq.req.img_id];
		sem_post(img_mutex);
		//sync_printf("trying cur_image->width thread %d\n",targs->thread_id);
		switch(opcode){
		case IMG_ROT90CLKW:
			//printf("ROT CALLED\n");
			postop=rotate90Clockwise(cur_image,NULL);
			break;
		case IMG_BLUR:
			//printf("BLUR CALLED\n");
			postop=blurImage(cur_image);
			break;
		case IMG_SHARPEN:
			//printf("SHARPEN CALLED\n");
			postop=sharpenImage(cur_image);
			break;
		case IMG_VERTEDGES:
			//printf("VERT CALLED\n");
			postop=detectVerticalEdges(cur_image);
			break;
		case IMG_HORIZEDGES:
			//printf("HORIZ CALLED \n");
			postop=detectHorizontalEdges(cur_image);
			break;
		case IMG_RETRIEVE:
			//printf("RETRIEVE CALLED\n");
			break;
		default:
			printf("ERROR: OPCODE NOT DETECTED\n");
		}
		//sync_printf("replacing old img\n");
		//prep the response
		uint64_t imgcode=curreq.req.img_id;
		// **CRIT SEC** ADDING OR MODIFYING IMG DB
		sem_wait(img_mutex);
		if(opcode!=IMG_RETRIEVE && !curreq.req.overwrite){
			//sync_printf("before malloc\n");
			img_arr=realloc(img_arr,(num_imgs+1)*sizeof(struct image*));
			db_mutex=realloc(db_mutex,(num_imgs+1)*sizeof(sem_t *));
			counters=realloc(counters,(num_imgs+1)*2*sizeof(int));
			counters[num_imgs*2]=counters[num_imgs*2+1]=0;
			db_mutex[num_imgs]=(sem_t*)malloc(sizeof(sem_t));
			if (sem_init(db_mutex[num_imgs],0,1)){
				ERROR_INFO();
				perror("Unable to init db_mutex");
			}
			img_arr[num_imgs]=postop;
			imgcode=num_imgs;
			num_imgs++;
			//sync_printf("after malloc \n");
		}else if(opcode!=IMG_RETRIEVE){
			//
			deleteImage(cur_image);
			img_arr[curreq.req.img_id]=postop;
		}
		//inc cur_op
		counters[(curreq.req.img_id*2)+1]+=1;
		clock_gettime(CLOCK_MONOTONIC,&completion);
		clientres.req_id=curreq.req.req_id;
		clientres.ack=0;
		clientres.img_id=imgcode;
		//add image response stuff, id and whatnot
		//sync_printf("sending response for IMG %ld\n", curreq.req.img_id);
		sem_wait(ret_mutex);
		if(opcode==IMG_RETRIEVE){
			send(conn_socket,(struct response *) &clientres,sizeof(struct response),0);
			sendImage(cur_image,conn_socket);
		}else{
			send(conn_socket,(struct response *) &clientres,sizeof(struct response),0);
		}
		sem_post(ret_mutex);

		sem_post(img_mutex);
		sem_post(db_mutex[curreq.req.img_id]);

		//change the post processing status print
		int id=curreq.req.req_id;
		double sent_time=TSPEC_TO_DOUBLE(curreq.req.req_timestamp);
		const char* op=OPCODE_TO_STRING(opcode);
		uint8_t ow=curreq.req.overwrite;
		uint64_t clim=curreq.req.img_id;

		double rectime=TSPEC_TO_DOUBLE(curreq.reciept);
		double starttime=TSPEC_TO_DOUBLE(start);
		double comptime=(TSPEC_TO_DOUBLE(completion));
		sync_printf("T%d R%d:%.6f,%s,%d,%ld,%ld,%.6f,%.6f,%.6f\n",targs->thread_id,id,sent_time,op,ow,clim,imgcode,rectime,starttime,comptime);
		dump_queue_status(queue);
		DEBUG_PRINTF("EOW WORKER\n");
	}
	return 0;
}
/* Main function to handle connection with the client. This function
 * takes in input conn_socket and returns only when the connection
 * with the client is interrupted. */

void handle_connection(int conn_socket, struct connection_params conn_params)
{
	struct queue * the_queue;

	/* The connection with the client is alive here. Let's
	 * initialize the shared queue. */
	the_queue=(struct queue*)malloc(sizeof(struct queue));
	/* IMPLEMENT HERE ANY QUEUE INITIALIZATION LOGIC */
	the_queue->head=-1;
	the_queue->tail=-1;
	the_queue->size=0;
	/* Queue ready to go here. Let's start the worker thread. */
	void* stacks[conn_params.thread_num];
	struct clargs* params[conn_params.thread_num];
	int i;
	int worker_done=0;
	for(i=0;i<conn_params.thread_num;i++){
		params[i]=malloc(sizeof(struct clargs));
		params[i]->worker_done=worker_done;
		params[i]->q=the_queue;
		params[i]->socket=conn_socket;
		params[i]->thread_id=i;
		params[i]->policy=conn_params.policy;

		stacks[i]=malloc(STACK_SIZE);

		int res=clone(worker_main,stacks[i]+STACK_SIZE,(CLONE_THREAD | CLONE_VM | CLONE_SIGHAND),(void *)params[i]);
		if(res<0){
		  ERROR_INFO();
		  perror("Unable to start thread.");
		  free(the_queue);
		  return;
		}
	}
	/* IMPLEMENT HERE THE LOGIC TO START THE WORKER THREAD. */

	/* We are ready to proceed with the rest of the request
	 * handling logic. */

	/* REUSE LOGIC FROM HW1 TO HANDLE THE PACKETS */
	int client_size;
	client_size=sizeof(struct request);
	int data;

	struct meta_req clientreq;
	while(1){
		data=recv(conn_socket, &clientreq.req, client_size,0);
		if(data<=0){
			break;
		}
		clock_gettime(CLOCK_MONOTONIC,&clientreq.reciept);
		//check if img_op=IMG_REGISTER. if so, handle the op right away, reading the img payload, replying to client and bypassing the queue.
		if(clientreq.req.img_op==1){
			//REGISTER
			// **CRIT SEC** ADDING TO IMG DB
			sem_wait(img_mutex);
			DEBUG_PRINTF("before malloc\n");
			struct timespec start, completion;
			clock_gettime(CLOCK_MONOTONIC,&start);
			img_arr=realloc(img_arr,(num_imgs+1)*sizeof(struct image*));
			db_mutex=realloc(db_mutex,(num_imgs+1)*sizeof(sem_t*));
			counters=realloc(counters,(num_imgs+1)*2*sizeof(int));
			counters[num_imgs*2]=0;
			counters[(num_imgs*2)+1]=0;
			db_mutex[num_imgs]=(sem_t*)malloc(sizeof(sem_t));
			int retval=sem_init(db_mutex[num_imgs],0,1);

			if (retval){
				ERROR_INFO();
				perror("Unable to init db_mutex");
			}
			DEBUG_PRINTF("after malloc \n");
			img_arr[num_imgs]=recvImage(conn_socket);
			sem_post(img_mutex);

			struct response regresp={clientreq.req.req_id,num_imgs,0};
			send(conn_socket, &regresp, sizeof(struct response),0);
			clock_gettime(CLOCK_MONOTONIC,&completion);
			double sent_time=TSPEC_TO_DOUBLE(clientreq.req.req_timestamp);
			const char* op=OPCODE_TO_STRING(clientreq.req.img_op);
			uint8_t ow=0;
			uint64_t clim=0;

			double rectime=TSPEC_TO_DOUBLE(clientreq.reciept);
			double starttime=TSPEC_TO_DOUBLE(start);
			double comptime=(TSPEC_TO_DOUBLE(completion));
			sync_printf("T0 R%ld:%.6f,%s,%d,%ld,%d,%.6f,%.6f,%.6f\n",clientreq.req.req_id,sent_time,op,ow,clim,num_imgs,rectime,starttime,comptime);
			num_imgs++;
			continue;
		}
		
		int status=add_to_queue(clientreq,the_queue);
		struct response rej_res;
		if(status==-1){
			rej_res.req_id=clientreq.req.req_id;
			rej_res.img_id=clientreq.req.img_id;
			rej_res.ack=1;
			send(conn_socket, &rej_res,sizeof(struct response),0);
			sync_printf("X%lu:%.6f,%.6f\n",clientreq.req.req_id, \
			       TSPEC_TO_DOUBLE(clientreq.req.req_timestamp), \
			       TSPEC_TO_DOUBLE(clientreq.reciept));
		}
		
	}

	/* Ask the worker thead to terminate */
	printf("INFO: Asserting termination flag for worker thread...\n");
	
	for(i=0;i<conn_params.thread_num;i++){
		params[i]->worker_done = 1;
		sem_post(queue_notify);
		waitpid(-1, NULL, 0);
		printf("INFO: Worker thread with id %d exited.\n",i);
	}


	/* Just in case the thread is stuck on the notify semaphore,
	 * wake it up */


	/* Wait for orderly termination of the worker thread */

	free(the_queue);
	for(i=0;i<conn_params.thread_num;i++){
		free(stacks[i]);
		free(params[i]);
	}

	shutdown(conn_socket, SHUT_RDWR);
	close(conn_socket);
	printf("INFO: Client disconnected.\n");


	
	
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
	struct connection_params conn_params;
	int opt;
	while((opt=getopt(argc,argv,"q:w:p:")) != -1){
		switch(opt){
		case 'q':
			conn_params.queue_size=strtol(optarg,NULL,0);
			break;
		case 'w':
			conn_params.thread_num = strtol(optarg,NULL,0);
			break;
		case 'p':
			if(strcmp(optarg,"FIFO")==0){
				conn_params.policy=0;
				printf("FIFO POLICY LIVE\n");
			/* }else if(strcmp(optarg,"SJN")==0){ */
			/* 	printf("SJN POLICY LIVE\n"); */
			/* 	conn_params.policy=1; */
			}else{
				printf("invalid policy entered. Please use SJN or FIFO\n");
				return EXIT_FAILURE;
			}
			break;
		default:
			printf("YOU USED IT WRONG. LEAVE.");
			return EXIT_FAILURE;
		}
	}
	QUEUE_SIZE=conn_params.queue_size;
	/* if(getopt(argc,argv,"q:w:")!='q'){ */
	/*   perror("Please Specify Queue Size using -q param\n"); */
	/*   return EXIT_FAILURE; */
	/* } */
	/* printf("ind=%d\n",optind); */
	/* conn_params.queue_size=strtol(optarg,NULL,0); */
	/* QUEUE_SIZE=conn_params.queue_size; */
	/* printf("q=%ld\n",strtol(optarg,NULL,0)); */
	/* if(getopt(argc,argv,"q:w:")!='w'){ */
	/* 	perror("Please Specify number of worker threads using -w param\n"); */
	/* 	return EXIT_FAILURE; */
	/* } */
	/* printf("ind=%d\n",optind); */
	/* conn_params.thread_num = strtol(optarg,NULL,0); */
	/* printf("w=%ld\n",strtol(optarg,NULL,0));	 */
 	/* Get port to bind our socket to */
	/* Get port to bind our socket to */
	if (optind<argc) {
		socket_port = strtol(argv[optind], NULL, 10);
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
	printf_mutex = (sem_t *)malloc(sizeof(sem_t));
	img_mutex = (sem_t *)malloc(sizeof(sem_t));
	ret_mutex=(sem_t*)malloc(sizeof(sem_t));
	retval = sem_init(queue_mutex, 0, 1);
	if(sem_init(printf_mutex,0,1)<0){
	  ERROR_INFO();
	  perror("Unable to init printf mutex");
	  return EXIT_FAILURE;
	}
	if (retval < 0) {
		ERROR_INFO();
		perror("Unable to initialize queue mutex");
		return EXIT_FAILURE;
	}
	if (sem_init(queue_notify,0,0)) {
		ERROR_INFO();
		perror("Unable to initialize queue notify");
		return EXIT_FAILURE;
	}
	if (sem_init(ret_mutex,0,1)){
		ERROR_INFO();
		perror("Unable to init ret_mutex");
		return EXIT_FAILURE;
	}
	if (sem_init(img_mutex,0,1)){
		ERROR_INFO();
		perror("Unable to init ret_mutex");
		return EXIT_FAILURE;
	}

	/* DONE - Initialize queue protection variables. DO NOT TOUCH */

	/* Ready to handle the new connection with the client. */
	handle_connection(accepted,conn_params);
	DEBUG_PRINTF("Starting frees\n")
	free(queue_mutex);
	free(queue_notify);
	free(printf_mutex);
	free(ret_mutex);
	free(img_mutex);
	for(int i=0;i<num_imgs;i++){
		deleteImage(img_arr[i]);
		free(db_mutex[i]);
	}
	free(img_arr);
	free(db_mutex);
	DEBUG_PRINTF("DONE W HANDCONN\n")
	DEBUG_PRINTF("AAAAAAA\n")
	close(sockfd);


	return EXIT_SUCCESS;

}
