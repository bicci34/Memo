
#include <linux/module.h>
#include <linux/kernel.h> 
#include <linux/fs.h>     
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/gfp.h>

MODULE_LICENSE("GPL");//GLOBAL PUBLIC LICENSE             
MODULE_AUTHOR("Francesco Bicciato");                                    
MODULE_DESCRIPTION("Store and read your message");      
MODULE_VERSION("1.0");  

/*Global variables*/
#define DEVICE_NAME "Memo"
#define SUCCESS 0
static int memo_major = 0;
static int device_Open = 0;	/* Is device open? */

static char* msg = NULL ;
static size_t msg_size = 0;
static char empty_msg[] = "No content to read\n";
static size_t empty_msg_size = sizeof(empty_msg);



/*Return message stored from user*/
static ssize_t memo_read(struct file *filp, char __user *buf, size_t count, loff_t *pos)
{
	if(!msg){//Check if user has write the message		
		return simple_read_from_buffer(buf, count, pos, empty_msg, empty_msg_size);	//Return nothing to read
	}
	return 	 simple_read_from_buffer(buf, count, pos, msg, msg_size);	//Return user message 
}

/*Store user message*/
static ssize_t memo_write(struct file *filp, const char __user *buf, size_t count, loff_t *pos)
{   
	if(count != msg_size) {//check if the new message has a different length than the previous one
		printk(KERN_DEBUG "Need msg reallocation to char =%ld\n",count);
		if (msg){// clear message when the message contains value 			
			kfree(msg);
		}
		msg = kcalloc(count,sizeof(char), GFP_KERNEL);//allocate memory according to new message 
		if(!msg){//check if the allocation was successful
			return -ENOMEM;
		}		
	}
	msg_size = count;
	return 	 simple_write_to_buffer(msg, msg_size, pos, buf, count);	
   
}

static int memo_open(struct inode *inode, struct file *filp)
{
	printk(KERN_DEBUG "Open method called\n");
	if (device_Open){ //when file is already open return busy
		return -EBUSY;
	}
	device_Open++; //Set device as Busy
	try_module_get(THIS_MODULE);    //Increment the use count
	return SUCCESS; 
}

static int memo_release(struct inode *inode, struct file *filp)
{	
	device_Open--;              //Release the operation on file 
	module_put(THIS_MODULE);     //Decrement the use count
	return SUCCESS; 
}
static struct file_operations memo_fops = { //Attach module operation  
	.owner = THIS_MODULE,
	.read =  memo_read, 
	.write = memo_write,
	.open = memo_open,
	.release = memo_release
};

/*Initialize new device*/
int memo_init(void){
	int result;
	printk(KERN_DEBUG "\nInitialise memo\n");
	
	result = register_chrdev(memo_major, DEVICE_NAME, & memo_fops);/* Assign major number, name and opration supported by this device */

	if (result < 0){     //Error during registration
		return result;
	}
	if (memo_major == 0)  {  

		memo_major = result; /* Dynamic major number will be assignment */
		printk(KERN_DEBUG "Major N = %d\n",memo_major);
	} 
	return SUCCESS;/*registration sucess and */
}

/*Unregister the device*/
void memo_cleanup(void)
{
	printk(KERN_DEBUG "Destroy memo= %d\n",memo_major); 
	unregister_chrdev(memo_major, DEVICE_NAME); 
}

module_init(memo_init);
module_exit(memo_cleanup);
