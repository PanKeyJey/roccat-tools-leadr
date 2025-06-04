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

#include "leadr_profile.h"
#include "leadr_device.h"
#include "leadr.h"

typedef struct _leadrProfile leadrProfile;

struct _leadrProfile {
	guint8 report_id; /* leadr_REPORT_ID_PROFILE */
	guint8 size; /* always 0x03 */
	guint8 profile_index;
} __attribute__ ((packed));

guint leadr_profile_read(RoccatDevice *leadr, GError **error) {
	leadrProfile *profile;
	guint result;

	profile = (leadrProfile *)leadr_device_read(leadr, leadr_REPORT_ID_PROFILE, sizeof(leadrProfile), error);
	if (*error)
		return 0;

	result = profile->profile_index;
	g_free(profile);
	return result;
}

gboolean leadr_profile_write(RoccatDevice *leadr, guint profile_index, GError **error) {
	leadrProfile profile;

	g_assert(profile_index < leadr_PROFILE_NUM);

	profile.report_id = leadr_REPORT_ID_PROFILE;
	profile.size = sizeof(leadrProfile);
	profile.profile_index = profile_index;

	return leadr_device_write(leadr, (gchar const *)&profile, sizeof(leadrProfile), error);
}
