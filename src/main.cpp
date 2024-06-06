#include <Arduino.h>
#include <LedControl.h>
#include "../aseprite/frames.h"

// Pin connections to the MAX7219
const int DIN_PIN = 12;
const int CS_PIN = 10;
const int CLK_PIN = 11;

// Initialize LedControl object
LedControl lc = LedControl(DIN_PIN, CLK_PIN, CS_PIN, 1);

// Macro to calculate the number of frames
#define NUM_FRAMES (sizeof(frames) / sizeof(frames[0]))

int currentFrame = 0;
unsigned long previousMillis = 0;
const long interval = 500; // Time between frames (in milliseconds)

void displayFrame(int frame)
{
  for (int row = 0; row < 8; row++)
  {
    lc.setRow(0, row, frames[frame][row]);
  }
}

void setup()
{
  lc.shutdown(0, false); // Wake up displays
  lc.setIntensity(0, 1); // Set brightness level (0 is min, 15 is max)
  lc.clearDisplay(0);    // Clear display register
}

void loop()
{
  unsigned long currentMillis = millis();

  // Check if it's time to update the frame
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    displayFrame(currentFrame);
    currentFrame = (currentFrame + 1) % NUM_FRAMES; // Move to the next frame
  }
}
