# HoopCentral

To install and run the HC-SR04 device driver:

1. cd to the km directory and compile the driver file, hcsr04.c:

make

2. Create a device node:

mknod /dev/hcsr04

3. Insert the module into the kernel

insmod hcsr04.ko


To run the bluetooth speaker program:

1. cd to the player directory and compile the files:

make

2. To run:

./player  <songs_directory_path>
