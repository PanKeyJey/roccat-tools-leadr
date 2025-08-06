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

#ifndef __ROCCAT_TALKFX_SELECTOR_H__
#define __ROCCAT_TALKFX_SELECTOR_H__

#include <glib-object.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define ROCCAT_TALKFX_SELECTOR_TYPE (roccat_talkfx_selector_get_type())
#define ROCCAT_TALKFX_SELECTOR(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), ROCCAT_TALKFX_SELECTOR_TYPE, RoccatTalkFXSelector))
#define ROCCAT_TALKFX_SELECTOR_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), ROCCAT_TALKFX_SELECTOR_TYPE, RoccatTalkFXSelectorClass))
#define ROCCAT_IS_TALKFX_SELECTOR(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), ROCCAT_TALKFX_SELECTOR_TYPE))
#define ROCCAT_IS_TALKFX_SELECTOR_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), ROCCAT_TALKFX_SELECTOR_TYPE))
#define ROCCAT_TALKFX_SELECTOR_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), ROCCAT_TALKFX_SELECTOR_TYPE, RoccatTalkFXSelectorClass))

typedef struct _RoccatTalkFXSelector RoccatTalkFXSelector;
typedef struct _RoccatTalkFXSelectorClass RoccatTalkFXSelectorClass;
typedef struct _RoccatTalkFXSelectorPrivate RoccatTalkFXSelectorPrivate;

struct _RoccatTalkFXSelector {
        GtkToggleButton parent;
        RoccatTalkFXSelectorPrivate *priv;
};

struct _RoccatTalkFXSelectorClass {
        GtkToggleButtonClass parent_class;
};

GType roccat_talkfx_selector_get_type(void) G_GNUC_CONST;
GtkWidget *roccat_talkfx_selector_new(void);

void roccat_talkfx_selector_set_active(RoccatTalkFXSelector *selector, gboolean active);
gboolean roccat_talkfx_selector_get_active(RoccatTalkFXSelector *selector);

G_END_DECLS

#endif /* __ROCCAT_TALKFX_SELECTOR_H__ */
