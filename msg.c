#include <string.h>

#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/socket.h>

#include "msg.h"

int msg_sendfd(struct msg *m, int fd) {
	char buf[CMSG_SPACE(sizeof fd)];
	memset(buf, '\0', sizeof buf);

	struct iovec io = {
		.iov_base = "",
		.iov_len = 1
	};

	struct msghdr msg = {
		.msg_iov = &io,
		.msg_iovlen = 1,
		.msg_control = buf,
		.msg_controllen = sizeof buf
	};

	struct cmsghdr* cmsg = CMSG_FIRSTHDR(&msg);
	cmsg->cmsg_level = SOL_SOCKET;
	cmsg->cmsg_type = SCM_RIGHTS;
	cmsg->cmsg_len = CMSG_LEN(sizeof fd);

	memmove(CMSG_DATA(cmsg), &fd, sizeof fd);

	msg.msg_controllen = cmsg->cmsg_len;

	return sendmsg(m->pair[0], &msg, 0);
}

int msg_readfd(struct msg *m) {
	char mbuf[1]; /* Message buffer */
	char cbuf[128]; /* Control buffer */

	struct iovec io = {
		.iov_base = mbuf,
		.iov_len = sizeof mbuf
	};

	struct msghdr msg = {
		.msg_iov = &io,
		.msg_iovlen = sizeof mbuf,
		.msg_control = cbuf,
		.msg_controllen = sizeof cbuf
	};

	if (recvmsg(m->pair[1], &msg, 0) < 0) {
		return -1;
	}

	struct cmsghdr *cmsg = CMSG_FIRSTHDR(&msg);
	int fd = 0;
	memmove(&fd, CMSG_DATA(cmsg), sizeof fd);
	return fd;
}

int msg_init(struct msg *m) {
	return socketpair(PF_LOCAL, SOCK_STREAM, 0, m->pair);
}

void msg_destroy(struct msg *m) {
	close(m->pair[0]);
	close(m->pair[0]);
}
