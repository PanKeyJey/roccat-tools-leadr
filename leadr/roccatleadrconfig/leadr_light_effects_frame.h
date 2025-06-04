#ifndef __ROCCAT_leadr_LIGHT_EFFECTS_FRAME_H__
#define __ROCCAT_leadr_LIGHT_EFFECTS_FRAME_H__

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

#include "leadr_rmp.h"
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define leadr_LIGHT_EFFECTS_FRAME_TYPE (leadr_light_effects_frame_get_type())
#define leadr_LIGHT_EFFECTS_FRAME(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), leadr_LIGHT_EFFECTS_FRAME_TYPE, leadrLightEffectsFrame))
#define IS_leadr_LIGHT_EFFECTS_FRAME(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), leadr_LIGHT_EFFECTS_FRAME_TYPE))

typedef struct _leadrLightEffectsFrame leadrLightEffectsFrame;

GType leadr_light_effects_frame_get_type(void);
GtkWidget *leadr_light_effects_frame_new(void);

void leadr_light_effects_frame_set_from_rmp(leadrLightEffectsFrame *frame, leadrRmp *rmp);
void leadr_light_effects_frame_update_rmp(leadrLightEffectsFrame *frame, leadrRmp *rmp);

G_END_DECLS

#endif
