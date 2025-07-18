#ifndef __ROCCAT_leadr_INFO_H__
#define __ROCCAT_leadr_INFO_H__

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

#include "roccat_device.h"

G_BEGIN_DECLS

typedef struct _leadrInfo leadrInfo;

struct _leadrInfo {
	guint8 report_id; /* leadr_REPORT_ID_INFO */
	guint8 size; /* always 0x08 */
	union {
		guint8 firmware_version; /* r */
		guint8 function; /* w */
	};
	union {
		guint8 dfu_version; /* r */
		guint8 xcelerator_min; /* w */
	};
	guint8 xcelerator_mid;
	guint8 xcelerator_max;
	guint8 unused1;
	guint8 unused2;
} __attribute__ ((packed));

typedef enum {
	leadr_INFO_FUNCTION_RESET = 0x1,
	leadr_INFO_FUNCTION_XCELERATOR_CALIB_START = 0x8,
	leadr_INFO_FUNCTION_XCELERATOR_CALIB_DATA = 0xb,
	leadr_INFO_FUNCTION_XCELERATOR_CALIB_END = 0xa,
} leadrInfoFunction;

leadrInfo *leadr_info_read(RoccatDevice *device, GError **error);
gboolean leadr_info_write(RoccatDevice *device, leadrInfo *info, GError **error);

guint leadr_firmware_version_read(RoccatDevice *device, GError **error);

gboolean leadr_reset(RoccatDevice *device, GError **error);

G_END_DECLS

#endif
