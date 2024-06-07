#include <Arduino.h>
#include <LedControl.h>

// Pin connections to the MAX7219
const int DIN_PIN = 12;
const int CS_PIN = 10;
const int CLK_PIN = 11;

// Initialize LedControl object
LedControl lc = LedControl(DIN_PIN, CLK_PIN, CS_PIN, 1);

const int maxFrames = 10; // Adjust as necessary
byte frames[maxFrames][8];
int numFrames = 0;

void readFramesFromSerial()
{
  numFrames = 0;
  while (Serial.available())
  {
    String line = Serial.readStringUntil('\n');
    line.trim(); // Remove any leading or trailing whitespace

    Serial.println("Received line: " + line); // Debugging message

    if (line.startsWith("0") || line.startsWith("1"))
    {
      int pos = 0;
      while (pos < line.length() && numFrames < maxFrames)
      {
        for (int y = 0; y < 8; y++)
        {
          int nextSpace = line.indexOf(' ', pos);
          if (nextSpace == -1)
            nextSpace = line.length();
          String byteStr = line.substring(pos, nextSpace);
          byteStr.trim(); // Remove any extra spaces
          frames[numFrames][y] = strtol(byteStr.c_str(), NULL, 2);
          pos = nextSpace + 1;                       // Move to the next byte
          Serial.println("Parsed byte: " + byteStr); // Debugging message
        }
        numFrames++;
      }
      Serial.println("Number of frames: " + String(numFrames)); // Debugging message
    }
  }
}

void displayFrames()
{
  for (int frame = 0; frame < numFrames; frame++)
  {
    for (int row = 0; row < 8; row++)
    {
      lc.setRow(0, row, frames[frame][row]);
    }
    delay(500); // Display each frame for 500 milliseconds
  }
}

void setup()
{
  Serial.begin(9600);
  lc.shutdown(0, false); // Wake up displays
  lc.setIntensity(0, 1); // Set brightness level (0 is min, 15 is max)
  lc.clearDisplay(0);    // Clear display register
}

void loop()
{
  if (Serial.available())
  {
    readFramesFromSerial();
    displayFrames();
  }
}
