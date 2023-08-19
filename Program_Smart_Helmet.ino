#include "DFRobotDFPlayerMini.h"
HardwareSerial hwSerial(2);
DFRobotDFPlayerMini dfPlayer;
int volume = 30;

#include "CTBot.h"
CTBot myBot;
String ssid  = "Redmi Note 10S";
String pass  = "11091998";
String token = "6592803937:AAGWP885td6H1qtd36Ke-qRKqzS-Sh2_vgI";
const int id = 1425969384;
uint8_t buzzer = 15;

#include <TinyGPS++.h>
#include <SoftwareSerial.h>
static const int RXPin = 12, TXPin = 14;
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
double latt ;
double lngg ;
String linkk ;
SoftwareSerial ss(RXPin, TXPin);

#include <Ultrasonic.h>
#define TRIGGER_PIN  5
#define ECHO_PIN     18
Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);
float cmMsec;
long microsec = ultrasonic.timing();

void setup() {
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);
  delay(200);
  digitalWrite(buzzer, HIGH);
  Serial.begin(9600);
  hwSerial.begin(9600, SERIAL_8N1);
  dfPlayer.begin(hwSerial);
  dfPlayer.setTimeOut(500);
  dfPlayer.volume(volume);
  dfPlayer.EQ(DFPLAYER_EQ_NORMAL);
  dfPlayer.outputDevice(DFPLAYER_DEVICE_SD);
  Serial.println("Starting TelegramBot...");
  myBot.wifiConnect(ssid, pass);
  myBot.setTelegramToken(token);
  if (myBot.testConnection())  {
    Serial.println("\ntestConnection OK");
    dfPlayer.play(1);
    delay (6000);
    dfPlayer.stop();
    delay (100);
  }
  else {
    Serial.println("\ntestConnection NOK");
  }
  myBot.sendMessage(id, "SUKSES KONEK NIH");
  delay(1000);
  ss.begin(GPSBaud);
  Serial.println();
}

void loop() {
  Teleg();
  delay(100);
  GPS();
  delay(100);
  GPS();
  delay(100);
  GPS();
  delay(100);
  GPS();
  delay(100);
  PING();
  delay(100);
  if ( cmMsec <= 200)  {
    dfPlayer.play(2);
    delay (5000);
    dfPlayer.stop();
    delay (100);
  }
}

void GPS() {
  while (ss.available() > 0)
    if (gps.encode(ss.read()))
      displayInfo();
  if (millis() > 5000 && gps.charsProcessed() < 10)  {
    Serial.println(F("No GPS detected: check wiring."));
    while (true);
  }
}

void displayInfo() {
  Serial.print(F("Location: "));
  if (gps.location.isValid())  {
    latt = gps.location.lat();
    lngg = gps.location.lng();
    Serial.print(latt, 6);
    Serial.print(F(","));
    Serial.println(lngg, 6);
  }
  else  {
    Serial.print(F("INVALID"));
  }
  Serial.println();
}

void PING() {
  microsec = ultrasonic.timing();
  cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
  Serial.print(", CM: ");
  Serial.print(cmMsec);
  Serial.println();
}

void Teleg() {
  TBMessage msg; //variable pesan
  if (CTBotMessageText == myBot.getNewMessage(msg)) {
    if (msg.text.equalsIgnoreCase("Hidupkan Buzzer")) {
      digitalWrite(buzzer, LOW);
      myBot.sendMessage(msg.sender.id, "OK, Buzzer sudah ON");
    }
    else if (msg.text.equalsIgnoreCase("Matikan Buzzer")) {
      digitalWrite(buzzer, HIGH);
      myBot.sendMessage(msg.sender.id, "OK, Buzzer sudah OFF");
    }
    else if (msg.text.equalsIgnoreCase("Cek lokasi")) {
      Serial.println("Cek lokasi");
      linkk += "www.google.com/maps/place/" + String(latt, 6) + "," + String(lngg, 6) ;
      Serial.println(linkk);
      myBot.sendMessage(msg.sender.id, linkk, "");
    }
    else {
      myBot.sendMessage(msg.sender.id, "Pesan Tidak Terdaftar");
    }
  }
  delay(500);
}
