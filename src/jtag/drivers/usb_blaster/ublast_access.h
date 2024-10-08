/* SPDX-License-Identifier: GPL-2.0-or-later */

/*
 *   Driver for USB-JTAG, Altera USB-Blaster and compatibles
 *
 *   Inspired from original code from Kolja Waschk's USB-JTAG project
 *   (http://www.ixo.de/info/usb_jtag/), and from openocd project.
 *
 *   Copyright (C) 2013 Franck Jullien franck.jullien@gmail.com
 *   Copyright (C) 2012 Robert Jarzmik robert.jarzmik@free.fr
 *   Copyright (C) 2011 Ali Lown ali@lown.me.uk
 *   Copyright (C) 2009 Catalin Patulea cat@vv.carleton.ca
 *   Copyright (C) 2006 Kolja Waschk usbjtag@ixo.de
 *
 */

#ifndef OPENOCD_JTAG_DRIVERS_USB_BLASTER_UBLAST_ACCESS_H
#define OPENOCD_JTAG_DRIVERS_USB_BLASTER_UBLAST_ACCESS_H

/* Low level flags */
#define COPY_TDO_BUFFER		(1 << 0)

struct ublast_lowlevel {
	uint16_t ublast_vid;
	uint16_t ublast_pid;
	uint16_t ublast_vid_uninit;
	uint16_t ublast_pid_uninit;
	struct libusb_device_handle *libusb_dev;
	char *firmware_path;

	int (*write)(struct ublast_lowlevel *low, uint8_t *buf, int size,
		     uint32_t *bytes_written);
	int (*read)(struct ublast_lowlevel *low, uint8_t *buf, unsigned int size,
		    uint32_t *bytes_read);
	int (*open)(struct ublast_lowlevel *low);
	int (*close)(struct ublast_lowlevel *low);
	int (*speed)(struct ublast_lowlevel *low, int speed);

	void *priv;
	int flags;
};

/**
 * ublast_register_ftdi - get a lowlevel USB Blaster driver
 * ublast2_register_libusb - get a lowlevel USB Blaster II driver
 *
 * Get a lowlevel USB-Blaster driver. In the current implementation, there are 2
 * possible lowlevel drivers :
 *  - one based on libftdi,
 *  - one based on libusb, specific to the USB-Blaster II
 *
 * Returns the lowlevel driver structure.
 */
extern struct ublast_lowlevel *ublast_register_ftdi(void);
extern struct ublast_lowlevel *ublast2_register_libusb(void);

#endif /* OPENOCD_JTAG_DRIVERS_USB_BLASTER_UBLAST_ACCESS_H */
