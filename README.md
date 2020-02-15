A simple Linux module that allows you to write your text and read it when you ask for it


COMPILE:
* Switch to root user
* Move on folder that contains the file
* Run the command  # make

HOW TO LOAD MODULE:
* Run commnad # sudo insmod Memo.ko
* Run commnad for retrive the value for Major number   # dmesg  
* Run commnad and replace the [majorNumber] with your major number # sudo mknod /dev/Memo c [majorNumber] 0 


HOW TO USE IT:
* for write your message  run command # echo "Your Message" > /dev/Memo
* for read  your message  run command # cut /dev/Memo

HOW TO UNLOAD MODULE:
* Run commnad # sudo rmmod Memo.ko


