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

#include "leadr_profile_page.h"
#include "leadr_buttons_frame.h"
#include "leadr_light_effects_frame.h"
#include "leadr_color_frame.h"
#include "leadr_cpi_selector.h"
#include "roccat_sensitivity_frame.h"
#include "roccat_polling_rate_frame.h"
#include "roccat_gamefile_selector.h"
#include "roccat_talkfx_selector.h"
#include "i18n.h"

#define leadr_PROFILE_PAGE_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), leadr_PROFILE_PAGE_TYPE, leadrProfilePageClass))
#define IS_leadr_PROFILE_PAGE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), leadr_PROFILE_PAGE_TYPE))
#define leadr_PROFILE_PAGE_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), leadr_PROFILE_PAGE_TYPE, leadrProfilePagePrivate))

typedef struct _leadrProfilePageClass leadrProfilePageClass;
typedef struct _leadrProfilePagePrivate leadrProfilePagePrivate;

struct _leadrProfilePage {
	RoccatProfilePage parent;
	leadrProfilePagePrivate *priv;
};

struct _leadrProfilePageClass {
	RoccatProfilePageClass parent_class;
};

struct _leadrProfilePagePrivate {
	leadrRmp *rmp;
	leadrButtonsFrame *buttons;
	RoccatGamefileSelector *gamefiles;
	RoccatPollingRateFrame *polling_rate;
	leadrCpiSelector *cpi;
	RoccatSensitivityFrame *sensitivity;
	leadrLightEffectsFrame *light_effects;
	leadrColorFrame *colors;
	RoccatTalkFXSelector *talkfx;
};

G_DEFINE_TYPE(leadrProfilePage, leadr_profile_page, ROCCAT_PROFILE_PAGE_TYPE);

GtkWidget *leadr_profile_page_new(void) {
	return GTK_WIDGET(g_object_new(leadr_PROFILE_PAGE_TYPE, NULL));
}

static void set_from_rmp(leadrProfilePage *profile_page) {
	leadrProfilePagePrivate *priv = profile_page->priv;
	guint i;
	guint sync;

	roccat_profile_page_set_name(ROCCAT_PROFILE_PAGE(profile_page), leadr_rmp_get_profile_name(priv->rmp));

	leadr_buttons_frame_set_from_rmp(priv->buttons, priv->rmp);
	leadr_light_effects_frame_set_from_rmp(priv->light_effects, priv->rmp);
	leadr_color_frame_set_from_rmp(priv->colors, priv->rmp);
	leadr_cpi_selector_set_from_rmp(priv->cpi, priv->rmp);
	roccat_polling_rate_frame_set_value(priv->polling_rate, leadr_rmp_get_polling_rate(priv->rmp));
	roccat_talkfx_selector_set_active(priv->talkfx, leadr_rmp_get_talkfx(priv->rmp) == leadr_PROFILE_SETTINGS_TALKFX_ON ? TRUE : FALSE);

	sync = leadr_rmp_get_xy_synchronous(priv->rmp);
	roccat_sensitivity_frame_set_advanced(priv->sensitivity, sync);
	if (sync == ROCCAT_SENSITIVITY_ADVANCED_ON) {
		roccat_sensitivity_frame_set_x_value(priv->sensitivity, leadr_rmp_get_sensitivity_x(priv->rmp));
		roccat_sensitivity_frame_set_y_value(priv->sensitivity, leadr_rmp_get_sensitivity_y(priv->rmp));
	} else
		roccat_sensitivity_frame_set_x_value(priv->sensitivity, leadr_rmp_get_sensitivity(priv->rmp));

	for (i = 0; i < leadr_GAMEFILE_NUM; ++i)
		roccat_gamefile_selector_set_text(priv->gamefiles, i, leadr_rmp_get_game_file_name(priv->rmp, i));
}

