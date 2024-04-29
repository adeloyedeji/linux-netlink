#include <linux/module.h>
#include <linux/moduleparam.h>

#define IPC_KERN_SERVER_VERSION "1.0.0"

static int __init ipc_kern_server_init(void)
{
    printk(KERN_INFO "Hello World.\nIPC Kernel server successfuly loaded.\n");
    
    return 0;
}

static void __exit ipc_kern_server_exit(void)
{
    printk(KERN_INFO "IPC Kernel Server says Goodbye.\n");
}

MODULE_AUTHOR("Adedeji Adeloye <adeloyedeji@gmail.com>");
MODULE_VERSION(IPC_KERN_SERVER_VERSION);
MODULE_DESCRIPTION("Linux IPC Netlink Tutorials");
MODULE_LICENSE("GPL");

module_init(ipc_kern_server_init);
module_exit(ipc_kern_server_exit);