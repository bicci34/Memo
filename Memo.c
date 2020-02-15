
#include <linux/module.h>
#include <linux/kernel.h> 
#include <linux/fs.h>     
#include <asm/uaccess.h>     

MODULE_LICENSE("GPL");                                     //GLOBAL PUBLIC LICENSE             
MODULE_AUTHOR("Francesco Bicciato");                                    
MODULE_DESCRIPTION("Store and read your message.");      
MODULE_VERSION("0.1");  

/* REGION Global variables*/
#define DEVICE_NAME "Memo"
#define SUCCESS 0
static int memo_major = 0;
int result;
static int Device_Open = 0;	/* Is device open? */



/*REGION Method */

/*Return message stored from user*/
static ssize_t memo_read(struct file *filp, char __user *buf, size_t count, loff_t *pos)
{
	
	printk(KERN_DEBUG "Read Method called , Function not supported\n");
	return -EFAULT;
}

/*Store user messafe*/
static ssize_t memo_write(struct file *filp, const char __user *buf, size_t count, loff_t *pos)
{
	printk(KERN_DEBUG "Write method called Function not supported\n");
	return SUCCESS; /* succeed, to avoid retrial */
}

static int memo_open(struct inode *inode, struct file *filp)
{
	printk(KERN_DEBUG "Open method called\n");
    if (Device_Open)                //when file is already open return busy
		return -EBUSY;

	Device_Open++;                  //Set device as Busy
    try_module_get(THIS_MODULE);    //Increment the use count
	return SUCCESS; 
}

static int memo_release(struct inode *inode, struct file *filp)
{
	printk(KERN_DEBUG "Release method called\n");
    Device_Open--;              //release the operation on file 
    module_put(THIS_MODULE)     //Decrement the use count
	return SUCCESS; 
}
static struct file_operations memo_fops = { /*attach my operation  */
	.owner = THIS_MODULE,
	.read =  memo_read, 
	.write = memo_write,
	.open = memo_open,
	.release = memo_release
};

/*Initialize the device*/
int memo_init(void)
{
	printk(KERN_DEBUG "\nInitialise memo\n");
	
	result = register_chrdev(memo_major, DEVICE_NAME, & memo_fops);/* Assign major number, name and opration supported by this device */

	if (result < 0){     /*error during registration*/
		return result;
	}
	if (memo_major == 0)  {  

		memo_major = result; /* dynamic major number will be assignment */
		printk(KERN_DEBUG "Major N = %d\n",memo_major);
	}
	return SUCCESS;/*registration sucess and */
}

/*Unregister the device*/
void memo_cleanup(void)
{
	printk(KERN_DEBUG "Destroy memo= %d",memo_major); 
	unregister_chrdev(memo_major, DEVICE_NAME); 
}

module_init(memo_init);
module_exit(memo_cleanup);
