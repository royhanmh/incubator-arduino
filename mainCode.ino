#include <EEPROM.h>
#include <DHT.h>
#define DHTPIN A0 //
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C  lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

const int lamp = 10;
const int mist = 11; 
const int motor = 12;
const int ledLamp = 5;
const int ledMist = 6;
const int ledMotor = 7;

const int mainBtn = 2;
const int upBtn = 3;
const int downBtn = 8;

int menuChoice;
int setPointSuhu;
int setPointHumid;
int setModePutar;
int motorTimer = 10000;
int suhu;
int humid;
bool state = false;

void setup() {

  // put your setup code here, to run once:
  pinMode(mist, OUTPUT);
  pinMode(lamp, OUTPUT);
  pinMode(motor, OUTPUT);
  pinMode(ledLamp, OUTPUT);
  pinMode(ledMist, OUTPUT);
  pinMode(ledMotor, OUTPUT);

  pinMode(mainBtn, INPUT_PULLUP);
  pinMode(upBtn, INPUT_PULLUP);
  pinMode(downBtn, INPUT_PULLUP);
  //setup button
  digitalWrite(mainBtn, HIGH);
  digitalWrite(upBtn, HIGH);
  digitalWrite(downBtn, HIGH);

  dht.begin();
  lcd.begin(16, 2);
  lcd.setCursor(4, 0); lcd.print("INCUBATOR");
  lcd.setCursor(6, 1); lcd.print("TI5C");
  delay(200);
  lcd.clear();
  menuChoice = 1;
}



void loop() {
  digitalWrite(motor, LOW);
  setModePutar = EEPROM.read(2);
  unsigned long presentMillis = millis();
  // put your main code here, to run repeatedly:


  //millis motor
  while (millis() - presentMillis <= motorTimer) {
    delay(200);

    digitalWrite(ledMotor, LOW);

  // read DHT11
  suhu = dht.readTemperature();
  humid = dht.readHumidity();
  //
  if (digitalRead(mainBtn) == LOW)
  {
    state = true;
    if (menuChoice < 4)
    {
      menuChoice++;
    }
    else {
      menuChoice = 1;
    }

  }


  switch (menuChoice) {
    case 1:
    setPointSuhu = EEPROM.read(0);
    setPointHumid = EEPROM.read(1);
      if ((state == true) || (suhu == 9)) {
        state = false;
        lcd.clear();
      }
      lcd.setCursor(0, 0); lcd.print("Suhu = ");
      lcd.setCursor(7, 0); lcd.print(suhu);
      lcd.setCursor(0, 1); lcd.print("Humid = ");
      lcd.setCursor(7, 1); lcd.print(humid);
      break;

    case 2:
      if (state == true) {
        state = false;
        lcd.clear();
      }
      lcd.setCursor(0, 0); lcd.print("Set Suhu = ");
      lcd.setCursor(13, 0); lcd.print(setPointSuhu);
      break;

    case 3:
      if (state == true) {
        state = false;
        lcd.clear();
      }
      lcd.setCursor(0, 0); lcd.print("Set Humid = ");
      lcd.setCursor(13, 0); lcd.print(setPointHumid);
      break;

    case 4:
      if (state == true) {
        state = false;
        lcd.clear();
      }
      lcd.setCursor(0, 0); lcd.print("Mode Putar = ");
      lcd.setCursor(13, 0); lcd.print(setModePutar);
      lcd.setCursor(0, 1); lcd.print(motorTimer);
      break;
  }
  // button set point suhu
  if (menuChoice == 2 && digitalRead(upBtn) == LOW) {
    setPointSuhu++;
    EEPROM.write(0, setPointSuhu);
  }
  if (menuChoice == 2 && digitalRead(downBtn) == LOW && setPointSuhu > 0) {
    setPointSuhu--;
    EEPROM.write(0, setPointSuhu);
  }

  // button set point humidity
  if (menuChoice == 3 && digitalRead(upBtn) == LOW) {
    setPointHumid++;
    EEPROM.write(1, setPointHumid);
  }
  if (menuChoice == 3 && digitalRead(downBtn) == LOW && setPointHumid > 0) {
    setPointHumid--;
    EEPROM.write(1, setPointHumid);
  }

  // button set motor mode
  if (menuChoice == 4 && digitalRead(upBtn) == LOW) {
    if (setModePutar < 3) {
      setModePutar++;
    } else {
      setModePutar = 1;
    }
    EEPROM.write(2, setModePutar);
  }
  if (menuChoice == 4 && digitalRead(downBtn) == LOW && setModePutar > 0) {
    setModePutar--;
    EEPROM.write(2, setModePutar);
  }

  // motor mode conditioning
  if (setModePutar == 1) {
    motorTimer = 10000;
  } else if (setModePutar == 2) {
    motorTimer = 20000;
  } else if (setModePutar == 3) {
    motorTimer = 30000;
  }




   //Pengkondisian Relay
   if (suhu > setPointSuhu)
  {
    digitalWrite(lamp, LOW);
    digitalWrite(ledLamp, LOW);  
  }
  else
  {
    digitalWrite(lamp, HIGH);
    digitalWrite(ledLamp, HIGH);
  }

  if (humid > setPointHumid)
  {
    digitalWrite(mist, LOW); 
    digitalWrite(ledMist, LOW);    
  }
  else
  {
    digitalWrite(mist, HIGH);  
    digitalWrite(ledMist, HIGH);  
  }
}
state = true;
lcd.clear();
lcd.setCursor(2, 0); lcd.print("Motor Putar");
digitalWrite(motor, HIGH);
digitalWrite(ledMotor, HIGH);
delay(1000);


}
