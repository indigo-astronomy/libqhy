// Copyright (c) 2016 CloudMakers, s. r. o.
// All rights reserved.
//
// You can use this software under the terms of 'INDIGO Astronomy
// open-source license' (see LICENSE.md).
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHORS 'AS IS' AND ANY EXPRESS
// OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
// GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef libqhy_base_h
#define libqhy_base_h

#include <assert.h>

#include <libusb-1.0/libusb.h>

#define QHY_DEBUG(c) c
#define QHY_TRACE(c)

#define CPUCS_ADDRESS       0xe600
#define USB_TIMEOUT         5000

enum USB_REQUEST {
  USB_RQ_LOAD_FIRMWARE      = 0xa0,
  USB_RQ_WRITE_SMALL_EEPROM = 0xa2
};

typedef struct libqhy_device_context {
	libqhy_camera_type type;
	libusb_device_handle *handle;
	pthread_mutex_t lock;
	int usb_speed;
	int usb_traffic;
	bool stream_mode;
	bool long_time_mode;
	double exposure_time;
	double gain;
	int offset;
	bool is_colour;
	bool has_cooler, has_guider_port;
	int bits_per_pixel;
	int width, height;
	double pixel_width, pixel_height;
	int max_bin_hor, max_bin_vert;
	int frame_width, frame_height, frame_bits_per_pixel;
	unsigned short reg300c;
	double ppl_ratio;
	pthread_mutex_t usb_mutex;
} libqhy_device_context;

enum request_type { REQUEST_READ = 0xC0, REQUEST_WRITE = 0x40 };
enum endpoint_type { INTERRUPT_READ_ENDPOINT = 0x81, INTERRUPT_WRITE_ENDPOINT = 0x01, DATA_READ_ENDPOINT = 0x82 };

extern void qhy_log(const char *format, ...);

extern int libqhy_i2c_write(libusb_device_handle *handle, unsigned addr,unsigned short value);
extern int libqhy_i2c_read(libusb_device_handle *handle, unsigned addr, unsigned short *value);

#endif /* libqhy_base_h */
