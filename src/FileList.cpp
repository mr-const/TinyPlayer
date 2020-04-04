#include <Arduino.h>
#include <FS.h>

void dumpDirTree()
{
    fs::FSInfo info;
    SPIFFS.info(info);

    Serial.println(String("SPIFFS used ") + info.usedBytes + F(" of ") + info.totalBytes + F(" bytes"));

    Serial.println("Dumping file list");
    fs::Dir dir = SPIFFS.openDir("");
    int count = -1;
    int startIdx = 0;

    while (dir.next())
    {
        ++count;

        if (count < startIdx) continue;

        Serial.println(String(dir.fileName()));
        fs::File f = dir.openFile("r");
        if (f)
        {
            Serial.println(String(f.size()));
            f.close();
        }
    }
}