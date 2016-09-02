#include <Arduino.h>

int redPins[8];
int greenPins[8];
int bluePins[8];
bool redValues[8];
bool greenValues[8];
bool blueValues[8];

void setup()
{
  for (int i = 0; i < 8; i++)
  {
    pinMode(4+i, OUTPUT);
    pinMode(23+(3*i), OUTPUT);
    pinMode(24+(3*i), OUTPUT);

    digitalWrite(4+i, LOW);
    digitalWrite(23+(3*i), LOW);
    digitalWrite(24+(3*i), LOW);

    redPins[i] = 4+i;   // on PWMs
    greenPins[i] = 23+(3*i); // still on old pins
    bluePins[i] = 24+(3*i);  // still on old pins
  }
}

void loop()
{
  // for (int num = 0; num < 2; num++)
  // {
    // Loop for each color
    for (int i = 0; i < 8; i++)
    {
      // Loop for each LED
      for (int j = 0; j < 8; j++)
      {
        // Whether to turn r/g/b on or off
        int r = i & 1;
        int g = i & 2;
        int b = i & 4;

        // Turn it on or off
        digitalWrite(redPins[j], r ? HIGH : LOW);
        digitalWrite(greenPins[j], g ? HIGH : LOW);
        digitalWrite(bluePins[j], b ? HIGH : LOW);
      }
      // After all LEDs set, wait a second
      delay(250);
    }
  // }

  // clear
  for (int i = 0; i < 8; i++)
  {
    digitalWrite(redPins[i], LOW);
    digitalWrite(greenPins[i], LOW);
    digitalWrite(bluePins[i], LOW);
    redValues[i] = 0;
    greenValues[i] = 0;
    blueValues[i] = 0;
  }

  int curColor = 0;
  while (curColor < 24)
  {
    curColor++;

    // shift colors
    for (int i = 7; i > 0; i--)
    {
      redValues[i] = redValues[i-1];
      greenValues[i] = greenValues[i-1];
      blueValues[i] = blueValues[i-1];
    }

    // Calculate new color
    if (curColor <= 7)
    {
      // Forward
      redValues[0] = curColor & 1;
      greenValues[0] = curColor & 2;
      blueValues[0] = curColor & 4;
    }
    else if (curColor <= 15)
    {
      // Backward (still works as 8 is ignored)
      int color = 15 - curColor;
      redValues[0] = color & 4;
      greenValues[0] = color & 2;
      blueValues[0] = color & 1;
    }
    else
    {
      // Clear
      redValues[0] = 0;
      greenValues[0] = 0;
      blueValues[0] = 0;
    }

    for (int i = 0; i < 8; i++)
    {
      digitalWrite(redPins[i], redValues[i] ? HIGH : LOW);
      digitalWrite(greenPins[i], greenValues[i] ? HIGH : LOW);
      digitalWrite(bluePins[i], blueValues[i] ? HIGH : LOW);
    }
    delay(1000);
  }

  // Die
  while(true)
  {
    ;
  }
}
