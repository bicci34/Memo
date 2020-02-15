A simple Linux module that allows you to write your text and read it when you ask for it


COMPILE:
* Switch to root user
* Install compiler # sudo apt-get install builder-essential
* Install the headers # sudo apt-get install linux-headers-$(uname -r)
* Move on folder that contains the file
* Compile module  # make

HOW TO LOAD MODULE:
* Run commnad # sudo insmod Memo.ko
* Run commnad for retrive the value for Major number   # dmesg  
* Run commnad and replace the [majorNumber] with your major number # sudo mknod /dev/Memo c [majorNumber] 0 

HOW TO USE IT:
* for write your message  run command # echo "Your Message" > /dev/Memo
* for read  your message  run command # cut /dev/Memo

HOW TO UNLOAD MODULE:
* Run commnad # sudo rmmod Memo.ko


