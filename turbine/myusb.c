#include <stdio.h>
#include <libusb-1.0/libusb.h>
#include "myusb.h"

static libusb_device_handle * Device = NULL;
libusb_context *ctx = NULL;


int myusb_init(){ // will become init()
	libusb_device **devs;	 	// pointer to pointer of device;
	int r;				// for return values
	ssize_t cnt;			//holding nubmer of devices in list
	struct libusb_device_descriptor desc;

	r = libusb_init(&ctx);			// initialize a library session
	if (r < 0) {
		printf("Init Error %d\n",r);	// there was an error
		return(1);
	}
	libusb_set_debug(ctx, 3);		// set verbosity level to 3 
	cnt = libusb_get_device_list(ctx, &devs);//get the list of devices
	if (cnt < 0){
		printf("Get Device Error\n");	// there was an error
	}
	printf ("%d Devices in list.\n", cnt);

	Device = libusb_open_device_with_vid_pid(ctx, 0x0403, 0xc5b9);
	if (Device == NULL){
		printf("Device not found\n");
		return( -1);
	} else {
		printf("Device opened\n");
		r = 0;
	}
	if (libusb_kernel_driver_active(Device,0) == 1) {
		printf("Kernel Driver active\n");
		if (libusb_detach_kernel_driver(Device, 0) == 0) {// detatch it
			printf("Kernel Driver Detached\n");
			r = 1;
		} else {
			return (-1);
		}
	}
	libusb_free_device_list(devs, 1); // free the list
	return (r);
}

int myusb_write(char * mem, size_t len){
	int actual;
	int r;
	r = libusb_claim_interface(Device, 0);
	if (r < 0) {
		printf("Cannot claim Interface 0\n");
		return -1;
	}
	printf("claimed interface 0 for write\n");
	r = libusb_bulk_transfer(Device, (2|LIBUSB_ENDPOINT_OUT), mem, len, &actual, 0);
	if (r == 0 && actual == len){
		printf("write successful\n");
	}
	libusb_release_interface(Device, 0);
	return actual;
}

int myusb_read(char* mem, size_t len, int timeout){
	int actual;
	int r;
        r = libusb_claim_interface(Device, 0);
        if (r < 0) {
                printf("Cannot claim Interface 0\n");
                return -1;
        }
        printf("claimed interface 0 for read\n");
	r = libusb_bulk_transfer(Device, (1|LIBUSB_ENDPOINT_IN), mem, len, &actual,timeout);
	libusb_release_interface(Device, 0);
	return r;
}
	
void myusb_close(){
	
	libusb_close(Device);
	libusb_exit(ctx);
	Device = NULL;
}
	
