# Sensor Test Application

Reads temperature, humidity, and pressure from BME280 sensor.

## Building
```bash
cd apps/02-Sensor_Test
west build --pristine -b adafruit_feather_esp32s3_tft_reverse/esp32s3/procpu
west flash