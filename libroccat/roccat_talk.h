#ifndef __ROCCAT_TALK_H__
#define __ROCCAT_TALK_H__

#include <glib.h>

G_BEGIN_DECLS

typedef enum {
	ROCCAT_TALK_DEVICE_NONE = 0,
	ROCCAT_TALK_DEVICE_KEYBOARD = 0xfffd,
	ROCCAT_TALK_DEVICE_MOUSE = 0xfffe,
	ROCCAT_TALK_DEVICE_ALL = 0xffff,
} RoccatTalkDevice;

typedef enum {
	ROCCAT_TALK_EASYAIM_OFF = 0,
	ROCCAT_TALK_EASYAIM_1 = 1,
	ROCCAT_TALK_EASYAIM_2 = 2,
	ROCCAT_TALK_EASYAIM_3 = 3,
	ROCCAT_TALK_EASYAIM_4 = 4,
	ROCCAT_TALK_EASYAIM_5 = 5,
} RoccatTalkEasyaim;

// FUNKCJE UŻYWANE PRZEZ INNE MODUŁY (muszą być zachowane)
guint roccat_talk_device_get_type(guint talk_device);
gchar const *roccat_talk_device_get_text_static(guint talk_device);

G_END_DECLS

#endif
