A simple Linux module that allows you to write your text and read it when you ask for it


COMPILE:
* Switch to root user
* Install compiler # sudo apt-get install builder-essential
* Install the headers # sudo apt-get install linux-headers-$(uname -r)
* Move on folder that contains the file
* Compile module  # make

HOW TO LOAD MODULE:
* Load module # sudo insmod Memo.ko
* Run commnad for retrive the Major's number value  # dmesg  
* Run commnad and replace the [majorNumber] with your major number #  mknod /dev/Memo c [majorNumber] 0 

HOW TO USE IT:
* For write your message  run command # echo "Your Message" > /dev/Memo
* For read  your message  run command # cat /dev/Memo

HOW TO UNLOAD MODULE:
* Create node # rm /dev/Memo
* Run commnad # sudo rmmod Memo.ko


