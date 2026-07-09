#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include<TinyGPS++.h>
#include<avr/wdt.h>

#define SIM800L_Tx 3
#define SIM800L_Rx 2
char t;
int en1 = 9;
int en2 = 6;
int in1 = 5;
int in2 = 4;
int in3 = 7;
int in4 = 8;

SoftwareSerial SIM800L(SIM800L_Tx, SIM800L_Rx);
SoftwareSerial mySerial(3, 2);
SoftwareSerial gpsSerial(12,11);
char dtmf_cmd;
bool call_flag = false;

// I2C LCD configuration
LiquidCrystal_I2C lcd(0x27, 16, 2); // Adjust address (0x27 or 0x3F) if needed

void init_gsm();
void update_robot();
void updateSerial();

void setup() {
  SIM800L.begin(9600);
  Serial.begin(9600);
  pinMode(en1, OUTPUT);
  pinMode(en2, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(9,OUTPUT);

  // Initialize I2C LCD
  Wire.begin();
  lcd.init();
  lcd.backlight();
  lcd.print("GSM Robot");
  delay(1000);
  lcd.clear();

  init_gsm();
}

void loop() {
  String gsm_data;
  int x = -1;

  // Store serial data from SIM800L
  while (SIM800L.available()) {
    char c = SIM800L.read();
    gsm_data += c;
    delay(10);
  }

  // Check if DTMF is received from SIM800L
  if (call_flag) {
    x = gsm_data.indexOf("DTMF");
    if (x > -1) {
      dtmf_cmd = gsm_data[x + 6];
      Serial.println(dtmf_cmd);
      lcd.clear();
      lcd.print("DTMF: ");
      lcd.print(dtmf_cmd);
      update_robot();
    }

    x = gsm_data.indexOf("NO CARRIER");
    if (x > -1) {
      // Terminate ongoing call when call is disconnected
      SIM800L.println("ATH");
      call_flag = false;
      lcd.clear();
      lcd.print("Call Ended");
      delay(1000);
      lcd.clear();
    }
  } else {
    x = gsm_data.indexOf("RING");
    if (x > -1) {
      delay(5000);
      SIM800L.println("ATA");
      call_flag = true;
      lcd.clear();
      lcd.print("Incoming Call");
      delay(1000);
      lcd.clear();
      lcd.print("Call Accepted");
      delay(1000);
      lcd.clear();
    }
  }
}

void init_gsm() {
  boolean gsm_Ready = 1;
  Serial.println("initializing GSM module");
  lcd.print("GSM Init...");
  while (gsm_Ready > 0) {
    SIM800L.println("AT");
    Serial.println("AT");
    while (SIM800L.available()) {
      if (SIM800L.find("OK") > 0)
        gsm_Ready = 0;
    }
    delay(2000);
  }
  Serial.println("AT READY");
  lcd.clear();
  lcd.print("AT Ready");
  delay(1000);

  boolean ntw_Ready = 1;
  Serial.println("finding network");
  lcd.clear();
  lcd.print("NTW Init...");
  while (ntw_Ready > 0) {
    SIM800L.println("AT+CPIN?");
    Serial.println("AT+CPIN?");
    while (SIM800L.available()) {
      if (SIM800L.find("+CPIN: READY") > 0)
        ntw_Ready = 0;
    }
    delay(2000);
  }
  Serial.println("NTW READY");
  lcd.clear();
  lcd.print("NTW Ready");
  delay(1000);

  boolean DTMF_Ready = 1;
  Serial.println("turning DTMF ON");
  lcd.clear();
  lcd.print("DTMF Init...");
  while (DTMF_Ready > 0) {
    SIM800L.println("AT+DDET=1");
    Serial.println("AT+DDET=1");
    while (SIM800L.available()) {
      if (SIM800L.find("OK") > 0)
        DTMF_Ready = 0;
    }
    delay(2000);
  }
  Serial.println("DTMF READY");
  lcd.clear();
  lcd.print("DTMF Ready");
  delay(1000);
}

void updateSerial() {
  delay(500);
  while (Serial.available()) {
    mySerial.write(Serial.read()); // Forward what Serial received to Software Serial Port
  }
  while (mySerial.available()) {
    Serial.write(mySerial.read()); // Forward what Software Serial received to Serial Port
  }
}

void Forward() {
  analogWrite(en1, 100);
  analogWrite(en2, 100);
  digitalWrite(in1, HIGH);
  digitalWrite(in3, HIGH);
  lcd.clear();
  lcd.print("Forward");
}

void Reverse() {
  analogWrite(en1, 100);
  analogWrite(en2, 100);
  digitalWrite(in2, HIGH);
  digitalWrite(in4, HIGH);
  lcd.clear();
  lcd.print("Reverse");
}

void left() {
  analogWrite(en1, 100);
  digitalWrite(in1, HIGH);
  digitalWrite(in4, HIGH);
  lcd.clear();
  lcd.print("Left");
}

void right() {
  analogWrite(en2, 100);
  digitalWrite(in3, HIGH);
  digitalWrite(in2, HIGH);
  lcd.clear();
  lcd.print("Right");
}

void stop() {
  analogWrite(en1, 0);
  analogWrite(en2, 0);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  lcd.clear();
  lcd.print("Stop");
}

void update_robot() {
  if (dtmf_cmd == '2') {
    Forward();
    delay(2000);
    stop();
  }

  if (dtmf_cmd == '8') {
    Reverse();
    delay(2000);
    stop();
  }

  if (dtmf_cmd == '4') {
    left();
    delay(500);
    stop();
  }

  if (dtmf_cmd == '6') { // Corrected for turning right
    right();
    delay(500);
    stop();
  }

  if (dtmf_cmd == '5') {
    stop();
  }
  if (dtmf_cmd == '9') {
    Serial.println("Fetching GPS...");

    // Start GPS serial (NEO-6M)
    gpsSerial.begin(9600);
    TinyGPSPlus gps;
    unsigned long start = millis();
    bool gpsLock = false;

    while (millis() - start < 10000) { // Wait up to 10 seconds
        while (gpsSerial.available()) {
            gps.encode(gpsSerial.read());
        }
        if (gps.location.isValid()) {
            gpsLock = true;
            break;
        }
    }

 
   delay(10000);

    // Send SMS
    mySerial.begin(9600);
    mySerial.println("AT");
    updateSerial();

    mySerial.println("AT+CMGF=1"); // Set SMS to Text Mode
    updateSerial();

    mySerial.println("AT+CMGS=\"+919845446425\""); // Change to your number
    updateSerial();

    mySerial.print("https://www.google.com/maps?q="); mySerial.print(gps.location.lat(), 6); mySerial.print(","); mySerial.println(gps.location.lng(), 6);

    updateSerial();

    mySerial.write(26); // Ctrl+Z to send
    lcd.clear();
    lcd.print("SMS Sent");
    delay(10000);
 // wdt_enable(WDTO_15MS);  // Reset after 15ms
  //while (1);
    


  }

}