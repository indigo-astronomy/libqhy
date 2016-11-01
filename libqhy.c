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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <syslog.h>

#include <libusb-1.0/libusb.h>
#include <libqhy/libqhy.h>

#include <libqhy/libqhy.h>

#include "config.h"

#include "libqhy_base.h"
#include "libqhy_5ii.h"

static struct {
  const int vid, pid;
  const libqhy_camera_type type;
  const unsigned char *firmware;
  const char *name;
  const bool is_guider;
} qhy_cameras[] = {
  { 0x1618, 0x0920, QHY_UNINITIALIZED, libqhy_5ii_firmware, "Uninitialized QHY5-II", false },
  { 0x1618, 0x0921, QHY_5II, NULL, "QHY5-II", true },
  { 0, 0, 0, NULL, NULL, false }
};

bool libqhy_debug_level = true;
bool libqhy_use_syslog = false;

const char *libqhy_version =  LIBQHY_VERSION;
const char *libqhy_os =       LIBQHY_OS;
const char *libqhy_arch =     LIBQHY_ARCH;

void qhy_log(const char *format, ...) {
  if (!libqhy_debug_level)
    return;
  static pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;
  static char buffer[1024];
  va_list argList;
  va_start(argList, format);
  pthread_mutex_lock(&log_mutex);
  vsnprintf(buffer, sizeof(buffer), format, argList);
  char *line = buffer;
  if (libqhy_use_syslog) {
    while (line) {
      char *eol = strchr(line, '\n');
      if (eol)
        *eol = 0;
      if (eol > line)
        syslog (LOG_NOTICE, "%s", buffer);
      syslog (LOG_NOTICE, "%s\n", line);
      if (eol)
        line = eol + 1;
      else
        line = NULL;
    }
  } else {
    char timestamp[16];
    struct timeval tmnow;
    gettimeofday(&tmnow, NULL);
    strftime (timestamp, 9, "%H:%M:%S", localtime(&tmnow.tv_sec));
#ifdef __APPLE__
    snprintf(timestamp + 8, sizeof(timestamp) - 8, ".%06d", tmnow.tv_usec);
#else
    snprintf(timestamp + 8, sizeof(timestamp) - 8, ".%06ld", tmnow.tv_usec);
#endif
    while (line) {
      char *eol = strchr(line, '\n');
      if (eol)
        *eol = 0;
      if (*line)
        fprintf(stderr, "%s libqhy: %s\n", timestamp, line);
      if (eol)
        line = eol + 1;
      else
        line = NULL;
    }
  }
  pthread_mutex_unlock(&log_mutex);
}

//static bool qhy_control_write(libusb_device_handle *handle, unsigned req, unsigned char* data, unsigned length) {
//  int rc = libusb_control_transfer(handle, QHYCCD_REQUEST_WRITE, req, 0, 0, data, length, 0);
//  QHY_DEBUG(qhy_log("libusb_control_transfer -> %s\n", rc < 0 ? libusb_error_name(rc) : "OK" ));
//  return rc >= 0;
//}
//
//static bool qhy_control_read(libusb_device_handle *handle, unsigned req, unsigned char* data, unsigned length) {
//  int rc = libusb_control_transfer(handle, QHYCCD_REQUEST_READ, req, 0, 0, data, length, 0);
//  QHY_DEBUG(qhy_log("libusb_control_transfer -> %s\n", rc < 0 ? libusb_error_name(rc) : "OK" ));
//  return rc >= 0;
//}
//
//static bool qhy_write(libusb_device_handle *handle, unsigned char *data, unsigned length) {
//  int length_transfered;
//  int rc = libusb_bulk_transfer(handle, QHYCCD_INTERRUPT_WRITE_ENDPOINT, data, length, &length_transfered, 0);
//  QHY_DEBUG(qhy_log("libusb_bulk_transfer -> %s\n", rc < 0 ? libusb_error_name(rc) : "OK" ));
//  return rc >= 0;
//}
//
//static bool qhy_read(libusb_device_handle *handle, unsigned char *data, unsigned length) {
//  int length_transfered;
//  int rc = libusb_bulk_transfer(handle, QHYCCD_INTERRUPT_READ_ENDPOINT, data, length, &length_transfered, 0);
//  QHY_DEBUG(qhy_log("libusb_bulk_transfer -> %s\n", rc < 0 ? libusb_error_name(rc) : "OK" ));
//  return rc >= 0;
//}

