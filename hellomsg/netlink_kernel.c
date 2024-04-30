#include <linux/module.h>
#include <net/sock.h>
#include <linux/netlink.h>
#include <linux/skbuff.h>

#define MY_NETLINK 30

struct sock *nl_sk = NULL;

static void netlink_recv_msg(struct sk_buff *skb)
{
    struct nlmsghdr *nlhead;
    struct sk_buff *skb_out;
    int pid, res, msg_sz;
    char *msg = "Hello msg from kernel";

    printk(KERN_INFO "Entering: %s\n", __FUNCTION__);

    msg_sz = strlen(msg);

    nlhead = (struct nlmsghdr*)skb->data;

    //TODO Add some message validation ...

    printk(KERN_INFO "netlink received: %s\n", (char*)nlmsg_data(nlhead));

    pid = nlhead->nlmsg_pid;

    skb_out = nlmsg_new(msg_sz, 0);

    if (!skb_out)
    {
        printk(KERN_ERR "Failed to allocate new skb for message.\n");
        return;
    }

    nlhead = nlmsg_put(skb_out, 0, 0, NLMSG_DONE, msg_sz, 0);

    NETLINK_CB(skb_out).dst_group = 0;

    strncpy(nlmsg_data(nlhead), msg, msg_sz);

    res = nlmsg_unicast(nl_sk, skb_out, pid);

    if (res < 0)
        printk(KERN_ERR "Error while sending response to user.\n");
}

static int
__init netlink_lkm_init(void)
{
    struct netlink_kernel_cfg cfg = {
        .input = netlink_recv_msg,
    };

    nl_sk = netlink_kernel_create(&init_net, MY_NETLINK, &cfg);
    printk(KERN_INFO "Entering: %s protocol family = %d.\n", __FUNCTION__, MY_NETLINK);

    if (!nl_sk)
    {
        printk(KERN_ALERT "Error creating socket.\n");
        return -10;
    }

    printk(KERN_INFO "Netlink LKM initialization OK!.\n");
    return 0;
}

static void
__exit netlink_lkm_exit(void)
{
    printk(KERN_INFO "Exiting Netlink LKM module.\n");
    netlink_kernel_release(nl_sk);
}

module_init(netlink_lkm_init);
module_exit(netlink_lkm_exit);

MODULE_LICENSE("GPL");