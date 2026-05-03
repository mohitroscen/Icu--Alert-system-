#include <WiFi.h>
#include <WebServer.h>
#include <WiFiClientSecure.h>

// ===== WiFi =====
const char* ssid = "ICU_NET";
const char* password = "12345678";

// ===== Telegram =====
const char* botToken = "8637984521:AAGgtBl5fpe_Xqy3uZENG2Ixdou1-Ez6Acs";
const char* chatID   = "8395443764";

// ===== Server =====
WebServer server(80);

// ===== Buzzer =====
const int BUZZER = 13;

// ===== State =====
bool alert = false;
bool buzzerOn = false;
unsigned long lastBeep = 0;
bool buzzerState = false;

// ===== Patient Data =====
String bedNo = "";
String wardNo = "";
String patientName = "";
String alertMsg = "";

// ===== TELEGRAM =====
void sendTelegram(String msg) {
  WiFiClientSecure client;
  client.setInsecure();

  msg.replace(" ", "%20");
  msg.replace("\n", "%0A");

  String url = "/bot" + String(botToken) +
               "/sendMessage?chat_id=" + chatID +
               "&text=" + msg;

  if (client.connect("api.telegram.org", 443)) {
    client.println("GET " + url + " HTTP/1.1");
    client.println("Host: api.telegram.org");
    client.println("Connection: close");
    client.println();
  }
}

// ===== MODERN UI PAGE =====
String page() {
  String p = R"rawliteral(
  <!DOCTYPE html>
  <html>
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ICU Monitor</title>

    <style>
      body {
        margin: 0;
        font-family: Arial, sans-serif;
        background: #0b0f14;
        color: white;
        display: flex;
        justify-content: center;
        align-items: center;
        height: 100vh;
      }

      .container {
        width: 90%;
        max-width: 420px;
        background: rgba(255,255,255,0.06);
        border-radius: 18px;
        padding: 20px;
        box-shadow: 0 0 25px rgba(0,0,0,0.6);
        backdrop-filter: blur(10px);
        text-align: center;
      }

      h1 {
        margin: 0;
        font-size: 22px;
      }

      .normal {
        color: #00ff88;
        text-shadow: 0 0 10px #00ff88;
      }

      .alert {
        color: #ff3b3b;
        text-shadow: 0 0 12px #ff3b3b;
      }

      .card {
        margin-top: 15px;
        padding: 15px;
        background: rgba(255,255,255,0.05);
        border-radius: 12px;
        text-align: left;
      }

      .label {
        color: #aaa;
        font-size: 13px;
      }

      .value {
        font-size: 16px;
        margin-bottom: 10px;
      }

      .btn {
        margin-top: 15px;
        padding: 12px;
        width: 100%;
        border: none;
        border-radius: 12px;
        background: linear-gradient(135deg, #ff3b3b, #ff7b00);
        color: white;
        font-size: 16px;
        cursor: pointer;
      }

      .btn:hover {
        opacity: 0.9;
      }

      .status-bar {
        height: 4px;
        width: 100%;
        background: #222;
        margin-bottom: 15px;
        border-radius: 5px;
        overflow: hidden;
      }

      .status-fill {
        height: 100%;
        width: 100%;
        background: %STATUS_COLOR%;
      }
    </style>
  </head>

  <body>
    <div class="container">

      <div class="status-bar">
        <div class="status-fill"></div>
      </div>

      %CONTENT%

    </div>
  </body>
  </html>
  )rawliteral";

  String content;

  if (alert) {
    content = "<h1 class='alert'>EMERGENCY ALERT</h1>";

    content += "<div class='card'>";
    content += "<div class='label'>Bed No</div><div class='value'>" + bedNo + "</div>";
    content += "<div class='label'>Ward No</div><div class='value'>" + wardNo + "</div>";
    content += "<div class='label'>Patient</div><div class='value'>" + patientName + "</div>";
    content += "<div class='label'>Reason</div><div class='value'>" + alertMsg + "</div>";
    content += "</div>";

    content += "<a href='/ack'><button class='btn'>ACKNOWLEDGE</button></a>";

    p.replace("%STATUS_COLOR%", "#ff3b3b");
  } 
  else {
    content = "<h1 class='normal'>SYSTEM NORMAL</h1>";
    content += "<p style='color:#aaa'>All ICU systems are stable</p>";

    p.replace("%STATUS_COLOR%", "#00ff88");
  }

  p.replace("%CONTENT%", content);

  return p;
}

// ===== HANDLERS =====
void handleRoot() {
  server.send(200, "text/html", page());
}

void handleAlert() {
  if (server.hasArg("bed")) bedNo = server.arg("bed");
  if (server.hasArg("ward")) wardNo = server.arg("ward");
  if (server.hasArg("name")) patientName = server.arg("name");
  if (server.hasArg("msg")) alertMsg = server.arg("msg");

  alert = true;
  buzzerOn = true;

  String msg = "ICU ALERT!\n";
  msg += "Bed: " + bedNo + "\n";
  msg += "Ward: " + wardNo + "\n";
  msg += "Patient: " + patientName + "\n";
  msg += "Reason: " + alertMsg;

  sendTelegram(msg);

  server.send(200, "text/plain", "OK");
}

void handleAck() {
  alert = false;
  buzzerOn = false;
  digitalWrite(BUZZER, LOW);

  server.sendHeader("Location", "/");
  server.send(302, "text/plain", "");
}

// ===== SETUP =====
void setup() {
  Serial.begin(115200);
  pinMode(BUZZER, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);

  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/alert", handleAlert);
  server.on("/ack", handleAck);

  server.begin();
}

// ===== LOOP =====
void loop() {
  server.handleClient();

  if (buzzerOn) {
    if (millis() - lastBeep > 300) {
      buzzerState = !buzzerState;
      digitalWrite(BUZZER, buzzerState);
      lastBeep = millis();
    }
  }
}