int libqhy_i2c_write(libusb_device_handle *handle, unsigned addr,unsigned short value) {
  unsigned char data[2];
  data[0] = (value & 0xff00) >> 8;
  data[1] = value & 0x00FF;
  int rc = libusb_control_transfer(handle, REQUEST_WRITE, 0xbb, 0, addr, data, 2, 0);
  QHY_DEBUG(qhy_log("libusb_control_transfer -> %s\n", rc < 0 ? libusb_error_name(rc) : "OK" ));
  return rc;
}

int libqhy_i2c_read(libusb_device_handle *handle, unsigned addr, unsigned short *value) {
  unsigned char data[2];
  int rc = libusb_control_transfer(handle, REQUEST_READ, 0xb7, 0, addr, data, 2, 0);
  QHY_DEBUG(qhy_log("libusb_control_transfer -> %s\n", rc < 0 ? libusb_error_name(rc) : "OK" ));
  if (rc >= 0)
    *value = data[0] * 256 + data[1];
  return rc;
}

static void qhy_firmware(libusb_device *device, const unsigned char *data) {
  unsigned char stop = 1;
  unsigned char reset = 0;
  libusb_device_handle *handle;
  int rc = libusb_open(device, &handle);
  QHY_DEBUG(qhy_log("libusb_open [%d] -> %s", __LINE__, rc < 0 ? libusb_error_name(rc) : "OK"));
  if (libusb_has_capability(LIBUSB_CAP_SUPPORTS_DETACH_KERNEL_DRIVER)) {
    if (libusb_kernel_driver_active(handle, 0) == 1) {
      rc = libusb_detach_kernel_driver(handle, 0);
      QHY_DEBUG(qhy_log("libusb_detach_kernel_driver [%d] -> %s", __LINE__, rc < 0 ? libusb_error_name(rc) : "OK"));
    }
  }
  if (rc >= 0) {
    rc = libusb_claim_interface(handle, 0);
    QHY_DEBUG(qhy_log("libusb_claim_interface [%d] -> %s", __LINE__, rc < 0 ? libusb_error_name(rc) : "OK"));
  }
  if (rc >= 0) {
    rc = rc < 0 ? rc : libusb_control_transfer(handle, REQUEST_WRITE, 0xA0, 0xe600, 0, &stop, 1, 3000);
    QHY_DEBUG(qhy_log("libusb_control_transfer [%d] -> %s", __LINE__, rc < 0 ? libusb_error_name(rc) : "OK"));
    while (true) {
      unsigned char byte_count = data[0];
      if (byte_count == 0)
        break;
      unsigned short address = ((data[1] & 0xFF) << 8) | (data[2] & 0xFF);
      rc = libusb_control_transfer(handle, REQUEST_WRITE, 0xA0, address, 0, (unsigned char *)(data+3), byte_count, 3000);
      if (rc != byte_count) {
        QHY_DEBUG(qhy_log("libusb_control_transfer [%d] -> %s", __LINE__, rc < 0 ? libusb_error_name(rc) : "OK"));
        break;
      }
      data += byte_count + 3;
    }
    rc = rc < 0 ? rc : libusb_control_transfer(handle, REQUEST_WRITE, 0xA0, 0xe600, 0, &reset, 1, 3000);
    QHY_DEBUG(qhy_log("libusb_control_transfer [%d] -> %s", __LINE__, rc < 0 ? libusb_error_name(rc) : "OK"));
    rc = libusb_release_interface(handle, 0);
    QHY_DEBUG(qhy_log("libusb_release_interface [%d] -> %s", __LINE__, rc < 0 ? libusb_error_name(rc) : "OK"));
    libusb_close(handle);
    QHY_DEBUG(qhy_log("libusb_close [%d]", __LINE__));
  }
}

bool libqhy_camera(libusb_device *device, libqhy_camera_type *type, const char **name, bool *is_guider) {
  struct libusb_device_descriptor descriptor;
  int rc = libusb_get_device_descriptor(device, &descriptor);
  QHY_DEBUG(qhy_log("libusb_get_device_descriptor [%d] ->  %s", __LINE__, rc < 0 ? libusb_error_name(rc) : "OK"));
  QHY_DEBUG(qhy_log("*** vid:  0x%04x", descriptor.idVendor));
  QHY_DEBUG(qhy_log("*** pid:  0x%04x", descriptor.idProduct));
  *type = 0;
  for (int i = 0; qhy_cameras[i].vid != 0; i++) {
    if (descriptor.idVendor == qhy_cameras[i].vid && qhy_cameras[i].pid == descriptor.idProduct) {
      if (qhy_cameras[i].type == QHY_UNINITIALIZED) {
        QHY_DEBUG(qhy_log("*** type: %s", qhy_cameras[i].name));
        qhy_firmware(device, qhy_cameras[i].firmware);
        return false;
      } else {
        if (type)
          *type = qhy_cameras[i].type;
        if (name)
          *name = qhy_cameras[i].name;
        if (is_guider)
          *is_guider = qhy_cameras[i].is_guider;
        QHY_DEBUG(qhy_log("*** type: %s", qhy_cameras[i].name));
        return true;
      }
    }
  }
  return false;
}

