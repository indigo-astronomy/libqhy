//  Copyright (c) 2016 CloudMakers, s. r. o.
//  All rights reserved.
//
//	You can use this software under the terms of 'INDIGO Astronomy
//  open-source license' (see LICENSE.md).
//
//	THIS SOFTWARE IS PROVIDED BY THE AUTHORS 'AS IS' AND ANY EXPRESS
//	OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//	ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
//	DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
//	DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
//	GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
//	INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
//	WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
//	NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#include <libqhy/libqhy.h>

#include "libqhy_base.h"
#include "libqhy_5ii.h"

//static int set_resolution(libqhy_device_context *context, unsigned frame_width, unsigned frame_height);
//static int set_exposure_time(libqhy_device_context *context, double time);
//static int set_usb_speed(libqhy_device_context *context);
//static int set_usb_traffic(libqhy_device_context *context);

static int write_regs(libqhy_device_context *context, unsigned short data[][3]) {
	libusb_device_handle *handle = context->handle;
	int rc = 0;
	int delay;
	for (int i = 0; rc >= 0 && data[i][0] != 0; i++) {
		rc = libqhy_i2c_write(handle, data[i][0], data[i][1]);
		if ((delay = data[i][2]) != 0)
			usleep(1000 * delay);
	}
	return rc;
}

static int init_regs(libqhy_device_context *context) {
  switch (context->type) {
    case QHY_5LII:
			return write_regs(context, libqhy_5lii_init);
    case QHY_5RII:
			return write_regs(context, libqhy_5rii_init);
    default:
			return 0;
  }
}

static int set_ppl(libqhy_device_context *context, int ppl) {
	libusb_device_handle *handle = context->handle;
	int rc = 0;
	rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x302A, 14);
	rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x302C, 1);
	rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x302E, 3);
	if (ppl == 0)
		rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3030, 42);
	else if (ppl == 1)
		rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3030, 65);
	else if (ppl == 2)
		rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3030, 57);
	rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3082, 0x0029);
	if (context->type == QHY_5LII) {
		if (context->stream_mode) {
			if (context->long_time_mode)
				rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x30B0, 0x5330);
			else
				rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x30B0, 0x1330);
		} else {
			rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x30B0, 0x5330);
		}
	} else {
		rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x30B0, 0x1330);
	}
	rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x305e, 0x0020);
	rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3EE4, 0xD208);
	rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3064, 0x1802);
	return rc;
}

static int set_resolution(libqhy_device_context *context, unsigned frame_width, unsigned frame_height) {
	if (context->frame_width == frame_width && context->frame_height == frame_height)
		return 0;
  libusb_device_handle *handle = context->handle;
  int rc = 0;
  switch (context->type) {
    case QHY_5II:
		case QHY_5PII:
		case QHY_5TII:
			context->frame_width = frame_width;
			context->frame_height = frame_height;
			if (context->type == QHY_5TII) {
				frame_width = (frame_width + 3) / 4 * 4;
				frame_height = (frame_height + 3) / 4 * 4;
			} else if (context->type == QHY_5II) {
				rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x09, 200);
			}
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x01, 8 + (frame_height - context->height) / 2);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x02, 16 + (frame_width - context->width) / 2);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x03, frame_height - 1);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x04, frame_width - 1);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x22, 0x00);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x23, 0x00);
      break;
		case QHY_5LII:
		case QHY_5HII: {
			unsigned frame_length, line_length;
			rc = init_regs(context);
			if (frame_width <= 320 && frame_height <= 240) {
				frame_width = 320;
				frame_height = 240;
				frame_length = 266;
				line_length = 1388;
				rc = rc < 0 ? rc : set_ppl(context, 1);
			} else if (frame_width <= 640 && frame_height <= 480) {
				frame_width = 640;
				frame_height = 480;
				frame_length = 506;
				line_length = 1388;
				rc = rc < 0 ? rc : set_ppl(context, 1);
			} else if (frame_width <= 800 && frame_height <= 600) {
				frame_width = 800;
				frame_height = 600;
				frame_length = 626;
				line_length = 1388;
				rc = rc < 0 ? rc : set_ppl(context, 2);
			} else if (frame_width <= 1024 && frame_height <= 768) {
				frame_width = 1024;
				frame_height = 768;
				frame_length = 795;
				line_length = 1388;
				rc = rc < 0 ? rc : set_ppl(context, 0);
			} else {
				frame_width = 1280;
				frame_height = 960;
				frame_length = 990;
				line_length = 1650;
				rc = rc < 0 ? rc : set_ppl(context, 0);
			}
			context->frame_width = frame_width;
			context->frame_height = frame_height;
			int left = 4 + (1280 - frame_width) / 2;
			int top = 4 + (960 - frame_height) / 2;
			int width = frame_width - 1;
			int height = frame_height - 1;
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3002, left);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3004, top);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3006, top + height);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3008, left + width);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x300a, frame_length);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x300c, line_length);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x301A, 0x10DC);
      break;
		}
		case QHY_5RII: {
			unsigned reg300A, reg3012;
			if (frame_width <= 320 && frame_height <= 240) {
				context->frame_width = frame_width = 320;
				context->frame_height = frame_height = 240;
				reg300A = 0x010d;
				reg3012 = 0x005a;
			} else if (frame_width <= 640 && frame_height <= 480) {
				context->frame_width = frame_width = 640;
				context->frame_height = frame_height = 480;
				reg300A = 0x01fd;
				reg3012 = 0x0038;
			} else {
				context->frame_width = frame_width = 728;
				context->frame_height = frame_height = 512;
				reg300A = 0x021d;
				reg3012 = 0x0033;
			}
			rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x301A,0x10d0);
			usleep(1000);
			rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3002, 0x0000);
			rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3004, 0x0000);
			rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x300A, reg300A);
			rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3006, frame_width - 1);
			rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3008, frame_height - 1);
			rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x300c, 0x01d5);
			rc = rc < 0 ? rc : libqhy_i2c_read(handle, 0x300c, &context->reg300c);
			rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3012, reg3012);
			rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3012, reg3012);
      usleep(4000);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x301A,0x10d0);
      usleep(1000);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x301A,0x10d4);
      break;
		}
    default:
      break;
  }
  return rc;
}

