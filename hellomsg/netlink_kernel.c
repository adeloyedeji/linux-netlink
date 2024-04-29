#include <linux/module.h>
#include <net/sock.h>
#include <linux/netlink.h>
#include <linux/skbuff.h>

#define MY_NETLINK 30

struct sock *nl_sk = NULL;

static void netlink_recv_msg(struct sk_buff *skb)
{
    
}