#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ===== WiFi =====
const char* ssid = "ICU_NET";
const char* password = "12345678";

// ===== Receiver IP =====
const char* esp32IP = "http://10.168.122.227"; // CHANGE THIS

// ===== Button =====
#define BUTTON_PIN D4

// ===== OLED =====
Adafruit_SSD1306 display(128, 64, &Wire, -1);

// ===== Patient Data =====
String bedNo = "21";
String wardNo = "106";
String patientName = "John";
String alertMsg = "Low Heart Rate";

bool lastState = HIGH;

// ===== OLED Screens =====
void showIdle() {
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.println("ICU TRANSMITTER");
  display.println("----------------");
  display.println("Press Button");
  display.display();
}

void showSending() {
  display.clearDisplay();
  display.setCursor(0,0);

  display.println("SENDING ALERT");
  display.println("----------------");
  display.println("Bed: " + bedNo);
  display.println("Ward: " + wardNo);
  display.println(patientName);

  display.display();
}

// ===== SEND ALERT =====
void sendAlert() {
  WiFiClient client;
  HTTPClient http;

  String url = String(esp32IP) + "/alert?";
  url += "bed=" + bedNo;
  url += "&ward=" + wardNo;
  url += "&name=" + patientName;
  url += "&msg=" + alertMsg;

  url.replace(" ", "%20");

  http.begin(client, url);
  http.GET();
  http.end();

  Serial.println(url);
}

// ===== SETUP =====
void setup() {
  Serial.begin(115200);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Wire.begin(D2, D1);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  display.setCursor(0,0);
  display.println("Connecting WiFi...");
  display.display();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);

  showIdle();
}

// ===== LOOP =====
void loop() {
  bool state = digitalRead(BUTTON_PIN);

  if (state == LOW && lastState == HIGH) {

    showSending();
    sendAlert();

    delay(2000);
    showIdle();
  }

  lastState = state;
}