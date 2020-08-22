#define _DEFAULT_SOURCE
#include "3dscapture.h"
#include <stdio.h>
#include <libusb-1.0/libusb.h>
#include <unistd.h>

enum {
    VID_3DS                 = 0x16D0,
    PID_3DS                 = 0x06A3,
    DEFAULT_CONFIGURATION   = 1,
    CAPTURE_INTERFACE       = 0,
    CONTROL_TIMEOUT         = 30,
    EP2_TIMEOUT             = 50,
    EP2_IN                  = 2 | LIBUSB_ENDPOINT_IN,

    CMDIN_I2C_READ          = 0x21,
    CMDOUT_I2C_WRITE        = 0x21,
    CMDOUT_CAPTURE_START    = 0x40,
    
    I2CADDR_3DSCONFIG       = 0x14,
    _3DSCFG_BITSTREAM_VER   = 1,
};

static libusb_device_handle *dev = NULL;
static bool init=false;

// Read vendor request from control endpoint.  Returns bytes transferred (<0 = libusb error)
static int vend_in(uint8_t bRequest, uint16_t wValue, /* uint16_t wIndex, */ uint16_t wLength, uint8_t *buf) {
    return libusb_control_transfer(dev, (LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_ENDPOINT_IN), bRequest, wValue, 0, buf, wLength, CONTROL_TIMEOUT);
}

// Write vendor request to control endpoint.  Returns bytes transferred (<0 = libusb error)
static int vend_out(uint8_t bRequest, uint16_t wValue, /* uint16_t wIndex, */ uint16_t wLength, uint8_t *buf) {
    return libusb_control_transfer(dev, (LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_ENDPOINT_OUT), bRequest, wValue, 0, buf, wLength, CONTROL_TIMEOUT);
}

// Read from bulk endpoint.  Returns libusb error code
static int bulk_in(uint8_t *buf, int length, int *transferred) {
    return libusb_bulk_transfer(dev, EP2_IN, buf, length, transferred, EP2_TIMEOUT);
}

// Read FPGA configuration regs
static bool readConfig(uint8_t cfgAddr, uint8_t *buf, int count) {
    if(    (count>0 && count<=256)
        && (vend_out(CMDOUT_I2C_WRITE, I2CADDR_3DSCONFIG, 1, &cfgAddr))
        && (vend_in(CMDIN_I2C_READ, I2CADDR_3DSCONFIG, count, buf)==count)
    ) return true;
    return false;
}



// Open capture device (only call once)
bool capture_init() {

    if(libusb_init(NULL)!=LIBUSB_SUCCESS)
        return false;
    init=true;

    dev = libusb_open_device_with_vid_pid(NULL, VID_3DS, PID_3DS);
    if(!dev)
        goto err;
    if(libusb_set_configuration(dev, DEFAULT_CONFIGURATION) != LIBUSB_SUCCESS)
        goto err;
    if(libusb_claim_interface(dev, CAPTURE_INTERFACE) != LIBUSB_SUCCESS)
        goto err;

    //FW bug(?) workaround- first read times out sometimes
    uint8_t dummy;
    vend_out(CMDOUT_CAPTURE_START, 0, 0, &dummy);
    usleep(50000);

    return true;

err:
    capture_deinit();
    return false;
}

// Close capture device
void capture_deinit() {
    if(dev) {
        libusb_release_interface(dev, CAPTURE_INTERFACE);
        libusb_close(dev);
        dev=NULL;
    }
    if(init) {
        libusb_exit(NULL);
        init=false;
    }
}

// Gets 240x720 RGB24 (rotated) frame.
// frameBuf should be FRAMEBUFSIZE big.
int capture_grabFrame(uint8_t *frameBuf) {
    int result=vend_out(CMDOUT_CAPTURE_START, 0, 0, frameBuf);
    if(result<0)
        return (result==LIBUSB_ERROR_TIMEOUT)? CAPTURE_SKIP: CAPTURE_ERROR;

    //3DS sends data until end of frame, then 0-length packets.
    int transferSize;
    int transferred;
    int bytesIn=0;
    do {
        transferSize=((FRAMESIZE-bytesIn)+0x1ff) & ~0x1ff;  //multiple of maxPacketSize
        result = bulk_in(frameBuf, transferSize, &transferred);
        if(result==LIBUSB_SUCCESS) {
            bytesIn+=transferred;
            frameBuf+=transferred;
        }
    } while(bytesIn<FRAMESIZE && result==LIBUSB_SUCCESS && transferred>0);
	
    if(result==LIBUSB_ERROR_PIPE) {
        libusb_clear_halt(dev, EP2_IN);
        return CAPTURE_ERROR;
    } else if(result==LIBUSB_ERROR_TIMEOUT || bytesIn<FRAMESIZE) {
        return CAPTURE_SKIP;
    } else if(result==LIBUSB_SUCCESS) {
        return CAPTURE_OK;
    } else {
        return CAPTURE_ERROR;
    }
}

// 5 = first public release
// 6 = 3D capture support (beta)
uint8_t capture_getVersion() {
    uint8_t version=0;
    readConfig(_3DSCFG_BITSTREAM_VER, &version, 1);
    return version;
}

void toRGBA(const uint8_t* rgb, uint8_t* rgba) {
    size_t i;
    for (i = 0; i < FRAMESIZE / 3; i++) {
        rgba[4*i] = rgb[3*i];
        rgba[4*i+1] = rgb[3*i+1];
        rgba[4*i+2] = rgb[3*i+2];
        rgba[4*i+3] = 255;
    }
}
