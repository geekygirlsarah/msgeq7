#include <Arduino.h>

// Comment out this line if you want faster results/no serial
//#define SERIAL_VALUES

int analogPin = 0; // read from multiplexer using analog input 0
int strobePin = 2; // strobe is attached to digital pin 2
int resetPin = 3; // reset is attached to digital pin 3
int spectrumValue[7]; // to hold a2d values

int ledPins[7];
int bright = 0;

int lastMode = 0;   // start here
int buttonPin1 = 12;
int buttonPin2 = 13;

int getButtonPress(int button)
{
    return digitalRead(button);
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

        Serial.println();
    }
#endif
}

void ledGrEqBrightness()
{
    for (int i = 0; i < 7; i++)
    {
        int val = spectrumValue[i] / 4;

        // if it's low, just turn it off
        if      (val <  48) { bright = 0;   }
        else if (val <  92) { bright = 64;  }
        else if (val < 128) { bright = 128; }
        else if (val < 144) { bright = 192; }
        else { bright = 256; }
        analogWrite(ledPins[i], bright);
    }
}

void ledGrEqBass()
{
  	// Get bass value
  	double val = spectrumValue[0] + spectrumValue[1];
  	double percent = val / 600.0;
  	int bass = percent * 100.0;

#ifdef SERIAL_VALUES
    Serial.print("Val=");
    Serial.print(val);
    Serial.print("   Percent=");
    Serial.print(percent);
    Serial.print("   Bass=");
    Serial.print(bass);
#endif

  	for (int i = 0; i < 7; i++)
  	{
  	   digitalWrite(ledPins[i], LOW);
  	}
  	if (bass >  7) { digitalWrite(ledPins[0], HIGH); }
  	if (bass > 14) { digitalWrite(ledPins[1], HIGH); }
  	if (bass > 28) { digitalWrite(ledPins[2], HIGH); }
  	if (bass > 42) { digitalWrite(ledPins[3], HIGH); }
  	if (bass > 57) { digitalWrite(ledPins[4], HIGH); }
  	if (bass > 71) { digitalWrite(ledPins[5], HIGH); }
  	if (bass > 85) { digitalWrite(ledPins[6], HIGH); }
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
    for (int i = 0; i < 7; i++)
    {
        ledPins[i] = 5+i;
        pinMode(ledPins[i], OUTPUT);
        digitalWrite(ledPins[i], LOW);
    }
}

void loop()
{
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
