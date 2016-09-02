#include <Arduino.h>

// Comment out this line if you want faster results/no serial
//#define SERIAL_VALUES

int analogPin = 0; // read from multiplexer using analog input 0
int strobePin = 2; // strobe is attached to digital pin 2
int resetPin = 3; // reset is attached to digital pin 3
int spectrumValue[7]; // to hold a2d values

int redPins[8];
int greenPins[8];
int bluePins[8];
int bright = 0;

int lastMode = 0;   // start here
int buttonPin1 = 12;
int buttonPin2 = 13;

double maxBass = 0;

int getButtonPress(int button)
{
    return digitalRead(button);
}

void clearLeds()
{
  for (int i = 0; i < 8; i++)
  {
    digitalWrite(redPins[i], LOW);
    digitalWrite(greenPins[i], LOW);
    digitalWrite(bluePins[i], LOW);
  }
}

void getValues()
{
    digitalWrite(resetPin, HIGH);
    digitalWrite(resetPin, LOW);
    for (int i = 0; i < 7; i++)
    {
        digitalWrite(strobePin, LOW);
        delayMicroseconds(30); // to allow the output to settle
        spectrumValue[i] = analogRead(analogPin);
        digitalWrite(strobePin, HIGH);
    }
}

void printSerialValues() {
#ifdef SERIAL_VALUES
    for (int i = 0; i < 7; i++) {
        // comment out/remove the serial stuff to go faster
        // - its just here for show
        if (spectrumValue[i] < 10)
        {
            Serial.print(" ");
            Serial.print(spectrumValue[i]);
        }
        else if (spectrumValue[i] < 100 )
        {
            Serial.print(" ");
            Serial.print(spectrumValue[i]);
        }
        else
        {
            Serial.print(" ");
            Serial.print(spectrumValue[i]);
        }
    }
    Serial.println();
#endif
}

void ledGrEqBrightness()
{
    for (int i = 0; i < 7; i++)
    {
        int val = spectrumValue[i] / 4;

        // if it's low, just turn it off
        if      (val <  48) { bright = 0;   }
        else if (val <  96) { bright = 48;  }
        else if (val < 128) { bright = 64;  }
        else if (val < 192) { bright = 128; }
        else if (val < 224) { bright = 192; }
        else { bright = 256; }
        analogWrite(redPins[i], bright);
        digitalWrite(greenPins[i], LOW);
        digitalWrite(bluePins[i], LOW);
    }
}

void ledGrEqBass()
{
  	// Get bass value
  	double val = spectrumValue[0] + spectrumValue[1];

    if (val > maxBass)
    {
      maxBass = val;
    }

  	double percent = val / maxBass;
  	int bass = percent * 100.0;

#ifdef SERIAL_VALUES
    Serial.print("Val=");
    Serial.print(val);
    Serial.print("   Percent=");
    Serial.print(percent);
    Serial.print("   Bass=");
    Serial.print(bass);
    Serial.print("         ");
#endif

  	if (bass > 20) {
      digitalWrite(greenPins[0], HIGH);
    }
  	if (bass > 30) {
      digitalWrite(greenPins[1], HIGH);
    }
  	if (bass > 40) {
      digitalWrite(greenPins[2], HIGH);
    }
  	if (bass > 50) {
      digitalWrite(redPins[3], HIGH);
      digitalWrite(greenPins[3], HIGH);
    }
  	if (bass > 60) {
      digitalWrite(redPins[4], HIGH);
      digitalWrite(greenPins[4], HIGH);
    }
  	if (bass > 70) {
      digitalWrite(redPins[5], HIGH);
      digitalWrite(greenPins[5], HIGH);
    }
    if (bass > 80) {
      digitalWrite(redPins[6], HIGH);
    }
    if (bass > 90) {
      digitalWrite(redPins[7], HIGH);
    }
}


void setup()
{
    Serial.begin(9600);

    pinMode(analogPin, INPUT);
    pinMode(strobePin, OUTPUT);
    pinMode(resetPin, OUTPUT);
    analogReference(DEFAULT);

    digitalWrite(resetPin, LOW);
    digitalWrite(strobePin, HIGH);

    // init LEDs
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
    clearLeds();
    getValues();
    printSerialValues();

    if (getButtonPress(buttonPin1) == HIGH)
    {
        lastMode = 0;
    }
    if (getButtonPress(buttonPin2) == HIGH)
    {
        lastMode = 1;
    }

    if (lastMode)
    {
      ledGrEqBass();
    }
    else
    {
      ledGrEqBrightness();
    }
}