static int set_bits_per_pixel(libqhy_device_context *context, unsigned bits_per_pixel) {
	unsigned char data[1] = { bits_per_pixel == 16 ? 1 : 0 };
	context->frame_bits_per_pixel = bits_per_pixel;
	int rc = libusb_control_transfer(context->handle, REQUEST_WRITE, 0xcd, 0, 0, data, 1, 3000);
	QHY_DEBUG(qhy_log("libusb_control_transfer [%d] -> %s", __LINE__, rc < 0 ? libusb_error_name(rc) : "OK"));
	return rc;
}

static int set_exposure_time(libqhy_device_context *context, double time) {
  libusb_device_handle *handle = context->handle;
  int rc = 0;
  switch (context->type) {
    case QHY_5II: {
      double cmosclk;
      if (context->usb_speed == 2)
        cmosclk = 48;
      else if (context->usb_speed == 1)
        cmosclk = 24;
      else
        cmosclk = 12;
      uint16_t reg04, reg05, reg0C, reg09;
      rc = rc < 0 ? rc : libqhy_i2c_read(handle, 0x04, &reg04);
      rc = rc < 0 ? rc : libqhy_i2c_read(handle, 0x05, &reg05);
      rc = rc < 0 ? rc : libqhy_i2c_read(handle, 0x09, &reg09);
      rc = rc < 0 ? rc : libqhy_i2c_read(handle, 0x0c, &reg0C);
      double pixelPeriod = 1.0 / cmosclk;
      uint32_t exp_time = time;
      double a = reg04 + 1;
      double p1 = 242;
      double p2 = 2 + reg05 - 19;
      double q = p1 + p2;
      double row_time = (a + q) * pixelPeriod;
      double max_short_exp_time = (15000 * row_time - 180 * pixelPeriod - 4 * reg0C * pixelPeriod);
      uint8_t data[4];
      if (exp_time > max_short_exp_time) {
        rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x09, 15000);
        exp_time = exp_time - max_short_exp_time;
        data[0] = 0;
        data[1] = ((exp_time / 1000) & ~0xff00ffff) >> 16;
        data[2] = ((exp_time / 1000) & ~0xffff00ff) >> 8;
        data[3] = ((exp_time / 1000) & ~0xffffff00);
        rc = rc < 0 ? rc : libusb_control_transfer(handle, REQUEST_WRITE, 0xc1, 0, 0, data, 4, 3000);
        QHY_DEBUG(qhy_log("libusb_control_transfer [%d] -> %s", __LINE__, rc < 0 ? libusb_error_name(rc) : "OK"));
      } else {
        data[0] = 0;
        data[1] = 0;
        data[2] = 0;
        data[3] = 0;
        rc = rc < 0 ? rc : libusb_control_transfer(handle, REQUEST_WRITE, 0xc1, 0, 0, data, 4, 3000);
        QHY_DEBUG(qhy_log("libusb_control_transfer [%d] -> %s", __LINE__, rc < 0 ? libusb_error_name(rc) : "OK"));
        usleep(10000);
        reg09 = (exp_time + 180 * pixelPeriod + 4 * reg0C * pixelPeriod) / row_time;
        if (reg09 < 1)
          reg09 = 1;
        rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x09, reg09);
      }
      break;
    }
    case QHY_5TII:
    case QHY_5PII: {
      double cmosclk;
      if (context->usb_speed == 2)
        cmosclk = 48;
      else if (context->usb_speed == 1)
        cmosclk = 24;
      else
        cmosclk = 12;
      uint16_t reg04, reg05, reg08, reg09, reg0C, reg23, reg22;
      rc = rc < 0 ? rc : libqhy_i2c_read(handle, 0x04, &reg04);
      rc = rc < 0 ? rc : libqhy_i2c_read(handle, 0x05, &reg05);
      rc = rc < 0 ? rc : libqhy_i2c_read(handle, 0x08, &reg08);
      rc = rc < 0 ? rc : libqhy_i2c_read(handle, 0x09, &reg09);
      rc = rc < 0 ? rc : libqhy_i2c_read(handle, 0x0c, &reg0C);
      rc = rc < 0 ? rc : libqhy_i2c_read(handle, 0x22, &reg22);
      rc = rc < 0 ? rc : libqhy_i2c_read(handle, 0x23, &reg23);
      uint16_t reg23_20 = reg23 & ~0xfff8;
      uint16_t reg22_54 = (reg22 & ~0xffcf) / 16;
      uint16_t reg23_54 = (reg23 & ~0xffcf) / 16;
      uint16_t hbmin = 0, max_hb_hbmin, row_time, pixel_period;
      if (reg23_54 == 0 && reg22_54 == 0)
        hbmin = 450;
      if (reg23_54 == 1 && reg22_54 == 0)
        hbmin = 430;
      if (reg23_54 == 2 && reg22_54 == 0)
        hbmin = 420;
      if (reg23_54 == 0 && reg22_54 == 1)
        hbmin = 796;
      if (reg23_54 == 1 && reg22_54 == 1)
        hbmin = 776;
      if (reg23_54 == 2 && reg22_54 == 1)
        hbmin = 766;
      if (reg23_54 == 0 && reg22_54 == 2)
        hbmin = 1488;
      if (reg23_54 == 1 && reg22_54 == 2)
        hbmin = 1468;
      if (reg23_54 == 2 && reg22_54 == 2)
        hbmin = 1458;
      double hb = reg05 + 1;
      double w = (reg04 + 1) / (reg23_20 + 1);
      if (hbmin > hb)
        max_hb_hbmin = hbmin;
      else
        max_hb_hbmin = hb;
      double so = 208 * (reg22_54 + 1) + 98 + reg0C - 94;
      if (w / 2 + max_hb_hbmin > 41 + 186 * (reg22_54 + 1) + 99)
        row_time = 2 * pixel_period * (w / 2 + max_hb_hbmin);
      else
        row_time = 41 + 186 * (reg22_54 + 1) + 99;
      double max_short_exp_time = 15000 * row_time - so * 2 * pixel_period;
      uint32_t exp_time = time;
      uint8_t data[4] = { 0, 0, 0, 0 };
      if (exp_time > max_short_exp_time) {
        rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x09, 15000);
        data[0] = 0;
        data[1] = ((exp_time / 1000) & ~0xff00ffff) >> 16;
        data[2] = ((exp_time / 1000) & ~0xffff00ff) >> 8;
        data[3] = ((exp_time / 1000) & ~0xffffff00);
        rc = rc < 0 ? rc : libusb_control_transfer(handle, REQUEST_WRITE, 0xc1, 0, 0, data, 4, 3000);
        QHY_DEBUG(qhy_log("libusb_control_transfer [%d] -> %s", __LINE__, rc < 0 ? libusb_error_name(rc) : "OK"));
        reg09 = 15000;
      } else {
        rc = rc < 0 ? rc : libusb_control_transfer(handle, REQUEST_WRITE, 0xc1, 0, 0, data, 4, 3000);
        QHY_DEBUG(qhy_log("libusb_control_transfer [%d] -> %s", __LINE__, rc < 0 ? libusb_error_name(rc) : "OK"));
        usleep(100000);
        reg09 = (exp_time + so * 2 * pixel_period) / row_time;
        if (reg09 < 1)
          reg09 = 1;
        rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x09, reg09);
      }
      break;
    }
    case QHY_5LII:
    case QHY_5HII: {
      double cmosclk;
      uint16_t reg300C, reg3012;
      if (context->usb_speed == 2)
        cmosclk = 48;
      else if (context->usb_speed == 1)
        cmosclk = 24;
      else
        cmosclk = 12;
      uint8_t data[4] = { 0, 0, 0, 0 };
      rc = rc < 0 ? rc : libusb_control_transfer(handle, REQUEST_WRITE, 0xc1, 0, 0, data, 4, 3000);
      QHY_DEBUG(qhy_log("libusb_control_transfer [%d] -> %s", __LINE__, rc < 0 ? libusb_error_name(rc) : "OK"));
      usleep(10000);
      double pixelPeriod = 1 / cmosclk; // unit: us
      rc = rc < 0 ? rc : libqhy_i2c_read(handle, 0x300C, &reg300C);
      usleep(1000);
      if(reg300C == 0) {
        rc = rc < 0 ? rc : libqhy_i2c_read(handle, 0x300C, &reg300C);
        usleep(1000);
        rc = rc < 0 ? rc : libqhy_i2c_read(handle, 0x300C, &reg300C);
        usleep(1000);
        rc = rc < 0 ? rc : libqhy_i2c_read(handle, 0x300C, &reg300C);
      }
      double row_time = reg300C * pixelPeriod;
      double max_short_exp_time = 65000 * row_time;
      uint32_t exp_time = time;
      if (exp_time > max_short_exp_time) {
        context->long_time_mode = true;
        libqhy_i2c_write(handle, 0x3012,65000);
        usleep(1000);
        exp_time = exp_time - max_short_exp_time;
        data[0] = 0;
        data[1] = (uint8_t)(((exp_time / 1000) & ~0xff00ffff) >> 16);
        data[2] = (uint8_t)(((exp_time / 1000) & ~0xffff00ff) >> 8);
        data[3] = (uint8_t)((exp_time / 1000) & ~0xffffff00);
        rc = rc < 0 ? rc : libusb_control_transfer(handle, REQUEST_WRITE, 0xc1, 0, 0, data, 4, 3000);
        QHY_DEBUG(qhy_log("libusb_control_transfer [%d] -> %s", __LINE__, rc < 0 ? libusb_error_name(rc) : "OK"));
        exp_time = exp_time + max_short_exp_time;
        reg3012 = 65000;
      } else {
        context->long_time_mode = false;
        rc = rc < 0 ? rc : libusb_control_transfer(handle, REQUEST_WRITE, 0xc1, 0, 0, data, 4, 3000);
        QHY_DEBUG(qhy_log("libusb_control_transfer [%d] -> %s", __LINE__, rc < 0 ? libusb_error_name(rc) : "OK"));
        usleep(10000);
        reg3012 = (uint16_t)(exp_time / row_time);
        if(reg3012 < 1)
          reg3012 = 1;
        rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3012, reg3012);
        usleep(1000);
        exp_time = (uint32_t)(reg3012 * row_time);
      }
      break;
    }
    case QHY_5RII: {
      uint8_t data[4] = { 0, 0, 0, 0 };
      rc = rc < 0 ? rc : libusb_control_transfer(handle, REQUEST_WRITE, 0xc1, 0, 0, data, 4, 3000);
      QHY_DEBUG(qhy_log("libusb_control_transfer [%d] -> %s", __LINE__, rc < 0 ? libusb_error_name(rc) : "OK"));
      double cmosclk;
      if (context->frame_bits_per_pixel == 8) {
        if(context->usb_speed  == 1)
          cmosclk = 12;
        else
          cmosclk = 24;
      } else {
        if(context->usb_speed  == 0)
          cmosclk = 6;
        else
          cmosclk = 12;
      }
      double pixelPeriod = 1 / cmosclk / 2;
      uint16_t reg300C = 0, reg3012;
      rc = rc < 0 ? rc : libqhy_i2c_read(handle, 0x300C, &reg300C);
      rc = rc < 0 ? rc : libqhy_i2c_read(handle, 0x300C, &reg300C);
      rc = rc < 0 ? rc : libqhy_i2c_read(handle, 0x300C, &reg300C);
      double row_time = reg300C * pixelPeriod;
      double max_short_exp_time = 65000 * row_time;
      uint32_t exp_time = time;
      if (exp_time > max_short_exp_time) {
        rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0xC840, 65000);
        usleep(10000);
        rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0xC842, 0xFFFF);
        exp_time = exp_time - max_short_exp_time - pixelPeriod * 1.0 * 0xFFFF;
        data[0] = 0;
        data[1] = (uint8_t)(((exp_time / 1000) & ~0xff00ffff) >> 16);
        data[2] = (uint8_t)(((exp_time / 1000) & ~0xffff00ff) >> 8);
        data[3] = (uint8_t)((exp_time / 1000) & ~0xffffff00);
        rc = rc < 0 ? rc : libusb_control_transfer(handle, REQUEST_WRITE, 0xc1, 0, 0, data, 4, 3000);
        QHY_DEBUG(qhy_log("libusb_control_transfer [%d] -> %s", __LINE__, rc < 0 ? libusb_error_name(rc) : "OK"));
        reg3012 = 65000;
      } else {
        reg3012 = (uint16_t)(exp_time / row_time);
        if (reg3012 < 1)
          reg3012 = 1;
        rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0xC840, reg3012);
        usleep(10000);
        rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0xC842, 0xFFFF);
      }
      break;
    }
    default:
      break;
  }
  return rc;
}

