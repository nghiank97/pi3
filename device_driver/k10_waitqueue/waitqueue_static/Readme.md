# Create the file operation
```
    cdev_init(&my_cdev, &fops);
    my_cdev.owner = THIS_MODULE;
    my_cdev.ops = &fops;
```
# Create the thread
```
    wait_thread = kthread_create(wait_function, NULL, "WaitThread");
    if (wait_thread) {
      pr_info(" Thread Created successfully\n");
      wake_up_process(wait_thread);
    } else
      pr_info(" Thread creation failed\n");
```
# Create the wait function
```
static int wait_function(void *unused){
  while(1) {
    pr_info(" Waiting For Event...\n");
    wait_event_interruptible(wait_queue_etx, wait_queue_flag != 0 );
    switch (wait_queue_flag){
      case 1:{
        pr_info(" Event Came From Read Function : %d\n", ++read_count);
        wait_queue_flag = 0;
        break;
      }
      case 2:{
        pr_info(" Event Came From Write Function : %d\n", ++read_count);
        wait_queue_flag = 0;
        break;
      }
      case 3:{
        pr_info(" Event Came From Exit Function\n");
        return 0;
      }
    }
  }
  do_exit(0);
  return 0;
}
```
# Start the device driver
```
	$ make start
```
```
[ 6017.166164]  Starting
[ 6017.166186]  Major = 239 Minor = 0
[ 6017.167554]  Thread Created successfully
[ 6017.167573]  Device Driver Insert...Done!!!
[ 6017.167580]  Waiting For Event...
```
pr_info(" Waiting For Event...\n");
wait_event_interruptible(wait_queue_etx, wait_queue_flag != 0 );
-> "[ 6017.167580]  Waiting For Event..."

# Write the device file
```
	$ sudo echo 1 >/dev/my_dev
```
```
[ 6254.263289]  Drive open function called
[ 6254.311288]  Write Function
[ 6254.311335]  Drive write function call
[ 6254.311361]  Event Came From Write Function : 1
[ 6254.311374]  Waiting For Event...
[ 6254.316629]  Drive release function call
```
```
pr_info(" Write Function\n");
wait_queue_flag = 2;
wake_up_interruptible(&wait_queue_etx);
```
-> "[ 6254.311361]  Event Came From Write Function : 1"

The function "wait_function" is called again
```
static int wait_function(void *unused){
  while(1) {
```
-> "[ 6254.311374]  Waiting For Event..."