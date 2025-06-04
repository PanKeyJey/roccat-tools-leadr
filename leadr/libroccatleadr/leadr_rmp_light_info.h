#ifndef __ROCCAT_leadr_RMP_LIGHT_INFO_H__
#define __ROCCAT_leadr_RMP_LIGHT_INFO_H__

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

#include "leadr_profile_settings.h"

G_BEGIN_DECLS

typedef struct _leadrRmpLightInfo leadrRmpLightInfo;

enum {
	leadr_RMP_LIGHT_INFO_COLORS_NUM = 16,
};

struct _leadrRmpLightInfo {
	guint8 index;
	guint8 state;
	guint8 red;
	guint8 green;
	guint8 blue;
	guint8 null;
	guint8 checksum;
} __attribute__ ((packed));

typedef enum {
	leadr_RMP_LIGHT_CHOSE_TYPE_PALETTE = 0,
	leadr_RMP_LIGHT_CHOSE_TYPE_CUSTOM = 1,
} leadrRmpLightType;

typedef enum {
	leadr_RMP_LIGHT_INFO_STATE_ON = 1,
	leadr_RMP_LIGHT_INFO_STATE_OFF = 2,
} leadrRmpLightInfoState;

leadrRmpLightInfo const *leadr_rmp_light_info_get_standard(guint index);
guint8 leadr_rmp_light_info_calc_checksum(leadrRmpLightInfo const *rmp_light_info);
void leadr_rmp_light_info_set_checksum(leadrRmpLightInfo *rmp_light_info);
void leadr_rmp_light_info_to_light(leadrRmpLightInfo const *from, leadrLight *to);
void leadr_light_to_rmp_light_info(leadrLight const *from, leadrRmpLightInfo *to, guint state);
gboolean leadr_rmp_light_info_equal(leadrRmpLightInfo const *left, leadrRmpLightInfo const *right);

G_END_DECLS

#endif
