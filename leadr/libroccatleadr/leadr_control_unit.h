#ifndef __ROCCAT_leadr_CONTROL_UNIT_H__
#define __ROCCAT_leadr_CONTROL_UNIT_H__

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

typedef struct _leadrControlUnit leadrControlUnit;

struct _leadrControlUnit {
	guint8 report_id; /* leadr_REPORT_ID_CONTROL_UNIT */
	guint8 size; /* always 6 */
	guint8 dcu;
	guint8 tcu;
	guint8 median;
	guint8 action;
} __attribute__ ((packed));

typedef enum {
	leadr_CONTROL_UNIT_ACTION_CANCEL = 0x00,
	leadr_CONTROL_UNIT_ACTION_ACCEPT = 0x01,
	leadr_CONTROL_UNIT_ACTION_OFF = 0x02,
	leadr_CONTROL_UNIT_ACTION_UNDEFINED = 0xff,
} leadrControlUnitAction;

typedef enum {
	leadr_TRACKING_CONTROL_UNIT_OFF = 0,
	leadr_TRACKING_CONTROL_UNIT_ON = 1,
} leadrControlUnitTcu;

typedef enum {
	leadr_DISTANCE_CONTROL_UNIT_OFF = 0,
	leadr_DISTANCE_CONTROL_UNIT_EXTRA_LOW = 1,
	leadr_DISTANCE_CONTROL_UNIT_LOW = 2,
	leadr_DISTANCE_CONTROL_UNIT_NORMAL = 3,
} leadrControlUnitDcu;

leadrControlUnit *leadr_control_unit_read(RoccatDevice *leadr, GError **error);

gboolean leadr_tracking_control_unit_test(RoccatDevice *leadr, guint dcu, guint median, GError **error);
gboolean leadr_tracking_control_unit_cancel(RoccatDevice *leadr, guint dcu, GError **error);
gboolean leadr_tracking_control_unit_accept(RoccatDevice *leadr, guint dcu, guint median, GError **error);
gboolean leadr_tracking_control_unit_off(RoccatDevice *leadr, guint dcu, GError **error);

guint leadr_distance_control_unit_get(RoccatDevice *leadr, GError **error);
gboolean leadr_distance_control_unit_try(RoccatDevice *leadr, guint new_dcu, GError **error);
gboolean leadr_distance_control_unit_cancel(RoccatDevice *leadr, guint old_dcu, GError **error);
gboolean leadr_distance_control_unit_accept(RoccatDevice *leadr, guint new_dcu, GError **error);

G_END_DECLS

#endif
