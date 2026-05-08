#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <TEA5767Radio.h>

#define CLK 4
#define DT 3
#define SW 2
#define SKALA 5
#define Tipka1 8
#define Tipka2 9
#define Tipka3 10
#define Tipka4 11
#define Tipka5 12

TEA5767Radio radio = TEA5767Radio();
float frekvencija = 88;
int trenutnoStanjeCLK;
int zadnjeStanjeCLK;
byte status = 0;

//Niz za imena stanica
String NizStanica[] = {
  "Radio Dalmacija",     //0
  "HRT HrvatskiRadio1",  //1
  "HRT HrvatskiRadio2",  //2
  "HRT HrvatskiRadio3",  //3
  "HRT RadioSplit",      //4
  "HRT RadioZadar",      //5
  "BHRT BH Radio1",      //6
  "bravo!radio",         //7
  "Narodni radio",       //8
  "Nautic Radio",        //9
  "Radio Sunce",         //10
  "Radio Brac",          //11
  "Federalni radio",     //12
  "Otvoreni radio",      //13
  "Ultra Split",         //14
  "Megamix radioHvar",   //15
  "GradskiRadioTrogir",  //16
  "RadioMarijaVatikan",  //17
  "HrvKatolickiRadio",   //18
  "MirMedugorje",        //19
  "MakarskaRivijera",    //20
  "Jadranski Radio",     //21
};
LiquidCrystal_I2C lcd(0x27, 20, 4);

//MIJENJANJE LINEARNE SKALE
void promijeniSkalu() {
  int lcdBase = (frekvencija - 88) * 4 + 15;
  int lcdMajor = lcdBase / 5;
  int lcdMinor = lcdBase % 5;

  if (lcdMajor > 0) {
    // Igla nije na skroz lijevo
    // izbrisi prijasnju poziciju igle
    lcd.setCursor(lcdMajor - 1, 1);
  } else
    lcd.setCursor(lcdMajor, 1);
  lcd.write(lcdMinor);
}

