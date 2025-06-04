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

#include "leadr_control_unit.h"
#include "leadr_device.h"

leadrControlUnit *leadr_control_unit_read(RoccatDevice *leadr, GError **error) {
	return (leadrControlUnit *)leadr_device_read(leadr, leadr_REPORT_ID_CONTROL_UNIT, sizeof(leadrControlUnit), error);
}

static gboolean leadr_control_unit_write(RoccatDevice *leadr, leadrControlUnit const *data, GError **error) {
	return leadr_device_write(leadr, (char const *)data, sizeof(leadrControlUnit), error);
}

gboolean leadr_tracking_control_unit_test(RoccatDevice *leadr, guint dcu, guint median, GError **error) {
	leadrControlUnit control_unit;

	control_unit.report_id = leadr_REPORT_ID_CONTROL_UNIT;
	control_unit.size = sizeof(leadrControlUnit);
	control_unit.dcu = dcu;
	control_unit.tcu = leadr_TRACKING_CONTROL_UNIT_ON;
	control_unit.median = median;
	control_unit.action = leadr_CONTROL_UNIT_ACTION_CANCEL;

	return leadr_control_unit_write(leadr, &control_unit, error);
}

gboolean leadr_tracking_control_unit_cancel(RoccatDevice *leadr, guint dcu, GError **error) {
	return leadr_tracking_control_unit_test(leadr, dcu, 0, error);
}

gboolean leadr_tracking_control_unit_accept(RoccatDevice *leadr, guint dcu, guint median, GError **error) {
	leadrControlUnit control_unit;

	control_unit.report_id = leadr_REPORT_ID_CONTROL_UNIT;
	control_unit.size = sizeof(leadrControlUnit);
	control_unit.dcu = dcu;
	control_unit.tcu = leadr_TRACKING_CONTROL_UNIT_ON;
	control_unit.median = median;
	control_unit.action = leadr_CONTROL_UNIT_ACTION_ACCEPT;

	return leadr_control_unit_write(leadr, &control_unit, error);
}

gboolean leadr_tracking_control_unit_off(RoccatDevice *leadr, guint dcu, GError **error) {
	leadrControlUnit control_unit;

	control_unit.report_id = leadr_REPORT_ID_CONTROL_UNIT;
	control_unit.size = sizeof(leadrControlUnit);
	control_unit.dcu = dcu;
	control_unit.tcu = leadr_TRACKING_CONTROL_UNIT_OFF;
	control_unit.median = 0;
	control_unit.action = leadr_CONTROL_UNIT_ACTION_OFF;

	return leadr_control_unit_write(leadr, &control_unit, error);
}

guint leadr_distance_control_unit_get(RoccatDevice *leadr, GError **error) {
	leadrControlUnit *control_unit;
	guint retval;

	control_unit = leadr_control_unit_read(leadr, error);
	if (!control_unit)
		return 0;

	retval = control_unit->dcu;
	g_free(control_unit);
	return retval;
}

gboolean leadr_distance_control_unit_try(RoccatDevice *leadr, guint new_dcu, GError **error) {
	leadrControlUnit control_unit;

	control_unit.report_id = leadr_REPORT_ID_CONTROL_UNIT;
	control_unit.size = sizeof(leadrControlUnit);
	control_unit.dcu = new_dcu;
	control_unit.tcu = 0xff;
	control_unit.median = 0xff;
	control_unit.action = leadr_CONTROL_UNIT_ACTION_UNDEFINED;

	return leadr_control_unit_write(leadr, &control_unit, error);
}

gboolean leadr_distance_control_unit_cancel(RoccatDevice *leadr, guint old_dcu, GError **error) {
	leadrControlUnit control_unit;

	control_unit.report_id = leadr_REPORT_ID_CONTROL_UNIT;
	control_unit.size = sizeof(leadrControlUnit);
	control_unit.dcu = old_dcu;
	control_unit.tcu = 0xff;
	control_unit.median = 0xff;
	control_unit.action = leadr_CONTROL_UNIT_ACTION_CANCEL;

	return leadr_control_unit_write(leadr, &control_unit, error);
}

gboolean leadr_distance_control_unit_accept(RoccatDevice *leadr, guint new_dcu, GError **error) {
	leadrControlUnit control_unit;

	control_unit.report_id = leadr_REPORT_ID_CONTROL_UNIT;
	control_unit.size = sizeof(leadrControlUnit);
	control_unit.dcu = new_dcu;
	control_unit.tcu = 0xff;
	control_unit.median = 0xff;
	control_unit.action = leadr_CONTROL_UNIT_ACTION_ACCEPT;

	return leadr_control_unit_write(leadr, &control_unit, error);
}
