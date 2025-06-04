#ifndef __ROCCAT_leadr_SENSOR_H__
#define __ROCCAT_leadr_SENSOR_H__

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

enum {
	leadr_SENSOR_IMAGE_SIZE = 30,
};

typedef struct _leadrSensorImage leadrSensorImage;

struct _leadrSensorImage {
	guint8 report_id; /* leadr_REPORT_ID_SENSOR */
	guint8 action; /* 3 */
	guint8 unused1; /* 0 */
	guint8 data[leadr_SENSOR_IMAGE_SIZE * leadr_SENSOR_IMAGE_SIZE];
	guint8 unused2[125]; /* all bytes 0 */
} __attribute__ ((packed));

gboolean leadr_sensor_write_register(RoccatDevice *device, guint8 reg, guint8 value, GError **error);
guint8 leadr_sensor_read_register(RoccatDevice *device, guint8 reg, GError **error);

leadrSensorImage *leadr_sensor_image_read(RoccatDevice *device, GError **error);
gboolean leadr_sensor_calibrate_step(RoccatDevice *device, GError **error);
guint leadr_sensor_image_get_median(leadrSensorImage const *image);

G_END_DECLS

#endif
