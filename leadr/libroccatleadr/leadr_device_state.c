/*
 * This file is part of roccat-tools.
 *
 * roccat-tools is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Macro Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * roccat-tools is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Macro Public License for more details.
 *
 * You should have received a copy of the GNU Macro Public License
 * along with roccat-tools. If not, see <http://www.gnu.org/licenses/>.
 */

#include "leadr_device_state.h"
#include "leadr_device.h"

typedef struct _leadrDeviceState leadrDeviceState;

struct _leadrDeviceState {
	guint8 report_id; /* leadr_REPORT_ID_DEVICE_STATE */
	guint8 size; /* always 0x03 */
	guint8 state;
} __attribute__ ((packed));

gboolean leadr_device_state_write(RoccatDevice *leadr, guint state, GError **error) {
	leadrDeviceState device_state;

	device_state.report_id = leadr_REPORT_ID_DEVICE_STATE;
	device_state.size = sizeof(leadrDeviceState);
	device_state.state = state;

	return leadr_device_write(leadr, (gchar const *)&device_state, sizeof(leadrDeviceState), error);
}
