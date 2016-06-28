/*
 * libusb example program to list devices on the bus
 * Copyright (C) 2007 Daniel Drake <dsd@gentoo.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <stdio.h>
#include <sys/types.h>

#include <libusb.h>
#include <names.h>

static void print_devs(libusb_device **devs)
{
	libusb_device *dev;
	int i = 0;
	int err;
	char path_to_usbids[] = "./usb.ids";
	const char *str;

	err = names_init(path_to_usbids);
	if (err != 0) {
	    printf("open %s failed\n", path_to_usbids);
	}

	while ((dev = devs[i++]) != NULL) {
		struct libusb_device_descriptor desc;
		int r = libusb_get_device_descriptor(dev, &desc);
		if (r < 0) {
			fprintf(stderr, "failed to get device descriptor");
			return;
		}

		printf("Bus %03d Device %03d: ID %04x:%04x %s",
		        libusb_get_bus_number(dev), libusb_get_device_address(dev),
			desc.idVendor, desc.idProduct,
			names_vendor(desc.idVendor));
		
		if ((str = names_product(desc.idVendor, desc.idProduct)) != NULL) {
		    printf(" %s", str);
		}
		printf("\n");
	}
	
	names_exit();
}

int main(void)
{
	libusb_device **devs;
	int r;
	ssize_t cnt;

	r = libusb_init(NULL);
	if (r < 0)
		return r;

	cnt = libusb_get_device_list(NULL, &devs);
	if (cnt < 0)
		return (int) cnt;

	print_devs(devs);
	libusb_free_device_list(devs, 1);

	libusb_exit(NULL);
	return 0;
}

