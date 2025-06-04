#ifndef __ROCCAT_leadr_DEVICE_H__
#define __ROCCAT_leadr_DEVICE_H__

/*
 * This file is part of roccat-tools.
 *
 * roccat-tools is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * roccat-tools is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with roccat-tools. If not, see <http://www.gnu.org/licenses/>.
 */

#include "roccat_device_scanner.h"

G_BEGIN_DECLS

typedef enum {
	leadr_CONTROL_REQUEST_CHECK = 0x00,
	/* button indexes for requesting macros */
	leadr_CONTROL_REQUEST_PROFILE_SETTINGS = 0x80,
	leadr_CONTROL_REQUEST_PROFILE_BUTTONS = 0x90,
} leadrControlRequest;

typedef enum {
	leadr_CONTROL_DATA_INDEX_NONE = 0x00,
	leadr_CONTROL_DATA_INDEX_MACRO_1 = 0x10,
	leadr_CONTROL_DATA_INDEX_MACRO_2 = 0x20,
} leadrControlDataIndex;

typedef enum {
	leadr_REPORT_ID_SPECIAL = 0x03,
	leadr_REPORT_ID_CONTROL = 0x04, /* 3 */
	leadr_REPORT_ID_PROFILE = 0x05, /* 3 */
	leadr_REPORT_ID_PROFILE_SETTINGS = 0x06, /* 30 */
	leadr_REPORT_ID_PROFILE_BUTTONS = 0x07, /* 99 */
	leadr_REPORT_ID_MACRO = 0x08, /* 2002 */
	leadr_REPORT_ID_INFO = 0x09, /* 8 */
	leadr_REPORT_ID_A = 0x0a, /* 8 */
	leadr_REPORT_ID_SENSOR = 0x0c, /* 4 */
	leadr_REPORT_ID_SROM_WRITE = 0x0d, /* 1028 */
	leadr_REPORT_ID_DEVICE_STATE = 0x0e, /* 3 */
	leadr_REPORT_ID_CONTROL_UNIT = 0x0f, /* 6 */
	leadr_REPORT_ID_TALK = 0x10, /* 16 */
	leadr_REPORT_ID_11 = 0x11, /* 16 */
	leadr_REPORT_ID_1A = 0x1a, /* 1029 */
} leadrReportId;

typedef enum {
	leadr_INTERFACE_MOUSE = 0,
	leadr_INTERFACE_KEYBOARD = 1,
	leadr_INTERFACE_JOYSTICK = 2,
	leadr_INTERFACE_MISC = 3,
} leadrInterface;

RoccatDevice *leadr_device_first(void);
RoccatDeviceScanner *leadr_device_scanner_new(void);

gchar *leadr_device_read(RoccatDevice *device, guint report_id, gssize length, GError **error);
gboolean leadr_device_write(RoccatDevice *device, gchar const *buffer, gssize length, GError **error);
gboolean leadr_select(RoccatDevice *device, guint profile_index, leadrControlDataIndex data_index, guint request, GError **error);

G_END_DECLS

#endif
