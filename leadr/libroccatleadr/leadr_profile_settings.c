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
#include "leadr_profile_settings.h"
#include "leadr_device.h"
#include "roccat_helper.h"

static guint16 leadr_profile_settings_calc_checksum(leadrProfileSettings const *profile_settings) {
	return ROCCAT_BYTESUM_PARTIALLY(profile_settings, leadrProfileSettings, report_id, checksum);
}

static void leadr_profile_settings_set_checksum(leadrProfileSettings *profile_settings) {
	guint16 checksum = leadr_profile_settings_calc_checksum(profile_settings);
	profile_settings->checksum = GUINT16_TO_LE(checksum);
}

void leadr_profile_settings_finalize(leadrProfileSettings *profile_settings, guint profile_index) {
	g_assert(profile_index < leadr_PROFILE_NUM);
	profile_settings->profile_index = profile_index;
	leadr_profile_settings_set_checksum(profile_settings);
}

gboolean leadr_profile_settings_write(RoccatDevice *device, guint profile_index, leadrProfileSettings *profile_settings, GError **error) {
	g_assert(profile_index < leadr_PROFILE_NUM);
	leadr_profile_settings_finalize(profile_settings, profile_index);
	return leadr_device_write(device, (gchar const *)profile_settings, sizeof(leadrProfileSettings), error);
}

leadrProfileSettings *leadr_profile_settings_read(RoccatDevice *device, guint profile_index, GError **error) {
	leadrProfileSettings *profile_settings;

	g_assert(profile_index < leadr_PROFILE_NUM);

	gaminggear_device_lock(GAMINGGEAR_DEVICE(device));

	if (!leadr_select(device, profile_index, leadr_CONTROL_DATA_INDEX_NONE, leadr_CONTROL_REQUEST_PROFILE_SETTINGS, error)) {
		gaminggear_device_unlock(GAMINGGEAR_DEVICE(device));
		return NULL;
	}

	profile_settings = (leadrProfileSettings *)leadr_device_read(device, leadr_REPORT_ID_PROFILE_SETTINGS, sizeof(leadrProfileSettings), error);

	gaminggear_device_unlock(GAMINGGEAR_DEVICE(device));
	return profile_settings;
}

gboolean leadr_profile_settings_equal(leadrProfileSettings const *left, leadrProfileSettings const *right) {
	gboolean equal;
	equal = ROCCAT_MEMCMP_PARTIALLY(left, right, leadrProfileSettings, advanced_sensitivity, checksum);
	return equal ? FALSE : TRUE;
}
