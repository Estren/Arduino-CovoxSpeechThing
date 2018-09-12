/*
 - Розробив Roman
 - Канал на YouTube: https://goo.gl/x8FL2o
 - Відео з проектом:  
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
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

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
    for (uint8_t i = 0; fileSelect != i; ) { 
        entry =  root.openNextFile();
        if (!entry) {
            break;
        }
        if (((String)entry.name()).endsWith(".WAV")) {
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

    // read from the file until there's nothing else in it:
    while (root.available()) {
      PORTD = root.read();
      for(int i = 0; i < 100; i++)
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



