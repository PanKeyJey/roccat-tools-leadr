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

#include "leadr.h"
#include "leadr_profile_buttons.h"
#include "leadr_device.h"
#include "roccat_helper.h"

void leadr_profile_buttons_finalize(leadrProfileButtons *profile_buttons, guint profile_index) {
	g_assert(profile_index < leadr_PROFILE_NUM);
	profile_buttons->profile_index = profile_index;
}

gboolean leadr_profile_buttons_write(RoccatDevice *device, guint profile_index, leadrProfileButtons *profile_buttons, GError **error) {
	g_assert(profile_index < leadr_PROFILE_NUM);
	leadr_profile_buttons_finalize(profile_buttons, profile_index);
	return leadr_device_write(device, (gchar const *)profile_buttons, sizeof(leadrProfileButtons), error);
}

leadrProfileButtons *leadr_profile_buttons_read(RoccatDevice *device, guint profile_index, GError **error) {
	leadrProfileButtons *profile_buttons;

	g_assert(profile_index < leadr_PROFILE_NUM);

	gaminggear_device_lock(GAMINGGEAR_DEVICE(device));

	if (!leadr_select(device, profile_index, leadr_CONTROL_DATA_INDEX_NONE, leadr_CONTROL_REQUEST_PROFILE_BUTTONS, error)) {
		gaminggear_device_unlock(GAMINGGEAR_DEVICE(device));
		return NULL;
	}

	profile_buttons = (leadrProfileButtons *)leadr_device_read(device, leadr_REPORT_ID_PROFILE_BUTTONS, sizeof(leadrProfileButtons), error);

	gaminggear_device_unlock(GAMINGGEAR_DEVICE(device));
	return profile_buttons;
}

gboolean leadr_profile_buttons_equal(leadrProfileButtons const *left, leadrProfileButtons const *right) {
	gboolean equal;
	equal = ROCCAT_MEMCMP_PARTIALLY_TO_END(left, right, leadrProfileButtons, buttons[0]);
	return equal ? FALSE : TRUE;
}
