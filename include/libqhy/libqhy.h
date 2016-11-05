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

#ifndef libqhy_h
#define libqhy_h

#include <stdbool.h>
#include <pthread.h>

#include <libusb-1.0/libusb.h>

#define QHY_VID1	0x16C0
#define QHY_VID2	0x1618

typedef enum {
  QHY_UNINITIALIZED = 0,
  QHY_5II,
  QHY_5TII,
  QHY_5PII,
  QHY_5LII,
  QHY_5RII,
  QHY_5HII
} libqhy_camera_type;

typedef struct libqhy_device_context libqhy_device_context;

extern bool libqhy_camera(libusb_device *device, libqhy_camera_type *type, const char **name, bool *is_guider);

extern bool libqhy_open(libusb_device *device, libqhy_device_context **context);
extern bool libqhy_reset(libqhy_device_context *context);

extern bool libqhy_get_handle(libqhy_device_context *context, libusb_device_handle **handle);
extern bool libqhy_get_resolution(libqhy_device_context *context, unsigned *width, unsigned *height, unsigned *bits_per_pixel);
extern bool libqhy_get_pixel_size(libqhy_device_context *context, double *width, double *height);
extern bool libqhy_get_temperature(libqhy_device_context *context, double *temperature);

extern bool libqhy_set_exposure_time(libqhy_device_context *context, double exposure);
extern bool libqhy_set_gain(libqhy_device_context *context, double gain);

extern bool libqhy_start(libqhy_device_context *context);
extern bool libqhy_read_pixels(libqhy_device_context *context, unsigned short *image);
extern bool libqhy_stop(libqhy_device_context *context);

extern bool libqhy_set_guide_relays(libqhy_device_context *device_context, unsigned short mask);

extern void libqhy_close(libqhy_device_context *context);

extern bool libqhy_debug_level;
extern bool libqhy_use_syslog;

extern const char *libqhy_version;
extern const char *libqhy_os;
extern const char *libqhy_arch;

#endif /* libqhy_h */
