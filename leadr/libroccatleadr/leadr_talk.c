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

#include "leadr_talk.h"
#include "talkfx.h"
#include <string.h>

static gboolean leadr_talk_write(RoccatDevice *device, leadrTalk *talk, GError **error) {
	talk->report_id = leadr_REPORT_ID_TALK;
	talk->size = sizeof(leadrTalk);

	return leadr_device_write(device, (gchar const *)talk, sizeof(leadrTalk), error);
}

static gboolean leadr_talk_write_key(RoccatDevice *device,
		guint8 easyshift, guint8 easyshift_lock,
		guint8 easyaim, GError **error) {
	leadrTalk talk;

	memset(&talk, 0, sizeof(leadrTalk));

	talk.easyshift = easyshift;
	talk.easyshift_lock = easyshift_lock;
	talk.easyaim = easyaim;
	talk.fx_status = leadr_TALKFX_STATE_UNUSED;

	return leadr_talk_write(device, &talk, error);
}

gboolean leadr_talk_easyshift(RoccatDevice *device, guint8 state, GError **error) {
	return leadr_talk_write_key(device,
			state,
			leadr_TALK_EASYSHIFT_UNUSED,
			leadr_TALK_EASYAIM_UNUSED,
			error);
}

gboolean leadr_talk_easyshift_lock(RoccatDevice *device, guint8 state, GError **error) {
	return leadr_talk_write_key(device,
			leadr_TALK_EASYSHIFT_UNUSED,
			state,
			leadr_TALK_EASYAIM_UNUSED,
			error);
}

gboolean leadr_talk_easyaim(RoccatDevice *device, guint8 state, GError **error) {
	return leadr_talk_write_key(device,
			leadr_TALK_EASYSHIFT_UNUSED,
			leadr_TALK_EASYSHIFT_UNUSED,
			state,
			error);
}

static gboolean leadr_talkfx_write(RoccatDevice *leadr, leadrTalk *talk, GError **error) {
	talk->easyshift = leadr_TALK_EASYSHIFT_UNUSED;
	talk->easyshift_lock = leadr_TALK_EASYSHIFT_UNUSED;
	talk->easyaim = leadr_TALK_EASYAIM_UNUSED;
	return leadr_talk_write(leadr, talk, error);
}

gboolean leadr_talkfx(RoccatDevice *leadr, guint32 effect, guint32 ambient_color, guint32 event_color, GError **error) {
	leadrTalk talk;
	guint zone;

	memset(&talk, 0, sizeof(leadrTalk));

	talk.fx_status = ROCCAT_TALKFX_STATE_ON;

	zone = (effect & ROCCAT_TALKFX_ZONE_BIT_MASK) >> ROCCAT_TALKFX_ZONE_BIT_SHIFT;
	talk.zone = (zone == ROCCAT_TALKFX_ZONE_AMBIENT) ? leadr_TALKFX_ZONE_AMBIENT : leadr_TALKFX_ZONE_EVENT;

	talk.effect = (effect & ROCCAT_TALKFX_EFFECT_BIT_MASK) >> ROCCAT_TALKFX_EFFECT_BIT_SHIFT;
	talk.speed = (effect & ROCCAT_TALKFX_SPEED_BIT_MASK) >> ROCCAT_TALKFX_SPEED_BIT_SHIFT;
	talk.ambient_red = (ambient_color & ROCCAT_TALKFX_COLOR_RED_MASK) >> ROCCAT_TALKFX_COLOR_RED_SHIFT;
	talk.ambient_green = (ambient_color & ROCCAT_TALKFX_COLOR_GREEN_MASK) >> ROCCAT_TALKFX_COLOR_GREEN_SHIFT;
	talk.ambient_blue = (ambient_color & ROCCAT_TALKFX_COLOR_BLUE_MASK) >> ROCCAT_TALKFX_COLOR_BLUE_SHIFT;
	talk.event_red = (event_color & ROCCAT_TALKFX_COLOR_RED_MASK) >> ROCCAT_TALKFX_COLOR_RED_SHIFT;
	talk.event_green = (event_color & ROCCAT_TALKFX_COLOR_GREEN_MASK) >> ROCCAT_TALKFX_COLOR_GREEN_SHIFT;
	talk.event_blue = (event_color & ROCCAT_TALKFX_COLOR_BLUE_MASK) >> ROCCAT_TALKFX_COLOR_BLUE_SHIFT;

	return leadr_talkfx_write(leadr, &talk, error);
}

gboolean leadr_talkfx_off(RoccatDevice *leadr, GError **error) {
	leadrTalk talk;

	memset(&talk, 0, sizeof(leadrTalk));

	talk.fx_status = ROCCAT_TALKFX_STATE_OFF;

	return leadr_talkfx_write(leadr, &talk, error);
}