static void update_rmp(leadrProfilePage *profile_page, leadrRmp *rmp) {
	leadrProfilePagePrivate *priv = profile_page->priv;
	gchar *profile_name;
	guint i;
	guint sync;

	profile_name = roccat_profile_page_get_name(ROCCAT_PROFILE_PAGE(profile_page));
	leadr_rmp_set_profile_name(rmp, profile_name);
	g_free(profile_name);

	leadr_buttons_frame_update_rmp(priv->buttons, rmp);
	leadr_light_effects_frame_update_rmp(priv->light_effects, rmp);
	leadr_color_frame_update_rmp(priv->colors, rmp);
	leadr_cpi_selector_update_rmp(priv->cpi, rmp);
	leadr_rmp_set_polling_rate(rmp, roccat_polling_rate_frame_get_value(priv->polling_rate));
	leadr_rmp_set_talkfx(rmp, roccat_talkfx_selector_get_active(priv->talkfx) ? leadr_PROFILE_SETTINGS_TALKFX_ON : leadr_PROFILE_SETTINGS_TALKFX_OFF);

	sync = roccat_sensitivity_frame_get_advanced(priv->sensitivity);
	leadr_rmp_set_xy_synchronous(rmp, sync);
	if (sync == ROCCAT_SENSITIVITY_ADVANCED_ON) {
		leadr_rmp_set_sensitivity_x(rmp, roccat_sensitivity_frame_get_x_value(priv->sensitivity));
		leadr_rmp_set_sensitivity_y(rmp, roccat_sensitivity_frame_get_y_value(priv->sensitivity));
	} else
		leadr_rmp_set_sensitivity(rmp, roccat_sensitivity_frame_get_x_value(priv->sensitivity));

	for (i = 0; i < leadr_GAMEFILE_NUM; ++i)
		leadr_rmp_set_game_file_name(rmp, i, roccat_gamefile_selector_get_text(priv->gamefiles, i));
}

static void reset_cb(RoccatProfilePage *profile_page, gpointer user_data) {
	set_from_rmp(leadr_PROFILE_PAGE(profile_page));
}

static void leadr_profile_page_init(leadrProfilePage *profile_page) {
	leadrProfilePagePrivate *priv = leadr_PROFILE_PAGE_GET_PRIVATE(profile_page);
	GtkWidget *content;
	GtkWidget *left;
	GtkWidget *light;
	GtkWidget *right;

	profile_page->priv = priv;

	content = gtk_hbox_new(FALSE, 0);
	left = gtk_vbox_new(FALSE, 0);
	light = gtk_hbox_new(FALSE, 0);
	right = gtk_vbox_new(FALSE, 0);
	priv->buttons = leadr_BUTTONS_FRAME(leadr_buttons_frame_new());
	priv->gamefiles = ROCCAT_GAMEFILE_SELECTOR(roccat_gamefile_selector_new(leadr_GAMEFILE_NUM));
	priv->polling_rate = ROCCAT_POLLING_RATE_FRAME(roccat_polling_rate_frame_new());
	priv->cpi = leadr_CPI_SELECTOR(leadr_cpi_selector_new());
	priv->sensitivity = ROCCAT_SENSITIVITY_FRAME(roccat_sensitivity_frame_new());
	priv->light_effects = leadr_LIGHT_EFFECTS_FRAME(leadr_light_effects_frame_new());
	priv->colors = leadr_COLOR_FRAME(leadr_color_frame_new());
	priv->talkfx = ROCCAT_TALKFX_SELECTOR(roccat_talkfx_selector_new());

	gtk_box_pack_start(GTK_BOX(light), GTK_WIDGET(priv->colors), TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(light), GTK_WIDGET(priv->light_effects), TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(right), GTK_WIDGET(priv->buttons), TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(right), GTK_WIDGET(priv->sensitivity), TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(right), GTK_WIDGET(priv->polling_rate), TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(right), GTK_WIDGET(priv->gamefiles), TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(left), light, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(left), GTK_WIDGET(priv->talkfx), FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(left), GTK_WIDGET(priv->cpi), TRUE, TRUE, 0);

	gtk_container_add(GTK_CONTAINER(content), left);
	gtk_container_add(GTK_CONTAINER(content), right);

	gtk_box_pack_start(GTK_BOX(profile_page), content, TRUE, TRUE, 0);

	gtk_widget_show_all(content);
	g_signal_connect(G_OBJECT(profile_page), "reset", G_CALLBACK(reset_cb), NULL);
}

static void leadr_profile_page_class_init(leadrProfilePageClass *klass) {
	g_type_class_add_private(klass, sizeof(leadrProfilePagePrivate));
}

void leadr_profile_page_set_rmp(leadrProfilePage *profile_page, leadrRmp *rmp) {
	leadrProfilePagePrivate *priv = profile_page->priv;
	if (priv->rmp)
		leadr_rmp_free(priv->rmp);
	priv->rmp = leadr_rmp_dup(rmp);
	set_from_rmp(profile_page);
}

leadrRmp *leadr_profile_page_get_rmp(leadrProfilePage *profile_page) {
	leadrProfilePagePrivate *priv = profile_page->priv;
	leadrRmp *rmp;
	rmp = leadr_rmp_dup(priv->rmp);
	update_rmp(profile_page, rmp);
	return rmp;
}
