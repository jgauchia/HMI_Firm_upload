#include <Arduino.h>
#include "FS.h"
#include "SPIFFS.h"

void setup()
{
  if (!SPIFFS.begin(true))
    log_i("SPIFFS Mount Failed");
  else
    log_i("SPIFFS Mounted");
  File file = SPIFFS.open("/mk4duo.tft");
  size_t filesize = file.size();
  Serial.begin(115200);
  Serial.flush();
  delay(1000);
  Serial.println("boot");
  Serial2.begin(115200, SERIAL_8N1, 18, 17);
  delay(500);
  Serial2.printf("whmi-wri %d,115200,0", filesize);
  Serial2.write(0xFF);
  Serial2.write(0xFF);
  Serial2.write(0xFF);
  delay(500);
  Serial2.write(0xFF);
  Serial2.write(0xFF);
  Serial2.write(0xFF);
  delay(500);

  char buf[4096];
  size_t avail;
  size_t readcount;

  while (filesize > 0)
  {
    avail = file.available();
    if (avail)
    {
      readcount = file.readBytes(buf, avail > 4096 ? 4096 : avail);
      Serial2.write(buf, readcount);
      filesize -= readcount;

      delay(500);
    }
    delay(1);
  }

  file.close();

  while (Serial2.available() > 0)
  {
    Serial.write(Serial2.read());
  }
}

void loop()
{
}
