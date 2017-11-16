#ifndef MSG_HDR
#define MSG_HDR

struct msg
{
	int pair[2];
};

/* Send file descriptor for shared memory across process */
int msg_sendfd(struct msg* m, int fd);

/* Read file descriptor for shared memory from socket */
int msg_readfd(struct msg* m);

/* Create message socket pair for communicating shared memory descriptor */
int msg_init(struct msg* m);

/* Destroy message socket pair */
void msg_destroy(struct msg* m);


#endif
