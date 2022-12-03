#include <SPI.h>
#include <MFRC522.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define FIREBASE_HOST "nodeatt-2225b-default-rtdb.firebaseio.com/" // realtime database url
#define FIREBASE_AUTH "dGMPr6Z9T6jf1Ms2D5J8GGbQ29HXdLDC0NLjkGqN" // database key

const char* ssid = "apple"; //WIFI SSID
const char* password = "applewas01"; //WIFI Password
const char* host = "maker.ifttt.com"; //IFTTT url
constexpr uint8_t RST_PIN = D1;     // Reset Pin 
constexpr uint8_t SDA_PIN = D4;     // SDA Pin
const long utcOffsetInSeconds = 20700; // UTC time of Nepal in seconds
MFRC522 rfid(SDA_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"}; //list of days of the week

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
String tag;
//Define FirebaseESP8266 data object
FirebaseData firebaseData;
FirebaseData ledData;

FirebaseJson json;

void setup() {
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
  delay(1000);
  Serial.println("Wifi not connected");}
  pinMode(D8, OUTPUT);
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  timeClient.begin();
}

void loop() {
  timeClient.update();
  if ( ! rfid.PICC_IsNewCardPresent()){
    return;
  }
  if (rfid.PICC_ReadCardSerial()) {
    for (byte i = 0; i < 4; i++) {
      tag += rfid.uid.uidByte[i];
    }
    Serial.println(tag);
    if (tag == "1299225426") {
      String DAY = daysOfTheWeek[timeClient.getDay()];
      String TIME = timeClient.getFormattedTime();
      String Presnece = "/Attendance/Aayush Pandey on " + DAY+" - "+TIME;
      Serial.println("Aayush Pandey attended at "+Presnece);
      if (Firebase.setString(firebaseData, Presnece, "Present")){
        Serial.println("PASSED");
      }else{
        Serial.println("FAILED");
      }
      WiFiClient client ;
      const int httpPort = 80;
      if (!client.connect(host, httpPort )) {
        Serial.println("connection failed");
        delay(1000);
        return;
      }
      String url = "/trigger/Halow_World/json/with/key/h1FNc652UKgBVioTvVKQuZ1fzGhEtdtpQYF0vy_pezt";
      client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                      "Host: " + host +  "\r\n" + 
                                  "Connection: close\r\n\r\n");
    } else if (tag == "86149244249") {
      
      String DAY = daysOfTheWeek[timeClient.getDay()];
      String TIME = timeClient.getFormattedTime();
      String Presnece = "/Attendance/Bishwash Adhikari on " + DAY+" - "+TIME;
      Serial.println("Bishwash Adhikari attended at "+Presnece);
      if (Firebase.setString(firebaseData, Presnece, "Present")){
        Serial.println("PASSED");
      }else{
        Serial.println("FAILED");
      }
      WiFiClient client ;
      const int httpPort = 80;
      if (!client.connect(host, httpPort )) {
        Serial.println("connection failed");
        delay(1000);
        return;
      }
      String url = "/trigger/Halow_World/json/with/key/h1FNc652UKgBVioTvVKQuZ1fzGhEtdtpQYF0vy_pezt";
      client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                      "Host: " + host +  "\r\n" + 
                                  "Connection: close\r\n\r\n");
    }
    else {
      Serial.println("User Not Registered");
    }
    tag = "";
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
    
  }
}
