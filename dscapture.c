#include "dscapture.h"
#include <stdio.h>
#include <memory.h>
#include <libusb-1.0/libusb.h>

enum {
    USBDS_VID               = 0x16D0,
    USBDS_PID               = 0x0647,
    DEFAULT_CONFIGURATION   = 1,
    CAPTURE_INTERFACE       = 0,
    CONTROL_TIMEOUT         = 500,
    BULK_TIMEOUT            = 500,
    EP2_IN                  = 2 | LIBUSB_ENDPOINT_IN,

    //device vendor requests:
    CMDIN_STATUS            = 0x31,
        // Returns device status:
        // struct {
        //     u32 framecount,              //free running frame counter
        //     u8 lcd_on,
        //     u8 capture_in_progress,
        // }
    CMDIN_FRAMEINFO         = 0x30,
        // Returns record of last captured frame:
        // struct {
        //     u8 bitmap[48],     //bitmap of lines sent (1 bit per half-line)
        //     u32 frame,         //frame number
        //     u8 valid,          //0 if capture timed out (LCD is inactive)
        // }
    CMDOUT_CAPTURE_START    = 0x30,
        // capture new frame
    CMDOUT_CAPTURE_STOP     = 0x31,
        // stop capture in progress and reset frame counter to 0
};



static libusb_device_handle *dev = NULL;
static bool init=false;


// Open DS Capture device (call once)
bool capture_init() {

    if(libusb_init(NULL)!=LIBUSB_SUCCESS)
        return false;
    init=true;

    dev = libusb_open_device_with_vid_pid(NULL, USBDS_VID, USBDS_PID);
    if(!dev)
        goto err;
    if(libusb_set_configuration(dev, DEFAULT_CONFIGURATION) != LIBUSB_SUCCESS)
        goto err;
    if(libusb_claim_interface(dev, CAPTURE_INTERFACE) != LIBUSB_SUCCESS)
        goto err;
    return true;

err:
    capture_deinit();
    return false;
}

// Close DS Capture device (call before exiting)
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

// Read vendor request from control endpoint.  Returns bytes transferred (<0 = libusb error)
static int vend_in(uint8_t bRequest, /* uint16_t wValue, uint16_t wIndex, */ uint16_t wLength, uint8_t *buf) {
    return libusb_control_transfer(dev, (LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_ENDPOINT_IN), bRequest, 0, 0, buf, wLength, CONTROL_TIMEOUT);
}

// Write vendor request to control endpoint.  Returns bytes transferred (<0 = libusb error)
static int vend_out(uint8_t bRequest, uint16_t wValue, /* uint16_t wIndex, */ uint16_t wLength, uint8_t *buf) {
    return libusb_control_transfer(dev, (LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_ENDPOINT_OUT), bRequest, wValue, 0, buf, wLength, CONTROL_TIMEOUT);
}

// Read from bulk endpoint.  Returns libusb error code
static int bulk_in(uint8_t *buf, int length, int *transferred) {
    return libusb_bulk_transfer(dev, EP2_IN, buf, length, transferred, BULK_TIMEOUT);
}

// frameBuf is 256x384 BGR16
bool capture_grabFrame(uint16_t *frameBuf) {
    enum {
        infoSize=64,
        lcdWidth=256,
        lcdHeight=192,
        frameSize=1024*lcdHeight,
    };

    static uint16_t tmpBuf[frameSize/sizeof(uint16_t)];
    static uint8_t frameInfo[infoSize];
	
    uint8_t dummy;
    if(vend_out(CMDOUT_CAPTURE_START, 0, 0, &dummy) < 0)
        return false;

    //DS sends data until end of frame, then sends 0-length packets.

    int transferred;
    int result;
    int bytesIn=0;
    uint8_t *p=(uint8_t*)tmpBuf;
    do {
        result = bulk_in(p, frameSize-bytesIn, &transferred);
        if(result==LIBUSB_SUCCESS) {
            bytesIn+=transferred;
            p+=transferred;
        }
    } while(bytesIn<frameSize && result==LIBUSB_SUCCESS && transferred>0);

    if(result!=LIBUSB_SUCCESS)
        return false;
    if(vend_in(CMDIN_FRAMEINFO, infoSize, frameInfo)<0)
        return false;
    if((frameInfo[0]&3)!=3)   //First line is valid.  Should always be true, simplifies filling code below.
        return false;
    if(!frameInfo[52])        //LCD was off
        return false;

    //de-interleave pixels and deal with lost packets

    int i;
    int line;
    uint16_t *src=tmpBuf;
    uint16_t *dst=frameBuf;

    for(line=0; line<lcdHeight*2; ++line) {
        if(frameInfo[line>>3] & (1<<(line&7))) {
            for(i=0; i<lcdWidth/2; ++i) {
                dst[0]=src[1];
                dst[lcdWidth*lcdHeight]=src[0];
                dst++;
                src+=2;
            }
        } else {        //line missing, copy previous
            memcpy(dst, dst-256, 256);
            memcpy(dst+256*192, dst+256*191, 256);
            dst+=128;
            //linesLost++;
        }
    }
    return true;
}

void toRGBA(uint8_t *out, uint16_t *in) {
    int i;
    for(i=0;i<FRAMEWIDTH*FRAMEHEIGHT*2;i++) {
        unsigned char r,g,b;
        g=((*in)>>5)&0x3f;
        b=((*in<<1)&0x3e)|(g&1);
        r=(((*in)>>10)&0x3e)|(g&1);
        out[0]=(r<<2) | (r>>4);
        out[1]=(g<<2) | (g>>4);
        out[2]=(b<<2) | (b>>4);
        out[3]=255;
        out+=4;
        in++;
    }
}
