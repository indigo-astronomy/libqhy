/*
 * libqhy - interface library for QHY cameras
 *
 * Copyright (c) 2016 CloudMakers, s. r. o. All Rights Reserved.
 *
 * Redistribution and use in binary form is permitted provided that
 * the above copyright notice and this paragraph are duplicated in all
 * such forms and that any documentation, advertising materials, and
 * other materials related to such distribution and use acknowledge that
 * the software was developed by the  CloudMakers, s. r. o. The name of
 * the CloudMakers, s. r. o. may not be used to endorse or promote products
 * derived from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED 'AS IS' AND WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <libusb-1.0/libusb.h>
#include <libqhy/libqhy.h>

unsigned short pixels[2 * 16 * 1024 * 1024] = { 0, 0, 0, 0, 0 };
int width, height;

static void *test(libusb_device *dev) {
  const char *name;
  libqhy_camera_type type;
  if (libqhy_camera(dev, &type, &name, NULL)) {
    printf("\n%s connected...\n\n", name);
    libqhy_device_context *context;
    if (libqhy_open(dev, &context)) {
      printf("\nShort exposure test...\n\n");
      libqhy_start_exposure(context, 10);
      libqhy_read_pixels(context, pixels);
      printf("\n %04x %04x %04x %04x\n", pixels[0], pixels[1], pixels[2], pixels[3]);
      if (context->has_cooler) {
//        printf("\nCooler test...\n\n");
//        libqhy_set_cooler(context, true, -10);
//        sleep(1);
//        bool state;
//        double temperature, power;
//        libqhy_check_cooler(context, &state, &power, &temperature);
//        sleep(1);
//        libqhy_check_cooler(context, &state, &power, &temperature);
//        sleep(1);
//        libqhy_set_cooler(context, false, 0);
      } else {
        printf("\nTemperature sensor test...\n\n");
        double temperature;
        libqhy_check_temperature(context, &temperature);        
      }
//      if (context->has_guider_port) {
//        printf("\nGuide relay test...\n\n");
//        libqhy_guide_relays(context, QHY_GUIDE_EAST);
//        sleep(1);
//        libqhy_guide_relays(context, QHY_GUIDE_WEST);
//        sleep(1);
//        libqhy_guide_relays(context, QHY_GUIDE_NORTH);
//        sleep(1);
//        libqhy_guide_relays(context, QHY_GUIDE_SOUTH);
//        sleep(1);
//        libqhy_guide_relays(context, 0);
//      }
      printf("\nTest OK\n\n");
      libqhy_close(context);
    }
  }
  libusb_unref_device(dev);
  printf("\nWaiting for next device, press CTRL+C if done\n\n");
  return NULL;
}

static int hotplug_callback(libusb_context *ctx, libusb_device *dev, libusb_hotplug_event event, void *user_data) {
  pthread_t hotplug_thread;
  libusb_ref_device(dev);
  pthread_create(&hotplug_thread, NULL, (void *)(void *)test, dev);
  return 0;
};

int main(int argc, char **argv) {
  printf("libqhy_test %s %s %s\n\n", libqhy_version, libqhy_os, libqhy_arch);
  libqhy_debug_level = true;
  libusb_init(NULL);
  int rc = libusb_hotplug_register_callback(NULL, LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED, LIBUSB_HOTPLUG_ENUMERATE, QHY_VID1, LIBUSB_HOTPLUG_MATCH_ANY, LIBUSB_HOTPLUG_MATCH_ANY, hotplug_callback, NULL, NULL);
  if (rc >= 0)
    rc = libusb_hotplug_register_callback(NULL, LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED, LIBUSB_HOTPLUG_ENUMERATE, QHY_VID2, LIBUSB_HOTPLUG_MATCH_ANY, LIBUSB_HOTPLUG_MATCH_ANY, hotplug_callback, NULL, NULL);
  if (rc >= 0) {
    while (true)
      libusb_handle_events(NULL);
  }
}