static int set_usb_speed(libqhy_device_context *context, int speed) {
  libusb_device_handle *handle = context->handle;
  int rc = 0;
  uint8_t data[1];
  switch (context->type) {
    case QHY_5II:
    case QHY_5LII:
    case QHY_5PII:
    case QHY_5HII:
    case QHY_5TII:
      if (context->stream_mode) {
        if (context->frame_bits_per_pixel == 8)
          data[0] = context->usb_speed = speed;
        else {
          if(speed >= 1)
            data[0] = context->usb_speed = 1;
          else
            data[0] = context->usb_speed = 0;
        }
      } else {
        data[0] = context->usb_speed;
      }
      rc = rc < 0 ? rc : libusb_control_transfer(handle, REQUEST_WRITE, 0xc8, 0, 0, data, 1, 3000);
      QHY_DEBUG(qhy_log("libusb_control_transfer [%d] -> %s", __LINE__, rc < 0 ? libusb_error_name(rc) : "OK"));
      rc = rc < 0 ? rc : set_exposure_time(context, context->exposure_time);
      break;
    case QHY_5RII:
      if (context->frame_bits_per_pixel == 8) {
        if(speed <= 1)
          data[0] = context->usb_speed = 1;
        else
          data[0] = context->usb_speed = 2;
      } else {
        if(speed >= 1)
          data[0] = context->usb_speed = 1;
        else
          data[0] = context->usb_speed = 0;
      }
      rc = rc < 0 ? rc : libusb_control_transfer(handle, REQUEST_WRITE, 0xc9, 0, 0, data, 1, 3000);
      QHY_DEBUG(qhy_log("libusb_control_transfer [%d] -> %s", __LINE__, rc < 0 ? libusb_error_name(rc) : "OK"));
      break;
    default:
      break;
  }
  return rc;
}

