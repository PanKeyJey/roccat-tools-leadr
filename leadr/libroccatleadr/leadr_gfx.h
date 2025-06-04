#ifndef __ROCCAT_leadr_GFX_H__
#define __ROCCAT_leadr_GFX_H__

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

#include "leadr.h"
#include "roccat_device.h"

G_BEGIN_DECLS

#define leadr_GFX_TYPE (leadr_gfx_get_type())
#define leadr_GFX(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), leadr_GFX_TYPE, leadrGfx))
#define IS_leadr_GFX(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), leadr_GFX_TYPE))
#define leadr_GFX_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), leadr_GFX_TYPE, leadrGfxClass))
#define IS_leadr_GFX_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), leadr_GFX_TYPE))

typedef struct _leadrGfx leadrGfx;
typedef struct _leadrGfxClass leadrGfxClass;
typedef struct _leadrGfxPrivate leadrGfxPrivate;

struct _leadrGfx {
	GObject parent;
	leadrGfxPrivate *priv;
};

struct _leadrGfxClass {
	GObjectClass parent_class;
};

GType leadr_gfx_get_type(void);
leadrGfx *leadr_gfx_new(RoccatDevice * const device);

gboolean leadr_gfx_update(leadrGfx *gfx, GError **error);

guint32 leadr_gfx_get_color(leadrGfx *gfx, guint index);
void leadr_gfx_set_color(leadrGfx *gfx, guint index, guint32 color);

G_END_DECLS

#endif
