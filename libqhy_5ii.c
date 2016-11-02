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

#include <libqhy/libqhy.h>

#include "libqhy_base.h"
#include "libqhy_5ii.h"

extern unsigned char libqhy_5ii_firmware[];

static int set_resolution(libqhy_device_context *context);
static int set_exposure_time(libqhy_device_context *context, double time);
static int set_usb_speed(libqhy_device_context *context);
static int set_usb_traffic(libqhy_device_context *context);

static int init_cmos(libqhy_device_context *context) {
  libusb_device_handle *handle = context->handle;
  int rc = 0;
  switch (context->type) {
    case QHY_5II:
      break;
    case QHY_5TII:
      break;
    case QHY_5PII:
      break;
    case QHY_5LII:
      rc = rc < 0 ? rc : set_exposure_time(context, 1000);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x30D4, 0xe00f);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x301A, 0x0001);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x301A, 0x10D8);
      usleep(200000);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3088, 0x8000);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0025);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x5050);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2D26);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0828);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0D17);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0926);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0028);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0526);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0xA728);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0725);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x8080);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2925);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0040);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2702);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1616);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2706);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1F17);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x3626);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0xA617);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0326);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0xA417);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1F28);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0526);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2028);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0425);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2020);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2700);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x171D);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2500);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2017);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1028);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0519);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1703);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2706);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1703);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1741);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2660);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x175A);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2317);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1122);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1741);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2500);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x9027);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0026);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1828);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x002E);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2A28);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x081C);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1470);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x7003);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1470);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x7004);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1470);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x7005);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1470);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x7009);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x170C);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0014);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0020);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0014);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0050);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0314);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0020);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0314);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0050);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0414);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0020);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0414);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0050);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0514);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0020);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2405);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1400);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x5001);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2550);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x502D);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2608);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x280D);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1709);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2600);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2805);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x26A7);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2807);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2580);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x8029);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2500);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x4027);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0216);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1627);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0620);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1736);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x26A6);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1703);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x26A4);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x171F);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2805);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2620);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2804);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2520);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2027);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0017);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1D25);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0020);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1710);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2805);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1A17);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0327);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0617);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0317);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x4126);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x6017);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0xAE25);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0090);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2700);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2618);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2800);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2E2A);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2808);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1D05);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1470);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x7009);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1720);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1400);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2024);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1400);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x5002);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2550);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x502D);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2608);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x280D);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1709);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2600);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2805);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x26A7);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2807);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2580);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x8029);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2500);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x4027);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0216);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1627);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0617);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x3626);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0xA617);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0326);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0xA417);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1F28);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0526);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2028);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0425);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2020);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2700);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x171D);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2500);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2021);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1710);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2805);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1B17);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0327);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0617);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0317);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x4126);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x6017);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0xAE25);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0090);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2700);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2618);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2800);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2E2A);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2808);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1E17);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0A05);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1470);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x7009);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1616);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1616);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1616);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1616);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1616);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1616);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1616);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1616);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1616);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1616);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1616);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1616);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1616);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1616);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1616);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1616);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1400);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2024);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1400);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x502B);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x302C);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2C2C);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2C00);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0225);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x5050);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2D26);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0828);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0D17);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0926);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0028);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0526);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0xA728);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0725);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x8080);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2917);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0525);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0040);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2702);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1616);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2706);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1736);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x26A6);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1703);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x26A4);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x171F);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2805);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2620);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2804);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2520);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2027);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0017);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1E25);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0020);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2117);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1028);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x051B);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1703);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2706);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1703);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1747);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2660);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x17AE);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2500);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x9027);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0026);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1828);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x002E);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2A28);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x081E);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0831);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1440);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x4014);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2020);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1410);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1034);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1400);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1014);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0020);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1400);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x4013);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1802);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1470);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x7004);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1470);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x7003);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1470);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x7017);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2002);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1400);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2002);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1400);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x5004);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1400);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2004);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x1400);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x5022);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0314);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0020);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0314);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x0050);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2C2C);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3086, 0x2C2C);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x309E, 0x018A);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x301A, 0x10D8);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3082, 0x0029);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x301E, 0x00C8);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3EDA, 0x0F03);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3EDE, 0xC007);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3ED8, 0x01EF);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3EE2, 0xA46B);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3EE0, 0x067D);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3EDC, 0x0070);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3044, 0x0404);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3EE6, 0x4303);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3EE4, 0xD208);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3ED6, 0x00BD);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3EE6, 0x8303);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x30E4, 0x6372);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x30E2, 0x7253);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x30E0, 0x5470);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x30E6, 0xC4CC);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x30E8, 0x8050);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x30BA, 0x000a);
      usleep(100000);
      rc = rc < 0 ? rc : set_exposure_time(context, context->exposure_time);
      break;
    case QHY_5RII:
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x001A,0x0026);
      usleep(20000);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x0040,0x8101);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0xc972,0x0005);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x098e,0x4802);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0xc802,0x0000);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0xc800,0x0000);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0xc806,0x02d7);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0xc804,0x01ff);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0xc80c,0x00a4);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0xc80e,0x01b2);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0xc810,0x0355);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0xc812,0x0212);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0xc814,0x0031);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0xc816,0x0202);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0xc806,0x02d7);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0xc86c,0x0000);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0xc882,0x3c00);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0xc884,0x3c00);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0xc968,0x02d8);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0xc96a,0x0200);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0xc96c,0x0200);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x098e,0x4960);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0xc960,0x0000);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0xc8ff,0x0000);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0xa804,0x0000);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0xfc00,0x0000);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x0040,0x8606);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0xc864,0x0001);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0xc858,0x0011);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0xc972,0x0025);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0xc972,0x0025);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0xc858,0x0011);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0xc83d,0x0002);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0xc83a,0x0020);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0xc83c,0x0000);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x9427,0x0000);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x9427,0x0000);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x9427,0x0000);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0xc850,0x0000);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x001e,0x0302);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0xfc00,0x2800);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x0040,0x8100);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x0040,0x8101);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3056,0x101f);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x305c,0x101f);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x305a,0x101f);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3058,0x103b);
      usleep(20000);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x305a,0x101f);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3058,0x103b);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x301e,0x0020);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x300c,0x01d5);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3012,0x0033);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3040,0x0041);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3015,0x0000);
      break;
    case QHY_5HII:
      break;
    default:
      break;
  }
  return rc;
}