static int set_usb_traffic(libqhy_device_context *context) {
  libusb_device_handle *handle = context->handle;
  int rc = 0;
  switch (context->type) {
    case QHY_5II:
      rc = rc < 0 ? rc : set_exposure_time(context, 1000);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x0005, 9 + context->usb_traffic * 50);
      rc = rc < 0 ? rc : set_exposure_time(context, context->exposure_time);
      break;
    case QHY_5TII:
    case QHY_5PII:
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x0005, 346 + 64 + 70 * context->usb_traffic - 1);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x0006, 26 - 1);
      break;
    case QHY_5LII:
    case QHY_5HII:
      rc = rc < 0 ? rc : set_exposure_time(context, 1000);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x300c, 1650 + context->usb_traffic * 50);
      rc = rc < 0 ? rc : set_exposure_time(context, context->exposure_time);
      break;
    case QHY_5RII:
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x300c, context-> reg300c + context->usb_traffic * 50);
      break;
    default:
      break;
  }
  return rc;
}

static int set_offset(libqhy_device_context *context) {
  libusb_device_handle *handle = context->handle;
  int rc = 0;
  switch (context->type) {
    case QHY_5RII:
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x301e, context->offset);
      break;
    default:
      break;
  }
  return rc;
}

static int set_gain(libqhy_device_context *context, double gain) {
  libusb_device_handle *handle = context->handle;
	context->gain = gain;
  int rc = 0;
  switch (context->type) {
		case QHY_5II: {
			int i = gain / 100 * 72;
			rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x35, libqhy_5ii_gain[i]);
			break;
		}
		case QHY_5TII: {
			double RG = 1, BG = 1;
			gain = 1260 * gain / 1000;
			if (gain <= 7.875 * 2.25) {
				uint32_t a = 0;
				uint32_t b = gain * 8 / 2.25;
				uint32_t c = 10;
				rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x2b, b + a*64 + c*256);
				rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x2e, b + a*64 + c*256);
				c = (2.25 * RG - 1) * 8;
				rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x2d, b + a*64 + c*256);
				c = (2.25 * BG - 1) * 8;
				rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x2c, b + a*64 + c*256);
			} else if (gain <= 11.75 * 2.25) {
				uint32_t a = 1;
				uint32_t b = gain * 4 / 2.25;
				uint32_t c = 10;
				rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x2b, b + a*64 + c*256);
				rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x2e, b + a*64 + c*256);
				c = (2.25 * RG - 1) * 8;
				rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x2d, b + a*64 + c*256);
				c = (2.25 * BG - 1) * 8;
				rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x2c, b + a*64 + c*256);
			} else {
				uint32_t a = 1;
				uint32_t b = 31;
				uint32_t c = (4 * gain / b - 1) * 8;
				rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x2b, b + a*64 + c*256);
				rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x2e, b + a*64 + c*256);
				b = 31 * RG;
				rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x2d, b + a*64 + c*256);
				b = 31 * BG;
				rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x2c, b + a*64 + c*256);
			}
			break;
		}
		case QHY_5PII: {
			int i = gain / 100 * 65;
			rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x35, libqhy_5pii_gain[i]);
			break;
		}
		case QHY_5LII:
		case QHY_5HII: {
			gain = 1.0348 + context->gain * 38.7652 / 100;
			uint16_t reg30B0;
			if (context->stream_mode) {
				if (context->long_time_mode)
					reg30B0 = 0x5330;
				else
					reg30B0 = 0x1330;
			} else {
				reg30B0 = 0x5330;
			}
			uint16_t base_gain;
			double c[8] = { 10, 8, 5, 4, 2.5, 2, 1.25, 1 };
			double s[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
			uint32_t a[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
			uint32_t b[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
			double error[8];
			for (uint32_t i = 0; i < 8; i++) {
				s[i] = gain / c[i];
				a[i] = (uint32_t)s[i];
				b[i] = (s[i] - a[i]) / 0.03125;
				if (a[i] > 3)
					a[i] = 10000;
				if (a[i] == 0)
					a[i] = 10000;
				error[i] = fabs((a[i]+b[i] * 0.03125) * c[i] - gain);
			}
			double min_value = error[0];
			uint32_t min_value_position = 0;
			for (uint32_t i = 0; i < 8; i++) {
				if (min_value > error[i]) {
					min_value = error[i];
					min_value_position = i;
				}
			}
			uint32_t aa = 0, bb = 0, cc = 0;
			double dd = 0, ee = 0;
			aa = a[min_value_position];
			bb = b[min_value_position];
			if (min_value_position == 0) {
				cc = 8;
				dd = 1.25;
				rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x30B0, (reg30B0 &~0x0030) + 0x30);
				rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3EE4, 0XD308);
			} else if (min_value_position == 1) {
				cc = 8;
				dd = 1;
				rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x30B0, (reg30B0 &~0x0030) + 0x30);
				rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3EE4, 0XD208);
			} else if (min_value_position == 2) {
				cc = 4;
				dd = 1.25;
				rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x30B0, (reg30B0 &~0x0030) + 0x20);
				rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3EE4, 0XD308);
			} else if (min_value_position == 3) {
				cc = 4;
				dd = 1;
				rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x30B0, (reg30B0 &~0x0030) + 0x20);
				rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3EE4, 0XD208);
			} else if (min_value_position == 4) {
				cc = 2;
				dd = 1.25;
				rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x30B0, (reg30B0 &~0x0030) + 0x10);
				rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3EE4, 0XD308);
			} else if (min_value_position == 5) {
				cc = 2;
				dd = 1;
				rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x30B0, (reg30B0 &~0x0030) + 0x10);
				rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3EE4, 0XD208);
			} else if (min_value_position == 6) {
				cc = 1;
				dd = 1.25;
				rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x30B0, (reg30B0 &~0x0030) + 0x00);
				rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3EE4, 0XD308);
			} else if (min_value_position == 7) {
				cc = 1;
				dd = 1;
				rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x30B0, (reg30B0 &~0x0030) + 0x00);
				rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3EE4, 0XD208);
			}
			ee = fabs((aa+bb * 0.03125) * cc * dd - gain);
			base_gain = bb + aa * 32;
			if (context->is_colour) {
				rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3058, base_gain);
				rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x305a, base_gain);
				rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x305c, base_gain);
				rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3056, base_gain);
			} else {
				rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x305E, base_gain);
			}
			break;
		}
		case QHY_5RII: {
			int rgbgainb = 0, rgbgainc = 0;
			switch((int)(gain / 100.0 * 9)) {
				case 0:
					rgbgainb = 0;
					rgbgainc = 1;
					break;
				case 1:
					rgbgainb = 1;
					rgbgainc = 1;
					break;
				case 2:
					rgbgainb = 2;
					rgbgainc = 1;
					break;
				case 3:
					rgbgainb = 3;
					rgbgainc = 1;
					break;
				case 4:
					rgbgainb = 3;
					rgbgainc = 2;
					break;
				case 5:
					rgbgainb = 3;
					rgbgainc = 3;
					break;
				case 6:
					rgbgainb = 3;
					rgbgainc = 4;
					break;
				case 7:
					rgbgainb = 3;
					rgbgainc = 5;
					break;
				case 8:
					rgbgainb = 3;
					rgbgainc = 6;
					break;
				case 9:
					rgbgainb = 3;
					rgbgainc = 7;
					break;
			}
			uint32_t tmp = rgbgainc << 12 | rgbgainb << 6;
			uint16_t reg0x3058 = 63;
			reg0x3058 |= tmp;
			rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3058, reg0x3058);
			uint16_t reg0x305a = 63;
			reg0x305a |= tmp;
			rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x305a, reg0x305a);
			uint16_t reg0x3056 = 16;
			reg0x3056 |= tmp;
			rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3056, reg0x3056);
			uint16_t reg0x305c = 16;
			reg0x305c |= tmp;
			rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x305C, reg0x305c);
			break;
		}
		default:
			break;
	}
	return rc;
}

