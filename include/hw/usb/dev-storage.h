#ifndef HW_USB_DEV_STORAGE_H
#define HW_USB_DEV_STORAGE_H

//copy internal definitions in dev-storage.c here to share with converge bus
#include "qemu/osdep.h"
#include "qapi/error.h"
#include "qemu-common.h"
#include "qemu/error-report.h"
#include "qemu/option.h"
#include "qemu/config-file.h"
#include "hw/usb.h"
//#include "desc.h"
#include "hw/scsi/scsi.h"
#include "ui/console.h"
#include "monitor/monitor.h"
#include "sysemu/sysemu.h"
#include "sysemu/block-backend.h"
#include "qapi/visitor.h"
#include "qemu/cutils.h"

//#define DEBUG_MSD

#ifdef DEBUG_MSD
#define DPRINTF(fmt, ...) \
do { printf("usb-msd: " fmt , ## __VA_ARGS__); } while (0)
#else
#define DPRINTF(fmt, ...) do {} while(0)
#endif

/* USB requests.  */
#define MassStorageReset  0xff
#define GetMaxLun         0xfe

enum USBMSDMode {
    USB_MSDM_CBW, /* Command Block.  */
    USB_MSDM_DATAOUT, /* Transfer data to device.  */
    USB_MSDM_DATAIN, /* Transfer data from device.  */
    USB_MSDM_CSW /* Command Status.  */
};

struct usb_msd_csw {
    uint32_t sig;
    uint32_t tag;
    uint32_t residue;
    uint8_t status;
};

typedef struct {
    USBDevice dev;
    enum USBMSDMode mode;
    uint32_t scsi_off;
    uint32_t scsi_len;
    uint32_t data_len;
    struct usb_msd_csw csw;
    SCSIRequest *req;
    SCSIBus bus;
    /* For async completion.  */
    USBPacket *packet;
    /* usb-storage only */
    BlockConf conf;
    uint32_t removable;
    SCSIDevice *scsi_dev;
} MSDState;

#define TYPE_USB_STORAGE "usb-storage-dev"
#define USB_STORAGE_DEV(obj) OBJECT_CHECK(MSDState, (obj), TYPE_USB_STORAGE)

struct usb_msd_cbw {
    uint32_t sig;
    uint32_t tag;
    uint32_t data_len;
    uint8_t flags;
    uint8_t lun;
    uint8_t cmd_len;
    uint8_t cmd[16];
};

enum {
    STR_MANUFACTURER = 1,
    STR_PRODUCT,
    STR_SERIALNUMBER,
    STR_CONFIG_FULL,
    STR_CONFIG_HIGH,
    STR_CONFIG_SUPER,
};


#endif /* HW_USB_DEV_STORAGE_H */