static int set_ppl(libqhy_device_context *context) {
	libusb_device_handle *handle = context->handle;
	int rc = 0;
	switch (context->type) {
		case QHY_5LII:
			// set pll
			rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x302A, 14);
			rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x302C, 1);
			rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x302E, 3);
			rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3030, 42);
			rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3082, 0x0029);
			if (context->stream_mode) {
				if (context->long_time_mode)
					rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x30B0, 0x5330);
				else
					rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x30B0, 0x1330);
			} else {
				rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x30B0, 0x5330);
			}
			rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x305e, 0x0020);
			rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3EE4, 0xD208);
			rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3064, 0x1802);
			break;
		case QHY_5HII:
			// set pll
			rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x302A, 14);
			rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x302C, 1);
			rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x302E, 3);
			rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3030, 42);
			rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3082, 0x0029);
			rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x30B0, 0x0020);
			rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x305e, 0x0020);
			rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3EE4, 0xD208);
			rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3064, 0x1802);
			break;
		default:
			break;
	}
	return rc;
}

static int set_resolution(libqhy_device_context *context) {
  libusb_device_handle *handle = context->handle;
  int rc = 0;
  switch (context->type) {
    case QHY_5II:
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x09, 200);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x01, 8); // y start
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x02, 16); // x start
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x03, 1024 - 1); // y size
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x04, 1280 - 1); // x size
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x22, 0x00); // normal bin
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x23, 0x00); // normal bin
      break;
    case QHY_5TII:
      break;
    case QHY_5PII:
      break;
    case QHY_5LII:
			rc = rc < 0 ? rc : set_ppl(context);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3002, 4); // y start
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3004, 4); // x start
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3006, 4 + 960 - 1); // y end
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3008, 4 + 1280 - 1); // x end
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x300a, 506); // frame length
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x300c, 1388); // line  length
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x301A, 0x10DC); // RESET_REGISTER
      break;
    case QHY_5RII:
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x301A,0x10d0);
      usleep(1000);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3002, 0x0000);  //0
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3004, 0x0000);  //0
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x300A, 0x021d);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3006, 0x01ff);  //512-1
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3008, 0x02d7);  //728-1
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x300c, 0x01d5);
      rc = rc < 0 ? rc : libqhy_i2c_read(handle, 0x300c, &context->reg300c);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3012, 0x0033);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3012, 0x0033);
      usleep(4000);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x301A,0x10d0);
      usleep(1000);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x301A,0x10d4);
      break;
    case QHY_5HII:
			rc = rc < 0 ? rc : set_ppl(context);
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3002, 4); // y start
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3004, 4); // x start
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3006, 4 + 960 - 1); // y end
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x3008, 4 + 1280 - 1); // x end
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x300a, 990); // frame length
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x300c, 1650); // line  length
      rc = rc < 0 ? rc : libqhy_i2c_write(handle, 0x301A, 0x10DC); // RESET_REGISTER
      break;
    default:
      break;
  }
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

