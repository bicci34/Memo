
#include <linux/module.h>
#include <linux/kernel.h> 
#include <linux/fs.h>     
#include <linux/uaccess.h>


MODULE_LICENSE("GPL");                                     //GLOBAL PUBLIC LICENSE             
MODULE_AUTHOR("Francesco Bicciato");                                    
MODULE_DESCRIPTION("Store and read your message.");      
MODULE_VERSION("0.1");  

/* REGION Global variables*/
#define DEVICE_NAME "Memo"
#define SUCCESS 0
static int memo_major = 0;
static int Device_Open = 0;	/* Is device open? */

static char *msg[] = "OK va la read";



/*REGION Method */

/*Return message stored from user*/
static ssize_t memo_read(struct file *filp, char __user *buf, size_t count, loff_t *pos)
{
    ssize_t retval;
	printk(KERN_DEBUG "Read Method called , Function not supported\n");

    if (!msg) {                     /*check allocation insuccess*/
	printk(KERN_DEBUG "Msg Null pointer\n");	
            retval = -ENOMEM;           //
            goto out;
    }
    if (copy_to_user(msg, buf, count)) {/*check allocation insuccess*/
		printk(KERN_DEBUG "copy to user fail\n message:%d\n,buffer%s\n",msg,buf);	
            retval = -EFAULT;
            goto out;
    }
    retval = count;

    out:     
        return retval;	
        
	
}

/*Store user messafe*/
static ssize_t memo_write(struct file *filp, const char __user *buf, size_t count, loff_t *pos)
{    
    ssize_t retval;
	printk(KERN_DEBUG "Write call");

	 char*newmsg[] = vmalloc(count * sizeof(char)); //,sizeof(char));/*message memory allocation or new message */
    if (!newmsg) {                     /*check allocation insuccess*/
            retval = -ENOMEM;           //
            goto out;
    }

    if (copy_from_user(newmsg, buf, count)) {/*check allocation insuccess*/
            retval = -EFAULT;
            goto out;
    }
	vfree(msg);
	msg = vmalloc(count * sizeof(char));

	if(!msg){
		 retval = -ENOMEM;           //
		goto out;
	}

    msg = newmsg;
    newmsg = NULL;
    retval = count;

    out:
        vfree(newmsg);
        return retval;	
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
    	module_put(THIS_MODULE);     //Decrement the use count
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
int result;
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
	printk(KERN_DEBUG "Destroy memo= %d\n",memo_major); 
	unregister_chrdev(memo_major, DEVICE_NAME); 
	vfree (msg);							/*Relese memory allocation*/
}

module_init(memo_init);
module_exit(memo_cleanup);