bool libqhy_5ii_init(libqhy_device_context *context) {
  libusb_device_handle *handle = context->handle;
  context->has_guider_port = true;
  context->has_cooler = false;
  int rc = 0;
  uint8_t data[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  rc = rc < 0 ? rc : libusb_control_transfer(handle, REQUEST_READ, 0xCA, 0, 0x10, data, 16, 2000);
  QHY_DEBUG(qhy_log("libusb_control_transfer [%d] -> %s", __LINE__, rc < 0 ? libusb_error_name(rc) : "OK"));
  if (rc >= 0) {
    switch (data[0]) {
      case 1:
        QHY_DEBUG(qhy_log("*** sub-type: QHY5-II"));
        context->is_colour = false;
        context->width = 1280;
        context->height = 1024;
        context->pixel_width = 5.2;
        context->pixel_height = 5.2;
        context->bits_per_pixel = 16;
        context->max_bin_hor = context->max_bin_vert = 1;
        context->usb_traffic = 30;
        context->stream_mode = true;
        break;
      case 2:
        QHY_DEBUG(qhy_log("*** sub-type: QHY5T-II"));
        context->type = QHY_5TII;
        context->is_colour = true;
        context->width = 2048;
        context->height = 1536;
        context->pixel_width = 3.2;
        context->pixel_height = 3.2;
        context->bits_per_pixel = 8;
        context->max_bin_hor = context->max_bin_vert = 1;
        context->usb_traffic = 0;
        context->stream_mode = false;
        break;
      case 5:
        QHY_DEBUG(qhy_log("*** sub-type: QHY5P-II"));
        context->type = QHY_5PII;
        context->is_colour = data[1] == 1;
        context->width = 2592;
        context->height = 1944;
        context->pixel_width = 2.2;
        context->pixel_height = 2.2;
        context->bits_per_pixel = 16;
        context->max_bin_hor = context->max_bin_vert = 1;
        context->usb_traffic = 0;
        context->stream_mode = true;
        break;
      case 6:
        QHY_DEBUG(qhy_log("*** sub-type: QHY5L-II"));
        context->type = QHY_5LII;
        context->is_colour = data[1] == 1;
        context->width = 1280;
        context->height = 960;
        context->pixel_width = 3.75;
        context->pixel_height = 3.75;
        context->bits_per_pixel = 16;
        context->max_bin_hor = context->max_bin_vert = 1;
        context->usb_traffic = 100;
        context->stream_mode = true;
        break;
      case 9:
        QHY_DEBUG(qhy_log("*** sub-type: QHY5R-II"));
        context->type = QHY_5RII;
        context->is_colour = false;
        context->width = 728;
        context->height = 512;
        context->pixel_width = 5.6;
        context->pixel_height = 5.6;
        context->bits_per_pixel = 16;
        context->max_bin_hor = context->max_bin_vert = 1;
        context->usb_traffic = 30;
        context->stream_mode = true;
        context->offset = 32;
        break;
      case 22:
        QHY_DEBUG(qhy_log("*** sub-type: QHY5H-II"));
        context->type = QHY_5HII;
        context->is_colour = false;
        context->width = 1280;
        context->height = 960;
        context->pixel_width = 3.75;
        context->pixel_height = 3.75;
        context->bits_per_pixel = 16;
        context->max_bin_hor = context->max_bin_vert = 1;
        context->usb_traffic = 30;
        context->stream_mode = true;
        break;
    }
    data[0] = data[1] = data[2] = data[3] = 0;
    rc = rc < 0 ? rc : libusb_control_transfer(handle, REQUEST_WRITE, 0xC1, 0, 0, data, 4, 2000);
    QHY_DEBUG(qhy_log("libusb_control_transfer [%d] -> %s", __LINE__, rc < 0 ? libusb_error_name(rc) : "OK"));
    rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3012, 1);
		rc = rc < 0 ? rc : init_regs(context);
		rc = rc < 0 ? rc : set_bits_per_pixel(context, 8);
		rc = rc < 0 ? rc : set_usb_traffic(context);
    rc = rc < 0 ? rc : set_usb_speed(context, 0);
		rc = rc < 0 ? rc : set_resolution(context, context->width, context->height);
    rc = rc < 0 ? rc : set_exposure_time(context, 1000);
    rc = rc < 0 ? rc : set_offset(context);
		rc = rc < 0 ? rc : set_gain(context, 100);
		rc = rc < 0 ? rc : set_usb_traffic(context);
		rc = rc < 0 ? rc : set_exposure_time(context, 1000);
  }
  return rc >= 0;
}

