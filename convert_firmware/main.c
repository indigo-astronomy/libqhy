//
//  main.c
//  convert_firmware
//
//  Created by Peter Polakovic on 31/10/2016.
//  Copyright © 2016 CloudMakers, s. r. o. All rights reserved.
//

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, const char * argv[]) {
  FILE *image;
  if ((image = fopen(argv[2], "r"))) {
    
    fprintf(stderr, "#ifndef __%s_FIRMWARE_H_\n", argv[1]);
    fprintf(stderr, "#define __%s_FIRMWARE_H_\n", argv[1]);
    fprintf(stderr, "\n");
    fprintf(stderr, "/*\n");
    fprintf(stderr, " * Firmware is in the following format:\n");
    fprintf(stderr, " *\n");
    fprintf(stderr, " * byte 0: Byte Count (Number of bytes in payload)\n");
    fprintf(stderr, " * byte 1-2: Address where data will go (1 word)\n");
    fprintf(stderr, " * byte 3-x: Data\n");
    fprintf(stderr, " */\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "#define %s_FIRMWARE \\\n", argv[1]);
    for (;;) {
      char buf[512], *cp;
      cp = fgets(buf, sizeof buf, image);
      if (cp == 0) {
        printf("EOF without EOF record\n");
        break;
      }
      if (buf[0] == '#')
        continue;
      if (buf[0] != ':') {
        printf("Invalid ihex record\n");
        break;
      }
      cp = strchr(buf, '\n');
      if (cp)
        *cp = 0;
      cp = strchr(buf, '\r');
      if (cp)
        *cp = 0;
      if (buf[8] == '1')
        break;
      buf[strlen(buf)-2] = 0;
      fprintf(stderr, "0x%c%c, 0x%c%c, 0x%c%c", buf[1], buf[2], buf[3], buf[4], buf[5], buf[6]);
      for (cp = buf + 9; *cp; cp += 2)
        fprintf(stderr, ", 0x%c%c", cp[0], cp[1]);
      fprintf(stderr, ",\\\n");
    }
    fprintf(stderr, ", 0x00");
    fprintf(stderr, "#endif\n");
    fclose(image);
  } else {
    printf("Can't open %s\n", argv[1]);
    return 0;
  }
}
