/*******************************************************************************
DESTINATION WEATHER FEATHER WING EXAMPLE - SD FAT TEST

This is an example sketch for the Destination Weather FeatherWing remote sensing
platform to test the microSD card reader.

modified 2023-12-17
by Madison Gleydura

MIT LICENSE AGREEMENT

Copyright © 2023 Destination SPACE Inc.

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the “Software”), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*******************************************************************************/

#include <Arduino.h>
#include <SPI.h>
#include <SdFat.h>
#include <Adafruit_TinyUSB.h>

const int CHIP_SELECT = 5;
const int CARD_DETECT = 6;

SdFat sd;
SdFile root;
SdFile file;

Adafruit_USBD_MSC usb_msc;

bool fs_changed;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(CARD_DETECT, INPUT);

  usb_msc.setID("DSPACE", "SD Card", "1.0");
  usb_msc.setReadWriteCallback(msc_read_cb, msc_write_cb, msc_flush_cb);
  usb_msc.setUnitReady(false);
  usb_msc.begin();

  Serial.begin(115200);
  while(!Serial){
    delay(10);
  }

  char buffer[256];
  sprintf(buffer, "\n+===============================================================+\n|      Destination Weather FeatherWing - microSD Card Test      |\n+===============================================================+");
  Serial.print(buffer);

  if(CARD_DETECT == HIGH){
    Serial.print("\n\033[48;5;1mCard not Inserted\033[0m");
    while(CARD_DETECT == HIGH){
      delay(10);
    }
  }

  Serial.print("\nInitializing SD card ... ");

  if(!sd.begin(, SD_SCK_MHZ(50))){
    Serial.print("\n\033[48;5;1mInitialization Failed\033[0m");
    while(1){
      delay(10);
    }
  }

  #if SD_FAT_VERSION >= 20000
    uint32_t block_count = sd.card()->sectorCount();
  #else
    uint32_t block_count = sd.card()->cardSize();
  #endif

  char buffer[64];
  sprintf(buffer, "\nVolume size (MB):\t%7.2f", block_count / 2048);
  Serial.print(buffer);

  usb_msc.setCapacity(block_count, 512);
  usb_msc.setUnitReady(true);
  fs_changed = true;
}

void loop()
{
  if(fs_changed)
  {
    root.open("/");
    Serial.println("SD contents:");

    while(file.openNext(&root, O_RDONLY)){
      file.printFileSize(&Serial);
      Serial.write(' ');
      file.printName(&Serial);
      if(file.isDir()){
        Serial.write('/');
      }
      Serial.println();
      file.close();
    }

    root.close();

    Serial.println();

    fs_changed = false;
    delay(500); // refresh every 0.5 second
  }
}

int32_t msc_read_cb (uint32_t lba, void* buffer, uint32_t bufsize){
  bool rc;

  #if SD_FAT_VERSION >= 20000
    rc = sd.card()->readSectors(lba, (uint8_t*) buffer, bufsize/512);
  #else
    rc = sd.card()->readBlocks(lba, (uint8_t*) buffer, bufsize/512);
  #endif

  return rc ? bufsize : -1;
}

int32_t msc_write_cb (uint32_t lba, uint8_t* buffer, uint32_t bufsize){
  bool rc;

  digitalWrite(LED_BUILTIN, HIGH);

  #if SD_FAT_VERSION >= 20000
    rc = sd.card()->writeSectors(lba, buffer, bufsize/512);
  #else
    rc = sd.card()->writeBlocks(lba, buffer, bufsize/512);
  #endif

  return rc ? bufsize : -1;
}

void msc_flush_cb (void){
  #if SD_FAT_VERSION >= 20000
    sd.card()->syncDevice();
  #else
    sd.card()->syncBlocks();
  #endif

  sd.cacheClear();

  fs_changed = true;

  digitalWrite(LED_BUILTIN, LOW);
}