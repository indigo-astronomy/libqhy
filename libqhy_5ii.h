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

#ifndef libqhy5ii_h
#define libqhy5ii_h

extern unsigned char libqhy_5ii_firmware[];

extern bool libqhy_5ii_init(libqhy_device_context *context);
extern bool libqhy_5ii_start_exposure(libqhy_device_context *context, double exposure);
extern bool libqhy_5ii_read_pixels(libqhy_device_context *context, unsigned short *image);
extern bool libqhy_5ii_check_temperature(libqhy_device_context *context, double *temperature);

#endif /* libqhy5ii_h */
