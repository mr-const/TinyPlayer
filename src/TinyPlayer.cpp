#include <Arduino.h>
#ifdef ESP32
  #include <WiFi.h>
  #include "SPIFFS.h"
#else
  #include <ESP8266WiFi.h>
#endif
#include "AudioFileSourceSPIFFS.h"
#include "AudioFileSourceID3.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"

#include "FileList.h"

// To run, set your ESP8266 build to 160MHz, and include a SPIFFS of 512KB or greater.
// Use the "Tools->ESP8266/ESP32 Sketch Data Upload" menu to write the MP3 to SPIFFS
// Then upload the sketch normally.  

// pno_cs from https://ccrma.stanford.edu/~jos/pasp/Sound_Examples.html

AudioGeneratorMP3 *mp3;
AudioFileSourceSPIFFS *file;
AudioOutputI2S *out;
AudioFileSourceID3 *id3;


// Called when a metadata event occurs (i.e. an ID3 tag, an ICY block, etc.
void MDCallback(void *cbData, const char *type, bool isUnicode, const char *string)
{
  (void)cbData;
  Serial.printf("ID3 callback for: %s = '", type);

  if (isUnicode) {
    string += 2;
  }
  
  while (*string) {
    char a = *(string++);
    if (isUnicode) {
      string++;
    }
    Serial.printf("%c", a);
  }
  Serial.printf("'\n");
  Serial.flush();
}


void setup()
{
  WiFi.mode(WIFI_OFF);
  WiFi.forceSleepBegin();
  delay(1);

  Serial.begin(115200);
  Serial.println("Starting TinyPlayer");
  SPIFFS.begin();

  dumpDirTree();

  Serial.printf("Preparing MP3...\n");

  audioLogger = &Serial;
  file = new AudioFileSourceSPIFFS("track.mp3");
  id3 = new AudioFileSourceID3(file);
  id3->RegisterMetadataCB(MDCallback, (void*)"ID3TAG");
  out = new AudioOutputI2S();
  mp3 = new AudioGeneratorMP3();
  mp3->begin(id3, out);
}

void loop()
{
  if (mp3->isRunning()) {
    if (!mp3->loop())
    {
      Serial.printf("MP3 done, restarting\n");
      id3->seek(SEEK_SET, 0);
      delay(1000);
    }
  } else {
    Serial.println("MP3 exited");
    delay(1000);
  }
}
