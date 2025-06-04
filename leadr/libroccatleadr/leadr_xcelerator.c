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

#include "leadr_xcelerator.h"
#include "leadr_info.h"

gboolean leadr_xcelerator_calibration_start(RoccatDevice *device, GError **error) {
	leadrInfo info = { 0 };
	info.function = leadr_INFO_FUNCTION_XCELERATOR_CALIB_START;
	return leadr_info_write(device, &info, error);
}

gboolean leadr_xcelerator_calibration_data(RoccatDevice *device, guint8 min, guint8 mid, guint8 max, GError **error) {
	leadrInfo info = { 0 };
	info.function = leadr_INFO_FUNCTION_XCELERATOR_CALIB_DATA;
	info.xcelerator_min = min;
	info.xcelerator_mid = mid;
	info.xcelerator_max = max;
	return leadr_info_write(device, &info, error);
}

gboolean leadr_xcelerator_calibration_end(RoccatDevice *device, GError **error) {
	leadrInfo info = { 0 };
	info.function = leadr_INFO_FUNCTION_XCELERATOR_CALIB_END;
	return leadr_info_write(device, &info, error);
}
