/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/sensor_data_types.h>
#include <zephyr/rtio/rtio.h>
#include <zephyr/dsp/print_format.h>
#include <zephyr/sys/printk.h>

#define TFT_I2C_POWER_PIN 7

static const struct device *gpio_dev;

/*
 * Get a device structure from a devicetree node with compatible
 * "bosch,bme280". (If there are multiple, just pick one.)
 */
const struct device *const dev = DEVICE_DT_GET_ANY(bosch_bme280);

SENSOR_DT_READ_IODEV(iodev, DT_COMPAT_GET_ANY_STATUS_OKAY(bosch_bme280),
		{SENSOR_CHAN_AMBIENT_TEMP, 0},
		{SENSOR_CHAN_HUMIDITY, 0},
		{SENSOR_CHAN_PRESS, 0});

RTIO_DEFINE(ctx, 1, 1);

/* Enable TFT I2C regulator on GPIO7 */
static int enable_tft_i2c_power(void)
{
    int ret;
    
    /* Get GPIO device */
    gpio_dev = DEVICE_DT_GET(DT_NODELABEL(gpio0));
    if (!device_is_ready(gpio_dev)) {
        printk("ERROR: GPIO device not ready\n");
        return -1;
    }
    
    /* Configure GPIO7 as output */
    ret = gpio_pin_configure(gpio_dev, TFT_I2C_POWER_PIN, GPIO_OUTPUT);
    if (ret < 0) {
        printk("ERROR: Failed to configure GPIO7: %d\n", ret);
        return ret;
    }
    
    /* Set GPIO7 HIGH to enable TFT I2C power */
    ret = gpio_pin_set(gpio_dev, TFT_I2C_POWER_PIN, 1);
    if (ret < 0) {
        printk("ERROR: Failed to set GPIO7 high: %d\n", ret);
        return ret;
    }
    
    printk("TFT I2C power enabled on GPIO7\n");
    
    /* Wait a bit for power to stabilize */
    k_msleep(100);
    
    return 0;
}

static const struct device *check_bme280_device(void)
{
	if (dev == NULL) {
		/* No such node, or the node does not have status "okay". */
		printk("\nError: no device found.\n");
		return NULL;
	}

	if (!device_is_ready(dev)) {
		printk("\nError: Device \"%s\" is not ready; "
		       "check the driver initialization logs for errors.\n",
		       dev->name);
		return NULL;
	}

	printk("Found device \"%s\", getting sensor data\n", dev->name);
	return dev;
}

int main(void)
{
    /* Add delay and multiple init messages to help with debugging */
    k_msleep(1000);
    
    printk("\n\n=== BME280 Sensor Test STARTING ===\n");
    printk("ESP32-S3 Feather TFT Reverse\n");
    printk("Zephyr Build: %s\n", __DATE__);
    printk("===================================\n\n");

    /* Enable TFT I2C power regulator first */
    printk("Enabling TFT I2C power regulator...\n");
    if (enable_tft_i2c_power() != 0) {
        printk("FAILED to enable TFT I2C power!\n");
        printk("BME280 may not work without power...\n");
    }

	const struct device *dev = check_bme280_device();

	if (dev == NULL) {
		return 0;
	}

	while (1) {
		uint8_t buf[128];

		int rc = sensor_read(&iodev, &ctx, buf, 128);

		if (rc != 0) {
			printk("%s: sensor_read() failed: %d\n", dev->name, rc);
			return rc;
		}

		const struct sensor_decoder_api *decoder;

		rc = sensor_get_decoder(dev, &decoder);

		if (rc != 0) {
			printk("%s: sensor_get_decode() failed: %d\n", dev->name, rc);
			return rc;
		}

		uint32_t temp_fit = 0;
		struct sensor_q31_data temp_data = {0};

		decoder->decode(buf,
			(struct sensor_chan_spec) {SENSOR_CHAN_AMBIENT_TEMP, 0},
			&temp_fit, 1, &temp_data);

		uint32_t press_fit = 0;
		struct sensor_q31_data press_data = {0};

		decoder->decode(buf,
				(struct sensor_chan_spec) {SENSOR_CHAN_PRESS, 0},
				&press_fit, 1, &press_data);

		uint32_t hum_fit = 0;
		struct sensor_q31_data hum_data = {0};

		decoder->decode(buf,
				(struct sensor_chan_spec) {SENSOR_CHAN_HUMIDITY, 0},
				&hum_fit, 1, &hum_data);

		printk("temp: %s%d.%d; press: %s%d.%d; humidity: %s%d.%d\n",
			PRIq_arg(temp_data.readings[0].temperature, 6, temp_data.shift),
			PRIq_arg(press_data.readings[0].pressure, 6, press_data.shift),
			PRIq_arg(hum_data.readings[0].humidity, 6, hum_data.shift));

		k_msleep(1000);
	}
	return 0;
}