bool libqhy_open(libusb_device *device, libqhy_device_context **device_context) {
  libqhy_camera_type type;
  libqhy_camera(device, &type, NULL, NULL);
  libusb_device_handle *handle = NULL;
  int rc = libusb_open(device, &handle);
  QHY_DEBUG(qhy_log("libusb_open [%d] ->  %s", __LINE__, rc < 0 ? libusb_error_name(rc) : "OK"));
  if (rc >= 0) {
    if (libusb_has_capability(LIBUSB_CAP_SUPPORTS_DETACH_KERNEL_DRIVER)) {
      if (libusb_kernel_driver_active(handle, 0) == 1) {
        rc = libusb_detach_kernel_driver(handle, 0);
        QHY_DEBUG(qhy_log("libusb_detach_kernel_driver [%d] ->  %s", __LINE__, rc < 0 ? libusb_error_name(rc) : "OK"));
      }
    }
  }
  if (rc >= 0) {
    rc = libusb_set_configuration(handle, 1);
    QHY_DEBUG(qhy_log("libusb_set_configuration [%d] ->  %s", __LINE__, rc < 0 ? libusb_error_name(rc) : "OK"));
  }
  if (rc >= 0) {
    rc = libusb_claim_interface(handle, 0);
    QHY_DEBUG(qhy_log("libusb_claim_interface [%d] ->  %s", __LINE__, rc < 0 ? libusb_error_name(rc) : "OK"));
  }
  if (rc >= 0) {
    libqhy_device_context *context = malloc(sizeof(libqhy_device_context));
    memset(context, 0, sizeof(libqhy_device_context));
    context->handle = handle;
    context->type = type;
    pthread_mutex_init(&context->lock, NULL);
    bool initialized = false;
    switch (type) {
      case  QHY_5II:
        initialized = libqhy_5ii_init(context);
      default:
        break;
      }
      if (initialized) {
        QHY_DEBUG(qhy_log("*** pixel count: %d x %d", context->width, context->height));
        QHY_DEBUG(qhy_log("*** pixel size: %3.2f x %3.2f", context->pixel_width, context->pixel_height));
        QHY_DEBUG(qhy_log("*** max bin: %d x %d", context->max_bin_hor, context->max_bin_vert));
        QHY_DEBUG(qhy_log("*** has guider port: %s", context->has_guider_port ? "true" : "false"));
        QHY_DEBUG(qhy_log("*** has cooler: %s", context->has_cooler ? "true" : "false"));
        QHY_DEBUG(qhy_log("*** is colour: %s", context->is_colour ? "true" : "false"));
        *device_context = context;
        QHY_DEBUG(qhy_log("QHY open -> OK"));
      } else {
        free (context);
      }
    }
    return rc >= 0;
  }

bool libqhy_check_temperature(libqhy_device_context *context, double *temperature) {
  switch (context->type) {
    case QHY_5II:
    case QHY_5TII:
    case  QHY_5PII:
    case  QHY_5LII:
    case  QHY_5RII:
    case  QHY_5HII:
      return libqhy_5ii_check_temperature(context, temperature);
    default:
      break;
  }
  QHY_DEBUG(qhy_log("QHY check temperature -> Failed"));
  return false;
}

bool libqhy_start_exposure(libqhy_device_context *context, double exposure) {
  switch (context->type) {
    case QHY_5II:
    case QHY_5TII:
    case  QHY_5PII:
    case  QHY_5LII:
    case  QHY_5RII:
    case  QHY_5HII:
      return libqhy_5ii_start_exposure(context, exposure);
    default:
      break;
  }
  QHY_DEBUG(qhy_log("QHY start exposure -> Failed"));
  return false;
}

bool libqhy_read_pixels(libqhy_device_context *context, unsigned short *image) {
  switch (context->type) {
    case QHY_5II:
    case QHY_5TII:
    case  QHY_5PII:
    case  QHY_5LII:
    case  QHY_5RII:
    case  QHY_5HII:
      return libqhy_5ii_read_pixels(context, image);
    default:
      break;
  }
  QHY_DEBUG(qhy_log("QHY read pixels -> Failed"));
  return false;
}

void libqhy_close(libqhy_device_context *device_context) {
  libusb_close(device_context->handle);
  free(device_context);
  QHY_DEBUG(qhy_log("QHY close"));
}