void setup() {

  //Za rotacijski enkoder
  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
  pinMode(SW, INPUT_PULLUP);
  //Za tipkala
  pinMode(Tipka1, INPUT_PULLUP);
  pinMode(Tipka2, INPUT_PULLUP);
  pinMode(Tipka3, INPUT_PULLUP);
  pinMode(Tipka4, INPUT_PULLUP);
  pinMode(Tipka5, INPUT_PULLUP);

  zadnjeStanjeCLK = digitalRead(CLK);
  Serial.begin(9600);
  Wire.begin();
  lcd.init();
  lcd.backlight();


  //Za nacrtat iglu na linearnoj skali na LCD ekran
  int i;
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

  lcd.createChar(SKALA, scaleChar);
  for (int j = 0; j < 5; j++) {
    for (i = 0; i < 8; i++)
      needleChar[i] = scaleChar[i] | (0b10000 >> j);
    lcd.createChar(j, needleChar);
  }
}
void loop() {
  int i;
  String NazivStanice;
  //Za mijenjanje frekvencije sa rotacijskim enkoderom
  trenutnoStanjeCLK = digitalRead(CLK);
  if (trenutnoStanjeCLK != zadnjeStanjeCLK && trenutnoStanjeCLK == 1) {
    if (digitalRead(DT) != trenutnoStanjeCLK) {
      frekvencija = frekvencija + 0.1;
      if (frekvencija > 108) {
        frekvencija = 87.8;
      }
      radio.setFrequency(frekvencija);
    } else {
      frekvencija = frekvencija - 0.1;
      if (frekvencija < 87.8) {
        frekvencija = 108;
      }
      radio.setFrequency(frekvencija);
    }
    //Za napisat imena stanica na ekran
    int frek = frekvencija * 10;
    if (frek == 878) {
      NazivStanice = NizStanica[0];
    }
    if (frek == 880) {
      NazivStanice = NizStanica[1];
    }
    if (frek == 881) {
      NazivStanice = NizStanica[6];
    }
    if (frek == 884) {
      NazivStanice = NizStanica[4];
    }
    if (frek == 888) {
      NazivStanice = NizStanica[3];
    }
    if (frek == 893) {
      NazivStanice = NizStanica[1];
    }
    if (frek == 897) {
      NazivStanice = NizStanica[1];
    }
    if (frek == 905) {
      NazivStanice = NizStanica[9];
    }
    if (frek == 902) {
      NazivStanice = NizStanica[7];
    }
    if (frek == 909) {
      NazivStanice = NizStanica[10];
    }
    if (frek == 913) {
      NazivStanice = NizStanica[1];
    }
    if (frek == 918) {
      NazivStanice = NizStanica[11];
    }

    if (frek == 925) {
      NazivStanice = NizStanica[12];
    }
    if (frek == 928) {
      NazivStanice = NizStanica[13];
    }
    if (frek == 929) {
      NazivStanice = NizStanica[10];
    }
    if (frek == 934) {
      NazivStanice = NizStanica[9];
    }
    if (frek == 936) {
      NazivStanice = NizStanica[14];
    }
    if (frek == 947) {
      NazivStanice = NizStanica[15];
    }
    if (frek == 951) {
      NazivStanice = NizStanica[1];
    }
    if (frek == 956) {
      NazivStanice = NizStanica[16];
    }
    if (frek == 961) {
      NazivStanice = NizStanica[2];
    }
    if (frek == 968) {
      NazivStanice = NizStanica[14];
    }
    if (frek == 972) {
      NazivStanice = NizStanica[17];
    }
    if (frek == 976) {
      NazivStanice = NizStanica[20];
    }
    if (frek == 979) {
      NazivStanice = NizStanica[18];
    }
    if (frek == 981) {
      NazivStanice = NizStanica[2];
    }
    if (frek == 984) {
      NazivStanice = NizStanica[13];
    }
    if (frek == 989) {
      NazivStanice = NizStanica[2];
    }
    if (frek == 993) {
      NazivStanice = NizStanica[13];
    }
    if (frek == 998) {
      NazivStanice = NizStanica[1];
    }
    if (frek == 1004) {
      NazivStanice = NizStanica[3];
    }
    if (frek == 1006) {
      NazivStanice = NizStanica[2];
    }
    if (frek == 1010) {
      NazivStanice = NizStanica[4];
    }
    if (frek == 1015) {
      NazivStanice = NizStanica[19];
    }
    if (frek == 1020) {
      NazivStanice = NizStanica[4];
    }
    if (frek == 1027) {
      NazivStanice = NizStanica[11];
    }
    if (frek == 1029) {
      NazivStanice = NizStanica[2];
    }
    if (frek == 1030) {
      NazivStanice = NizStanica[5];
    }
    if (frek == 1035) {
      NazivStanice = NizStanica[7];
    }
    if (frek == 1041) {
      NazivStanice = NizStanica[21];
    }
    if (frek == 1045) {
      NazivStanice = NizStanica[4];
    }
    if (frek == 1053) {
      NazivStanice = NizStanica[4];
    }
    if (frek == 1055) {
      NazivStanice = NizStanica[17];
    }
    if (frek == 1060) {
      NazivStanice = NizStanica[11];
    }
    if (frek == 1065) {
      NazivStanice = NizStanica[13];
    }
    if (frek == 1069) {
      NazivStanice = NizStanica[0];
    }
    if (frek == 1073) {
      NazivStanice = NizStanica[0];
    }
    if (frek == 1079) {
      NazivStanice = NizStanica[18];
    }
    //Za napisat imena stanica na ekran
    lcd.begin(20, 4);
    lcd.backlight();
    lcd.setCursor(((20 - NazivStanice.length()) / 2), 0);
    lcd.print(NazivStanice);
    Serial.print(NazivStanice);
    //Za nacrtat linearnu skalu na LCD ekran
    lcd.setCursor(1, 1);
    for (i = 0; i < 18; i++)
      lcd.write(SKALA);
    promijeniSkalu();
    //Za napisat frekvenciju i njenu mjernu jedinicu na LCD ekran
    lcd.setCursor(6, 2);
    lcd.print(String(frekvencija, 1) + String("MHz"));
    Serial.print("   ");
    Serial.print(frek);
    Serial.print("   ");
    Serial.print(String(frekvencija, 1) + String("MHz"));
  }
  //unaprijed postavljene frekvencije za brže kretanje po skali
  if (digitalRead(Tipka1) == LOW) {
    frekvencija = 87.8;
  }
  if (digitalRead(Tipka2) == LOW) {
    frekvencija = 92.8;
  }
  if (digitalRead(Tipka3) == LOW) {
    frekvencija = 97.9;
  }
  if (digitalRead(Tipka4) == LOW) {
    frekvencija = 102.9;
  }
  if (digitalRead(Tipka5) == LOW) {
    frekvencija = 107.9;
  }
  zadnjeStanjeCLK = trenutnoStanjeCLK;
  int btnState = digitalRead(SW);
  delay(1);
}
