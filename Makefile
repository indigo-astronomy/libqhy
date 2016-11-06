# Copyright (c) 2016 CloudMakers, s. r. o.
# All rights reserved.
#
# You can use this software under the terms of 'INDIGO Astronomy
# open-source license' (see LICENSE.md).
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHORS 'AS IS' AND ANY EXPRESS
# OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
# GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

ifeq ($(OS),Windows_NT)
  OS_DETECTED=Windows
else
	OS_DETECTED=$(shell uname -s)
	ARCH_DETECTED=$(shell uname -m)
	ifeq ($(ARCH_DETECTED),armv7l)
		ARCH_DETECTED=arm
	endif
	ifeq ($(ARCH_DETECTED),i686)
		ARCH_DETECTED=x86
	endif
	ifeq ($(ARCH_DETECTED),x86_64)
		ARCH_DETECTED=x64
	endif
endif

LIBQHY_VERSION=1.0

ifeq ($(OS_DETECTED),Darwin)
	LIBQHY=lib/macOS/libqhy.a
endif
ifeq ($(OS_DETECTED),Linux)
	LIBQHY=lib/Linux/$(ARCH_DETECTED)/libqhy.a
endif

ifeq ($(OS_DETECTED),Darwin)
	CC=gcc
	CFLAGS=-fPIC -O3 -Iinclude -std=gnu11 -mmacosx-version-min=10.9
	LDFLAGS=-framework CoreFoundation -framework IOKit -Llib/macOS lib/macOS/libusb-1.0.a 
	AR=ar
	ARFLAGS=-rv
endif

ifeq ($(OS_DETECTED),Linux)
	CC=gcc
	ifeq ($(ARCH_DETECTED),ARM)
		CFLAGS=-fPIC -O3 -march=armv6 -mfpu=vfp -mfloat-abi=hard -std=gnu11 -pthread
  else
    CFLAGS=-fPIC -O3 -Iinclude -std=gnu11 -pthread
	endif
	LDFLAGS=-lm -lrt -lusb-1.0
	AR=ar
	ARFLAGS=-rv
endif

.PHONY: init clean remote

all: init $(LIBQHY) libqhy_test

init:
	$(info -------------------- $(OS_DETECTED) $(ARCH_DETECTED) build --------------------)
	printf "#define LIBQHY_VERSION \"$(LIBQHY_VERSION)\"\n#define LIBQHY_OS \"$(OS_DETECTED)\"\n#define LIBQHY_ARCH \"$(ARCH_DETECTED)\"\n" >config.h
	install -d $(dir $(LIBQHY))

$(LIBQHY):	libqhy.o libqhy_5ii.o libqhy_5ii_data.o
	$(AR) $(ARFLAGS) $@ $^

libqhy_test: libqhy_test.o $(LIBQHY)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

remote:
#	hg commit -m "$(LIBQHY_VERSION)"
#	hg push
	install -d lib/Linux/arm
	ssh raspi.local "cd libqhy; make clean; git pull; make"
	scp raspi.local:libqhy/lib/Linux/arm/libqhy.a lib/Linux/arm
	install -d lib/Linux/x86
	ssh jessie32.local "cd libqhy; make clean; git pull; make"
	scp jessie32.local:libqhy/lib/Linux/x86/libqhy.a lib/Linux/x86
	install -d lib/Linux/x64
	ssh jessie64.local "cd libqhy; make clean; git pull; make"
	scp jessie64.local:libqhy/lib/Linux/x64/libqhy.a lib/Linux/x64

clean:
	rm -f $(LIBQHY) libqhy_test *.o
