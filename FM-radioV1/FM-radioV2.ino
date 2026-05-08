#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <uRTCLib.h>
#include <EEPROM.h>

#define CLK   4
#define DT    3
#define SW    2
#define SCALE 5

#define MIN_FREQUENCY 760
#define MAX_FREQUENCY 1080

LiquidCrystal_I2C lcd(0x27, 20, 4);
uRTCLib rtc(0x68);

String StationName = "----------";
int frequency_counter;
int CurrentState;
int LastState;
unsigned long lastClockUpdate = 0;
int address = 0;

byte needleChar[8];
byte scaleChar[8] = {
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00100,
    B10101,
    B10101
};

void clearLine(int column, int row) {
    lcd.setCursor(column, row);
    for (; column < 20; column++) {
        lcd.print(" ");
    }
}

void printInfoOnLCD() {
    clearLine(0, 0);
    lcd.setCursor((20 - StationName.length()) / 2, 0);
    lcd.print(StationName);

    lcd.setCursor(1, 1);
    for (int i = 0; i < 18; i++) {
        lcd.write(SCALE);
    }

    int pixelPos    = map(frequency_counter, MIN_FREQUENCY, MAX_FREQUENCY, 0, 89);
    int charIndex   = pixelPos / 5;
    int pixelOffset = pixelPos % 5;
    lcd.setCursor(1 + charIndex, 1);
    lcd.write(pixelOffset);

    lcd.setCursor(6, 2);
    lcd.print(String(frequency_counter / 10.0, 1) + " MHz");
}

void printInfoOnSerial() {
    Serial.print("Frequency: ");
    Serial.println(frequency_counter);
}

void clockInfo() {
    rtc.refresh();

    char dateBuffer[11];
    char timeBuffer[9];
    sprintf(dateBuffer, "%02d/%02d/%02d", rtc.day(), rtc.month(), rtc.year());
    sprintf(timeBuffer, "%02d:%02d:%02d", rtc.hour(), rtc.minute(), rtc.second());

    lcd.setCursor(0, 3);
    lcd.print(dateBuffer);
    lcd.print("    ");
    lcd.print(timeBuffer);
}

void setup() {
    Serial.begin(9600);
    Wire.begin();

    pinMode(CLK, INPUT);
    pinMode(DT,  INPUT);

    lcd.init();
    lcd.backlight();

    URTCLIB_WIRE.begin();
    // Uncomment once to set the time, then comment out again:
    // rtc.set(0, 02, 14, 7, 19, 7, 25);

    EEPROM.get(address, frequency_counter);
    if (frequency_counter < MIN_FREQUENCY || frequency_counter > MAX_FREQUENCY) {
        frequency_counter = 920;
    }

    lcd.createChar(SCALE, scaleChar);
    for (int col = 0; col < 5; col++) {
        for (int row = 0; row < 8; row++) {
            needleChar[row] = scaleChar[row] | (0b10000 >> col);
        }
        lcd.createChar(col, needleChar);
    }

    printInfoOnSerial();
    printInfoOnLCD();
    LastState = digitalRead(CLK);
}

void loop() {
    if (millis() - lastClockUpdate >= 1000) {
        clockInfo();
        lastClockUpdate = millis();
    }

    CurrentState = digitalRead(CLK);
    if (CurrentState != LastState && CurrentState == HIGH) {
        if (digitalRead(DT) != CurrentState) {
            frequency_counter++;
            if (frequency_counter > MAX_FREQUENCY) frequency_counter = MIN_FREQUENCY;
        } else {
            frequency_counter--;
            if (frequency_counter < MIN_FREQUENCY) frequency_counter = MAX_FREQUENCY;
        }

        EEPROM.put(address, frequency_counter);
        delay(10);
        printInfoOnSerial();
        printInfoOnLCD();
    }

    LastState = CurrentState;
}
