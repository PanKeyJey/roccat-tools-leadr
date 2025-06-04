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

#include "leadr_gfx.h"
#include "leadr_device.h"
#include "talkfx.h"
#include <gaminggear/gfx.h>
#include <string.h>

#define leadr_GFX_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), leadr_GFX_TYPE, leadrGfxPrivate))

typedef struct _leadrGfxColor leadrGfxColor;
typedef struct _leadrGfxData leadrGfxData;

struct _leadrGfxColor {
	guint8 red;
	guint8 green;
	guint8 blue;
} __attribute__ ((packed));

struct _leadrGfxData {
	guint8 report_id; /* leadr_REPORT_ID_TALK */
	guint8 size; /* always 16 */
	guint8 mode;
	guint8 effect;
	leadrGfxColor colors[leadr_LIGHTS_NUM];
	guint8 unused[6];
} __attribute__ ((packed));

typedef enum {
	leadr_GFX_MODE_GFX = 0x02,
} leadrGfxMode;

typedef enum {
	leadr_GFX_EFFECT_OFF = 0x00,
	leadr_GFX_EFFECT_ON = 0x01,
	leadr_GFX_EFFECT_BLINK = 0x02,
} leadrGfxEffect;

struct _leadrGfxPrivate {
	RoccatDevice *device;
	leadrGfxData *data;
};

enum {
	PROP_0,
	PROP_DEVICE,
};

G_DEFINE_TYPE(leadrGfx, leadr_gfx, G_TYPE_OBJECT);

leadrGfx *leadr_gfx_new(RoccatDevice * const device) {
	return leadr_GFX(g_object_new(leadr_GFX_TYPE,
			"device", device,
			NULL));
}

static void leadr_gfx_init(leadrGfx *gfx) {
	gfx->priv = leadr_GFX_GET_PRIVATE(gfx);
}

static void leadr_gfx_set_property(GObject *object, guint prop_id, GValue const *value, GParamSpec *pspec) {
	leadrGfxPrivate *priv = leadr_GFX(object)->priv;
	switch(prop_id) {
	case PROP_DEVICE:
		priv->device = ROCCAT_DEVICE(g_value_get_object(value));
		g_object_ref(G_OBJECT(priv->device));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
		break;
	}
}

static GObject *leadr_gfx_constructor(GType gtype, guint n_properties, GObjectConstructParam *properties) {
	leadrGfxPrivate *priv;
	leadrGfx *gfx;
	GObject *obj;

	obj = G_OBJECT_CLASS(leadr_gfx_parent_class)->constructor(gtype, n_properties, properties);
	gfx = leadr_GFX(obj);
	priv = gfx->priv;

	priv->data = (leadrGfxData *)leadr_device_read(priv->device, leadr_REPORT_ID_TALK, sizeof(leadrGfxData), NULL);
	if (!priv->data)
		priv->data = (leadrGfxData *)g_malloc0(sizeof(leadrGfxData));

	priv->data->report_id = leadr_REPORT_ID_TALK;
	priv->data->size = sizeof(leadrGfxData);
	priv->data->mode = leadr_GFX_MODE_GFX;
	priv->data->effect = leadr_GFX_EFFECT_ON;

	return obj;
}

static void leadr_gfx_finalize(GObject *object) {
	leadrGfxPrivate *priv = leadr_GFX(object)->priv;
	g_object_unref(G_OBJECT(priv->device));
	g_free(priv->data);
	G_OBJECT_CLASS(leadr_gfx_parent_class)->finalize(object);
}

static void leadr_gfx_class_init(leadrGfxClass *gfx_class) {
	GObjectClass *gobject_class = (GObjectClass*)gfx_class;
	gobject_class->constructor = leadr_gfx_constructor;
	gobject_class->finalize = leadr_gfx_finalize;
	gobject_class->set_property = leadr_gfx_set_property;

	g_type_class_add_private(gfx_class, sizeof(leadrGfxPrivate));

	g_object_class_install_property(gobject_class, PROP_DEVICE,
			g_param_spec_object("device",
					"device",
					"Device",
					ROCCAT_DEVICE_TYPE,
					G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));
}

gboolean leadr_gfx_update(leadrGfx *gfx, GError **error) {
	leadrGfxPrivate *priv = gfx->priv;
	return leadr_device_write(priv->device, (gchar const *)priv->data, sizeof(leadrGfxData), error);
}

guint32 leadr_gfx_get_color(leadrGfx *gfx, guint index) {
	leadrGfxData *data = gfx->priv->data;
	guint32 color = 0;
	gfx_color_set_brightness(&color, 0xff);
	gfx_color_set_red(&color, data->colors[index].red);
	gfx_color_set_green(&color, data->colors[index].green);
	gfx_color_set_blue(&color, data->colors[index].blue);
	return color;
}

void leadr_gfx_set_color(leadrGfx *gfx, guint index, guint32 color) {
	leadrGfxData *data = gfx->priv->data;
	data->colors[index].red = gfx_color_get_red(color);
	data->colors[index].green = gfx_color_get_green(color);
	data->colors[index].blue = gfx_color_get_blue(color);
}
