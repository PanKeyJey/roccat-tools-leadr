#ifndef __ROCCAT_leadr_H__
#define __ROCCAT_leadr_H__

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

#include <glib.h>

G_BEGIN_DECLS

#define USB_DEVICE_ID_ROCCAT_leadr_WIRED 0x2e4c
#define USB_DEVICE_ID_ROCCAT_leadr_WIRELESS 0x2e4e

#define leadr_DEVICE_NAME "leadr"
#define leadr_DEVICE_NAME_WIRED "leadr Wired"
#define leadr_DEVICE_NAME_WIRELESS "leadr Wireless"
#define leadr_DEVICE_NAME_COMBINED "leadr Wired/Wireless"

enum {
	leadr_PROFILE_NUM = 5,
	leadr_LIGHTS_NUM = 2,
	leadr_CPI_MIN = 200,
	leadr_CPI_MAX = 8200,
	leadr_CPI_STEP = 200,
};

G_END_DECLS

#endif
