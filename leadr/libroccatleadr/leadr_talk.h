#ifndef __ROCCAT_leadr_TALK_H__
#define __ROCCAT_leadr_TALK_H__

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

#include "leadr_device.h"

G_BEGIN_DECLS

typedef struct _leadrTalk leadrTalk;

struct _leadrTalk {
	guint8 report_id; /* leadr_REPORT_ID_TALK */
	guint8 size; /* always 16 */
	guint8 easyshift;
	guint8 easyshift_lock;
	guint8 easyaim;
	guint8 fx_status;
	guint8 zone;
	guint8 unused;
	guint8 effect;
	guint8 speed;
	guint8 ambient_red;
	guint8 ambient_green;
	guint8 ambient_blue;
	guint8 event_red;
	guint8 event_green;
	guint8 event_blue;
} __attribute__ ((packed));

/* also valid as leadrTalkEasyshiftLock */
typedef enum {
	leadr_TALK_EASYSHIFT_OFF = 0,
	leadr_TALK_EASYSHIFT_ON = 1,
	leadr_TALK_EASYSHIFT_UNUSED = 0xff,
} leadrTalkEasyshift;

typedef enum {
	leadr_TALK_EASYAIM_OFF = 0,
	leadr_TALK_EASYAIM_1 = 1,
	leadr_TALK_EASYAIM_2 = 2,
	leadr_TALK_EASYAIM_3 = 3,
	leadr_TALK_EASYAIM_4 = 4,
	leadr_TALK_EASYAIM_5 = 5,
	leadr_TALK_EASYAIM_UNUSED = 0xff,
} leadrTalkEasyaim;

typedef enum {
	leadr_TALKFX_ZONE_EVENT = 4,
	leadr_TALKFX_ZONE_AMBIENT = 3,
} leadrTalkfxZone;

typedef enum {
	leadr_TALKFX_STATE_UNUSED = 0xff,
} leadrTalkfxState;

gboolean leadr_talk_easyshift(RoccatDevice *device, guint8 state, GError **error);
gboolean leadr_talk_easyshift_lock(RoccatDevice *device, guint8 state, GError **error);
gboolean leadr_talk_easyaim(RoccatDevice *device, guint8 state, GError **error);
gboolean leadr_talkfx(RoccatDevice *leadr, guint32 effect, guint32 ambient_color, guint32 event_color, GError **error);
gboolean leadr_talkfx_off(RoccatDevice *leadr, GError **error);

G_END_DECLS

#endif