bool libqhy_5ii_get_temperature(libqhy_device_context *context, double *temperature) {
	unsigned short sensed = 0, calib1 = 0, calib2 = 0;
	libusb_device_handle *handle = context->handle;
	pthread_mutex_lock(&context->usb_mutex);
	int rc = libqhy_i2c_write(handle, 0x30B4, 0x0011);
	rc = rc < 0 ? rc : libqhy_i2c_read(handle, 0x30C6, &calib1);
	rc = rc < 0 ? rc : libqhy_i2c_read(handle, 0x30C8, &calib2);
	rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x30B4, 0x0000);
	rc = rc < 0 ? rc : libqhy_i2c_read(handle, 0x30B2, &sensed);
	pthread_mutex_unlock(&context->usb_mutex);
	if (rc >= 0) {
		double slope = (70.0 - 55.0)/(calib1 - calib2);
		double T0 = (slope * calib1 - 70.0);
		*temperature = slope * sensed - T0;
		QHY_DEBUG(qhy_log("*** temperature %g", *temperature));
		QHY_DEBUG(qhy_log("QHY5II check temperature -> OK"));
		return true;
	}
	QHY_DEBUG(qhy_log("QHY5II check temperature -> Failed"));
	return false;
}

bool libqhy_5ii_set_exposure_time(libqhy_device_context *context, double exposure) {
	pthread_mutex_lock(&context->usb_mutex);
	set_exposure_time(context, context->exposure_time = 1000 * exposure);
	pthread_mutex_unlock(&context->usb_mutex);
	return true;
}

