/*
 * This file is part of roccat-tools.
 * See license info in header.
 */

#include "roccat_talk.h"
#include "leadr.h"
#include "i18n-lib.h"

guint roccat_talk_device_get_type(guint talk_device) {
	switch (talk_device) {
		case ROCCAT_TALK_DEVICE_MOUSE:
                case USB_DEVICE_ID_ROCCAT_leadr_WIRED:
                case USB_DEVICE_ID_ROCCAT_leadr_WIRELESS:
                        return ROCCAT_TALK_DEVICE_MOUSE;
		default:
			return ROCCAT_TALK_DEVICE_NONE;
	}
}

gchar const *roccat_talk_device_get_text_static(guint talk_device) {
	switch (talk_device) {
		case ROCCAT_TALK_DEVICE_ALL:
			return _("All");
		case ROCCAT_TALK_DEVICE_MOUSE:
			return _("*Any mouse");
                case USB_DEVICE_ID_ROCCAT_leadr_WIRED:
                        return leadr_DEVICE_NAME_WIRED;
                case USB_DEVICE_ID_ROCCAT_leadr_WIRELESS:
                        return leadr_DEVICE_NAME_WIRELESS;
                default:
                        return _("Unknown device");
        }
}
