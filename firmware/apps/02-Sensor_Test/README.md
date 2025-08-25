# BME280 Sensor Application

Reads temperature, humidity, and pressure from BME280 sensor and displays on TFT.

## Building
```bash
cd apps/bme280-sensor
west build -b adafruit_feather_esp32s3_reverse_tft
west flash