bool libqhy_5ii_set_gain(libqhy_device_context *context, double gain) {
	pthread_mutex_lock(&context->usb_mutex);
	context->gain = gain;
	set_gain(context, gain);
	pthread_mutex_unlock(&context->usb_mutex);
	return true;
}

bool libqhy_5ii_start(libqhy_device_context *context) {
  unsigned char data[2] = { 0, 100 };
  libusb_device_handle *handle = context->handle;
	assert(handle != NULL);
	pthread_mutex_lock(&context->usb_mutex);
	unsigned exposure_time = (unsigned)context->exposure_time;
  int rc = libusb_control_transfer(handle, REQUEST_WRITE,  0xb3, exposure_time & 0xffff, exposure_time >> 16, data, 1, 2000);
	QHY_DEBUG(qhy_log("libusb_control_transfer [%d] -> %s", __LINE__, rc < 0 ? libusb_error_name(rc) : "OK"));
	set_gain(context, context->gain);
	pthread_mutex_unlock(&context->usb_mutex);
	return rc >= 0;
}

bool libqhy_5ii_read_pixels(libqhy_device_context *context, unsigned short *image) {
  libusb_device_handle *handle = context->handle;
  int bytes_transfered = 0;
  int retry_count = 0;
  int curent_position = 0;
	int remaining = (context->frame_bits_per_pixel == 8 ? 1 : 2) * context->frame_width * context->frame_height + 5;
  unsigned char *buffer = (unsigned char *)image;
  int rc = 0;
  while (rc >= 0 && remaining > 0) {
    rc = libusb_bulk_transfer(handle, DATA_READ_ENDPOINT, buffer + curent_position, remaining, &bytes_transfered, (int)context->exposure_time + 1500);
    QHY_DEBUG(qhy_log("libusb_bulk_transfer [%d] -> %s (%d)", __LINE__, rc < 0 ? libusb_error_name(rc) : "OK", bytes_transfered));
    if (rc < 0) {
      if (retry_count > 5) {
        QHY_DEBUG(qhy_log("QHY5II read pixels -> Failed (retry count > 5)"));
        return false;
      }
			rc = libusb_clear_halt(handle, DATA_READ_ENDPOINT);
			QHY_DEBUG(qhy_log("libusb_clear_halt [%d] -> %s", __LINE__, rc < 0 ? libusb_error_name(rc) : "OK"));
      retry_count++;
      continue;
    }
    curent_position += bytes_transfered;
    remaining -= bytes_transfered;
    unsigned char sentinel[4] = { 0xAA, 0x11, 0xCC, 0xEE };
    void *sentinel_found = memmem(buffer + curent_position - 5, 4, sentinel, 4);
    if (remaining && sentinel_found) {
      curent_position = 0;
      remaining = context->frame_width * context->frame_height + 5;
      QHY_DEBUG(qhy_log("QHY5II read pixels - misaligned frame, skipping"));
      continue;
    }
    if (remaining <= 0 && sentinel_found == NULL) {
      if (retry_count > 3) {
        QHY_DEBUG(qhy_log("QHY5II read pixels -> Failed (retry count > 3)"));
        return false;
      }
      curent_position = 0;
      remaining = context->frame_width * context->frame_height + 5;
      retry_count++;
      continue;
    }
  }
  return rc >= 0;
}

bool libqhy_5ii_stop(libqhy_device_context *context) {
	unsigned char data[4] = { 0, 0, 0, 0 };
	libusb_device_handle *handle = context->handle;
	pthread_mutex_lock(&context->usb_mutex);
	int rc = libusb_control_transfer(handle, REQUEST_WRITE, 0xC1, 0, 0, data, 4, 2000);
	pthread_mutex_unlock(&context->usb_mutex);
	QHY_DEBUG(qhy_log("libusb_control_transfer [%d] -> %s", __LINE__, rc < 0 ? libusb_error_name(rc) : "OK"));
	return rc >= 0;
}
