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

#include "leadr_info.h"
#include "leadr_device.h"

leadrInfo *leadr_info_read(RoccatDevice *device, GError **error) {
	return (leadrInfo *)leadr_device_read(device, leadr_REPORT_ID_INFO, sizeof(leadrInfo), error);
}

static void leadr_info_finalize(leadrInfo *info) {
	info->report_id = leadr_REPORT_ID_INFO;
	info->size = sizeof(leadrInfo);
}

gboolean leadr_info_write(RoccatDevice *device, leadrInfo *info, GError **error) {
	leadr_info_finalize(info);
	return leadr_device_write(device, (gchar const *)info, sizeof(leadrInfo), error);
}

gboolean leadr_reset(RoccatDevice *device, GError **error) {
	leadrInfo info = { 0 };
	info.function = leadr_INFO_FUNCTION_RESET;
	return leadr_info_write(device, &info, error);
}

guint leadr_firmware_version_read(RoccatDevice *device, GError **error) {
	leadrInfo *info;
	guint result;

	info = leadr_info_read(device, error);
	if (!info)
		return 0;
	result = info->firmware_version;
	g_free(info);
	return result;
}