static int set_usb_speed(libqhy_device_context *context) {
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
          data[0] = context->usb_speed;
        else {
          if(context->usb_speed >= 1)
            data[0] = 1;
          else
            data[0] = 0;
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
        if(context->usb_speed <= 1)
          data[0] = 1;
        else
          data[0] = 2;
      } else {
        if(context->usb_speed >= 1)
          data[0] = 1;
        else
          data[0] = 0;
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

//static int template(libqhy_device_context *context) {
//  libusb_device_handle *handle = context->handle;
//  int rc = 0;
//  switch (context->type) {
//    case QHY_5II:
//      break;
//    case QHY_5TII:
//      break;
//    case QHY_5PII:
//      break;
//    case QHY_5LII:
//      break;
//    case QHY_5RII:
//      break;
//    case QHY_5HII:
//      break;
//    default:
//      break;
//  }
//  return rc;
//}

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
        context->usb_traffic = 30;
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
    context->usb_speed = 0;
    context->frame_top = context->frame_left = 0;
    context->frame_width = context->width;
    context->frame_height = context->height;
		context->frame_bits_per_pixel = 8; //context->bits_per_pixel;
    rc = rc < 0 ? rc : set_usb_speed(context);
    rc = rc < 0 ? rc : init_cmos(context);
    rc = rc < 0 ? rc : set_resolution(context);
    rc = rc < 0 ? rc : set_usb_traffic(context);
    rc = rc < 0 ? rc : set_exposure_time(context, context->exposure_time = 20000);
    rc = rc < 0 ? rc : set_offset(context);
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
	set_exposure_time(context, context->exposure_time = exposure);
	pthread_mutex_unlock(&context->usb_mutex);
	return true;
}

bool libqhy_5ii_start(libqhy_device_context *context) {
  unsigned char data[1] = { 100 };
  libusb_device_handle *handle = context->handle;
	assert(handle != NULL);
	pthread_mutex_lock(&context->usb_mutex);
  int rc = libusb_control_transfer(handle, REQUEST_WRITE,  0xb3, 0, 0, data, 1, 2000);
	QHY_DEBUG(qhy_log("libusb_control_transfer [%d] -> %s", __LINE__, rc < 0 ? libusb_error_name(rc) : "OK"));
	pthread_mutex_unlock(&context->usb_mutex);
	return rc >= 0;
}

bool libqhy_5ii_read_pixels(libqhy_device_context *context, unsigned short *image) {
  libusb_device_handle *handle = context->handle;
  int bytes_transfered = 0;
  int retry_count = 0;
  int curent_position = 0;
  int remaining = context->frame_width * context->frame_height + 5;
  unsigned char *buffer = (unsigned char *)image;
  int rc = 0;
  while (rc >= 0 && remaining > 0) {
    rc = libusb_bulk_transfer(handle, DATA_READ_ENDPOINT, buffer + curent_position, remaining, &bytes_transfered, (int)context->exposure_time + 1500);
    QHY_DEBUG(qhy_log("libusb_bulk_transfer [%d] -> %s", __LINE__, rc < 0 ? libusb_error_name(rc) : "OK"));
    if (rc < 0) {
      if (retry_count > 3) {
        QHY_DEBUG(qhy_log("QHY5II read pixels -> Failed (retry count > 3)"));
        return false;
      }
      retry_count++;
      rc = 0;
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
