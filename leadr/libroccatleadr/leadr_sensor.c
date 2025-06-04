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

#include "leadr_sensor.h"
#include "leadr_device.h"

typedef struct _leadrSensor leadrSensor;

struct _leadrSensor {
	guint8 report_id; /* leadr_REPORT_ID_SENSOR */
	guint8 action;
	guint8 reg;
	guint8 value;
} __attribute__ ((packed));

typedef enum {
	leadr_SENSOR_ACTION_WRITE = 1,
	leadr_SENSOR_ACTION_READ = 2,
	leadr_SENSOR_ACTION_FRAME_CAPTURE = 3,
} leadrSensorAction;

static leadrSensor *leadr_sensor_read(RoccatDevice *device, GError **error) {
	return (leadrSensor *)leadr_device_read(device, leadr_REPORT_ID_SENSOR, sizeof(leadrSensor), error);
}

static gboolean leadr_sensor_write(RoccatDevice *device, leadrSensor *sensor, GError **error) {
	return leadr_device_write(device, (gchar const *)sensor, sizeof(leadrSensor), error);
}

leadrSensorImage *leadr_sensor_image_read(RoccatDevice *device, GError **error) {
	return (leadrSensorImage *)leadr_device_read(device, leadr_REPORT_ID_SENSOR, sizeof(leadrSensorImage), error);
}

static gboolean leadr_sensor_write_struct(RoccatDevice *device, guint8 action, guint8 reg, guint8 value, GError **error) {
	leadrSensor sensor;

	sensor.report_id = leadr_REPORT_ID_SENSOR;
	sensor.action = action;
	sensor.reg = reg;
	sensor.value = value;

	return leadr_sensor_write(device, &sensor, error);
}

gboolean leadr_sensor_write_register(RoccatDevice *device, guint8 reg, guint8 value, GError **error) {
	return leadr_sensor_write_struct(device, leadr_SENSOR_ACTION_WRITE, reg, value, error);
}

guint8 leadr_sensor_read_register(RoccatDevice *device, guint8 reg, GError **error) {
	leadrSensor *sensor;
	guint8 result;

	leadr_sensor_write_struct(device, leadr_SENSOR_ACTION_READ, reg, 0, error);
	if (*error)
		return 0;

	sensor = leadr_sensor_read(device, error);
	if (*error)
		return 0;
	result = sensor->value;
	g_free(sensor);

	return result;
}

gboolean leadr_sensor_calibrate_step(RoccatDevice *device, GError **error) {
	return leadr_sensor_write_struct(device, leadr_SENSOR_ACTION_FRAME_CAPTURE, 1, 0, error);
}

guint leadr_sensor_image_get_median(leadrSensorImage const *image) {
	guint i;
	gulong sum = 0;
	for (i = 0; i < leadr_SENSOR_IMAGE_SIZE * leadr_SENSOR_IMAGE_SIZE; ++i)
		sum += image->data[i];

	return sum / (leadr_SENSOR_IMAGE_SIZE * leadr_SENSOR_IMAGE_SIZE);
}

