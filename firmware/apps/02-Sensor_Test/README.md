# Sensor Test Application

Reads temperature, humidity, and pressure from BME280 sensor.

## Building
```bash
source ~/zephyrproject/.venv/bin/activate
cd apps/02-Sensor_Test
west build --pristine -b adafruit_feather_esp32s3_tft_reverse/esp32s3/procpu --sysbuild
west flash --esp-device /dev/ttyACM0