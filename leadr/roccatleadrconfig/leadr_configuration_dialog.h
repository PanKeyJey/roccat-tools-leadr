#ifndef __ROCCAT_leadr_CONFIGURATION_DIALOG_H__
#define __ROCCAT_leadr_CONFIGURATION_DIALOG_H__

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

#include "roccat_key_file.h"
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define leadr_CONFIGURATION_DIALOG_TYPE (leadr_configuration_dialog_get_type())
#define leadr_CONFIGURATION_DIALOG(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), leadr_CONFIGURATION_DIALOG_TYPE, leadrConfigurationDialog))
#define IS_leadr_CONFIGURATION_DIALOG(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), leadr_CONFIGURATION_DIALOG_TYPE))

typedef struct _leadrConfigurationDialog leadrConfigurationDialog;
typedef struct _leadrConfigurationDialogPrivate leadrConfigurationDialogPrivate;

struct _leadrConfigurationDialog {
	GtkDialog dialog;
	leadrConfigurationDialogPrivate *priv;
};

GType leadr_configuration_dialog_get_type(void);
gboolean leadr_configuration_dialog(GtkWindow *parent, RoccatKeyFile *config);
GtkWidget *leadr_configuration_dialog_new(GtkWindow *parent);

void leadr_configuration_dialog_get_configuration(leadrConfigurationDialog *leadr_configuration_dialog, RoccatKeyFile *config);
void leadr_configuration_dialog_set_configuration(leadrConfigurationDialog *leadr_configuration_dialog, RoccatKeyFile *config);

gboolean leadr_configuration_dialog_save(GtkWindow *parent, RoccatKeyFile *config);

G_END_DECLS

#endif
