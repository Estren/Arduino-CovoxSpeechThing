/*
 - Розробив Roman
 - Канал на YouTube: https://goo.gl/x8FL2o
 - Відео з проектом: https://youtu.be/8AksFNU6aIg
 - Після перегляду відео “386й ноутбук возвращается: Звук!” https://youtu.be/3rtj_jx8bck, я вирішив спробувати об’єднати Covox Speech Thing з Arduino
*/

#include <SPI.h>
#include <SD.h>

uint8_t fileCount = 0;
uint8_t fileSelect = 0;

void setup() {
  pinMode(8, INPUT);
  // Open serial communications and wait for port to open:
  Serial.begin(9600);  
  Serial.print("Initializing SD card...");

  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  printDirectory();
  if (fileCount == 0) {
    Serial.println("Files not found");
    }
  Serial.println("done!");
}

void loop() {
  while(fileCount != 0) {
    openFile(getFileNames());
    fileSelect++;
    if (fileSelect > (fileCount - 1)) {
    fileSelect = 0;
    }
    }
}

String getFileNames() {
    File entry;
    File root = SD.open("/");
    root.rewindDirectory(); 
    String FileName = "";
    for (uint8_t i = 0; fileSelect != (i - 1); ) { 
        entry =  root.openNextFile();
        if (!entry) {
            break;
        }
        if (((String)entry.name()).endsWith(".WAV") and !entry.isDirectory()) {
            FileName = entry.name();
            ++i;
        }
        entry.close();
    }
    root.close();
    return FileName;    
}

void openFile(String FileName) {
  File root = SD.open(FileName);
  if (root) {
    Serial.print(FileName);
    Serial.println(" is opened");
   
    // Define buffer for presumably higher reading speeds
    const unsigned char BUFFER_SIZE = 100;
    uint8_t buffer[BUFFER_SIZE];
    // read from the file until there's nothing else in it:
    while (root.available()) {
      buffer = root.read(buffer, BUFFER_SIZE);
      for (int i = 0; i < BUFFER_SIZE - 1; i++)
      {
          PORTD = buffer[i];
          for(int j = 0; j < 89; j++)
              __asm("nop");
      }
      // Write last byte (may require different delay)
      PORTD = buffer[BUFFER_SIZE - 1];
      for(int j = 0; j < 89; j++)
          __asm("nop");
      
      if (digitalRead(8) == LOW) {
        delay(300);  
        root.close();
        return;
       }
    }
    // close the file:
    root.close();
  } else {
    // if the file didn't open, print an error:
    Serial.print("Error opening ");
    Serial.println(FileName);
  }
  }

void printDirectory() {
  File root = SD.open("/");
  while (true) {
    File entry =  root.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    if (((String)entry.name()).endsWith(".WAV") and !entry.isDirectory()) {
    fileCount++;
    Serial.println(entry.name());
    }
    entry.close();
  }
  root.close();
}
