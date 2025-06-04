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

#include "leadr_cpi_selector.h"
#include "roccat_cpi_selector.h"
#include "leadr.h"

#define leadr_CPI_SELECTOR_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), leadr_CPI_SELECTOR_TYPE, leadrCpiSelectorClass))
#define IS_leadr_CPI_SELECTOR_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), leadr_CPI_SELECTOR_TYPE))
#define leadr_CPI_SELECTOR_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), leadr_CPI_SELECTOR_TYPE, leadrCpiSelectorPrivate))

typedef struct _leadrCpiSelectorClass leadrCpiSelectorClass;

struct _leadrCpiSelector {
	RoccatCpiSelector parent;
};

struct _leadrCpiSelectorClass {
	RoccatCpiSelectorClass parent_class;
};

G_DEFINE_TYPE(leadrCpiSelector, leadr_cpi_selector, ROCCAT_CPI_SELECTOR_TYPE);

void leadr_cpi_selector_set_from_rmp(leadrCpiSelector *selector, leadrRmp *rmp) {
	RoccatCpiSelector *roccat_selector = ROCCAT_CPI_SELECTOR(selector);
	guint i;

	roccat_cpi_selector_set_selected(roccat_selector, leadr_rmp_get_cpi_x(rmp));
	roccat_cpi_selector_set_all_active(roccat_selector, leadr_rmp_get_cpi_all(rmp));
	for (i = 0; i < leadr_PROFILE_SETTINGS_CPI_LEVELS_NUM; ++i)
		roccat_cpi_selector_set_value(roccat_selector, i, (gdouble)leadr_rmp_get_cpi_level_x(rmp, i) * 200.0);
}

void leadr_cpi_selector_update_rmp(leadrCpiSelector *selector, leadrRmp *rmp) {
	RoccatCpiSelector *roccat_selector = ROCCAT_CPI_SELECTOR(selector);
	guint i;

	leadr_rmp_set_cpi_x(rmp, roccat_cpi_selector_get_selected(roccat_selector));
	//leadr_rmp_set_cpi_y(rmp, roccat_cpi_selector_get_selected(roccat_selector));
	leadr_rmp_set_cpi_all(rmp, roccat_cpi_selector_get_all_active(roccat_selector));
	for (i = 0; i < leadr_PROFILE_SETTINGS_CPI_LEVELS_NUM; ++i) {
		leadr_rmp_set_cpi_level_x(rmp, i, (guint)((gdouble)roccat_cpi_selector_get_value(roccat_selector, i) / 200.0));
		//leadr_rmp_set_cpi_level_y(rmp, i, (guint)((gdouble)roccat_cpi_selector_get_value(roccat_selector, i) / 200.0));
	}
}

GtkWidget *leadr_cpi_selector_new(void) {
	leadrCpiSelector *cpi_selector;

	cpi_selector = leadr_CPI_SELECTOR(g_object_new(leadr_CPI_SELECTOR_TYPE,
			"count", leadr_PROFILE_SETTINGS_CPI_LEVELS_NUM,
			"min", (gdouble)leadr_CPI_MIN,
			"max", (gdouble)leadr_CPI_MAX,
			"step", (gdouble)leadr_CPI_STEP,
			NULL));

	return GTK_WIDGET(cpi_selector);
}

static void leadr_cpi_selector_init(leadrCpiSelector *cpi_selector) {}

static void leadr_cpi_selector_class_init(leadrCpiSelectorClass *klass) {}
