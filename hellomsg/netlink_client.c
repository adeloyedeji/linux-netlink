#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <asm/types.h>
#include <linux/netlink.h>
#include <linux/socket.h>
#include <errno.h>

#define NETLINK_USER    30
#define MAX_PAYLOAD     1024

struct sockaddr_nl src_addr;
struct sockaddr_nl dest_addr;
struct nlmsghdr *nlh = NULL;
struct nlmsghdr *nlh2 = NULL;
struct msghdr msg;
struct msghdr resp;
struct iovec iov;
struct iovec iov2;
int sock_fd;

int
main(int argc, char* argv[])
{
    sock_fd = socket(PF_NETLINK, SOCK_RAW, NETLINK_USER);
    if (sock_fd < 0)
        return -1;

    memset(&src_addr, 0, sizeof(src_addr));
    src_addr.nl_family = AF_NETLINK;
    src_addr.nl_pid = getpid();

    if (bind(sock_fd, (struct sockaddr*)&src_addr, sizeof(src_addr))) {
        perror("bind() failed\n");
        close(sock_fd);
        return -1;
    }

    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.nl_family = AF_NETLINK;
    dest_addr.nl_pid = 0;
    dest_addr.nl_groups = 0;

    //nlh: contains "Hello" msg
    nlh = (struct nlmsghdr*)calloc(1, NLMSG_SPACE(MAX_PAYLOAD));
    nlh->nlmsg_len = NLMSG_SPACE(MAX_PAYLOAD);
    nlh->nlmsg_pid = getpid();
    nlh->nlmsg_flags = 0;

    //nlh2: contains received msg
    nlh2 = (struct nlmsghdr*)calloc(1, NLMSG_SPACE(MAX_PAYLOAD));
    nlh2->nlmsg_len = NLMSG_SPACE(MAX_PAYLOAD);
    nlh2->nlmsg_pid = getpid();
    nlh2->nlmsg_flags = 0;

    char *u_msg = "Hello message from userspace.";

    strncpy(NLMSG_DATA(nlh), u_msg, strlen(u_msg));

    iov.iov_base = (void*)nlh;
    iov.iov_len = nlh->nlmsg_len;

    msg.msg_name = (void*)&dest_addr;
    msg.msg_namelen = sizeof(dest_addr);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    iov2.iov_base = (void*)nlh2;
    iov2.iov_len = nlh2->nlmsg_len;

    resp.msg_name = (void*)&dest_addr;
    resp.msg_namelen = sizeof(dest_addr);
    resp.msg_iov = &iov2;
    resp.msg_iovlen = 1;

    printf("Sending message to kernel space ... \n");

    int ret = sendmsg(sock_fd, &msg, 0);
    printf("send response code: %d.\n", ret);

    printf("Waiting for message from kernel ... \n");

    recvmsg(sock_fd, &resp, 0);

    printf("Received message payload: %s.\n", (char*)NLMSG_DATA(nlh2));

    char usermsg[MAX_PAYLOAD];
    while(1) {
        printf("Input your message to send to kernel:\t");
        scanf("%s", usermsg);

        strncpy(NLMSG_DATA(nlh), usermsg, MAX_PAYLOAD);

        printf("Sending emssage \" %s \" to kernel.\n", usermsg);

        ret = sendmsg(sock_fd, &msg, 0);
        printf("Send response code: %d.\n", ret);
        printf("Waiting for kernel to respond. \n");

        recvmsg(sock_fd, &resp, 0);
        printf("Received message payload: %s\n", (char*)NLMSG_DATA(nlh2));
    }

    close(sock_fd);

    return 0;
}