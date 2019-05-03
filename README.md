# HoopCentral

To install and run the HC-SR04 device driver:

1. Enter the km directory and compile the driver file, hcsr04.c

make

2. Create a device node:

mknod /dev/hcsr04

3. Insert the module into the kernel

insmod hcsr04.ko
