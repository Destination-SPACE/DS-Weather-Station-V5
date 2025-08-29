#!/bin/bash
# $1 = relative path to the app (e.g., apps/04-Display_Test)

APP_PATH="$1"
BUILD_DIR="build/$(basename $APP_PATH)"
APP_NAME=$(basename $APP_PATH)

# Activate virtual environment
source "${PWD}/.venv/bin/activate"

cd firmware

# Build the app
west build -b adafruit_feather_esp32s3_tft_reverse/esp32s3/procpu --sysbuild "$APP_PATH" -d "$BUILD_DIR" --pristine -- -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

# Update global compile_commands.json for IntelliSense
cp "$BUILD_DIR/$APP_NAME/compile_commands.json" build/compile_commands.json
