/*
 * Simple BME280 Sensor Test for ESP32-S3
 * Prints sensor data to USB serial console
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/sys/printk.h>
#include <math.h>

/* BME280 device reference */
static const struct device *bme280_dev;

/* Initialize BME280 sensor */
static int init_bme280(void)
{
    bme280_dev = DEVICE_DT_GET(DT_NODELABEL(bme280));
    
    if (!device_is_ready(bme280_dev)) {
        printk("ERROR: BME280 device not ready\n");
        return -1;
    }
    
    printk("BME280 sensor initialized\n");
    return 0;
}

/* Read and print sensor data */
static void read_and_print_data(void)
{
    struct sensor_value temp, humidity, pressure;
    int ret;

    /* Get sensor readings */
    ret = sensor_sample_fetch(bme280_dev);
    if (ret) {
        printk("ERROR: Failed to fetch sensor data (%d)\n", ret);
        return;
    }

    /* Get individual values */
    sensor_channel_get(bme280_dev, SENSOR_CHAN_AMBIENT_TEMP, &temp);
    sensor_channel_get(bme280_dev, SENSOR_CHAN_HUMIDITY, &humidity);
    sensor_channel_get(bme280_dev, SENSOR_CHAN_PRESS, &pressure);

    /* Convert to doubles for easier printing */
    double temp_c = sensor_value_to_double(&temp);
    double humid_pct = sensor_value_to_double(&humidity);
    double press_pa = sensor_value_to_double(&pressure);
    double press_hpa = press_pa / 100.0;

    /* Print the data */
    printk("Temperature: %.2f°C\n", temp_c);
    printk("Humidity: %.1f%%\n", humid_pct);
    printk("Pressure: %.1f hPa\n", press_hpa);
    printk("---\n");
}

/* Main function */
int main(void)
{
    printk("\n=== BME280 Sensor Test ===\n");
    printk("ESP32-S3 Feather TFT Reverse\n\n");

    /* Initialize sensor */
    if (init_bme280() != 0) {
        printk("Failed to initialize BME280. Check wiring!\n");
        return -1;
    }

    printk("Starting sensor readings (every 2 seconds):\n\n");

    /* Main loop */
    while (1) {
        read_and_print_data();
        k_sleep(K_SECONDS(2));
    }

    return 0;
}