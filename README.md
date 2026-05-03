# 🏥 ICU Emergency Alert System (ESP32 + Web + Telegram)

A lightweight **ICU monitoring & alert system** built using **ESP32**, featuring a modern mobile-friendly web dashboard, real-time emergency alerts, buzzer notifications, and Telegram integration.

---

## 🚀 Features

* ⚡ Real-time **Emergency Alert Trigger**
* 🌐 **Modern Web Dashboard** (Mobile Optimized UI)
* 🔔 **Buzzer Alarm System**
* 📲 **Telegram Notifications**
* 🧑‍⚕️ Patient Details Display (Bed, Ward, Name, Reason)
* ✅ Alert Acknowledge System
* 🌙 Clean **Matte Black UI Design**

---

## 📸 Preview

* **Normal Mode:**
  Displays *SYSTEM NORMAL* in green

* **Alert Mode:**
  Displays patient info with red emergency highlight + acknowledge button

---

## 🛠️ Hardware Requirements

* ESP32 Board
* Active Buzzer
* WiFi Network

---

## 💻 Software Requirements

* Arduino IDE
* ESP32 Board Package Installed

---

## 🔌 Wiring

| Component | ESP32 Pin |
| --------- | --------- |
| Buzzer    | GPIO 13   |

---

## ⚙️ Setup Instructions

### 1. Clone Repository

```bash
git clone https://github.com/your-username/icu-alert-system.git
cd icu-alert-system
```

---

### 2. Configure WiFi

Edit in code:

```cpp
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";
```

---

### 3. Setup Telegram Bot

1. Open Telegram and search for **BotFather**
2. Create a bot → get **Bot Token**
3. Get your **Chat ID**

Update in code:

```cpp
const char* botToken = "YOUR_BOT_TOKEN";
const char* chatID   = "YOUR_CHAT_ID";
```

---

### 4. Upload Code

* Select ESP32 board
* Upload via Arduino IDE

---

### 5. Get Device IP

Open Serial Monitor → note IP like:

```
192.168.x.x
```

---

### 6. Open Web Dashboard

Open in browser:

```
http://<ESP32-IP>
```

---

## 🚨 Trigger Alert

Use this URL:

```
http://<ESP32-IP>/alert?bed=12&ward=ICU&name=John&msg=Heart%20Rate%20Drop
```

---

## ✅ Acknowledge Alert

Click **ACKNOWLEDGE** button on dashboard
OR open:

```
http://<ESP32-IP>/ack
```

---

## 📲 Telegram Alert Example

```
ICU ALERT!
Bed: 12
Ward: ICU
Patient: John
Reason: Heart Rate Drop
```

---

## 🧠 How It Works

1. ESP32 hosts a web server
2. Alert is triggered via HTTP request
3. System:

   * Activates buzzer
   * Updates web UI
   * Sends Telegram message
4. User acknowledges → system resets

---

## 🎨 UI Design Highlights

* Matte black background
* Glassmorphism card layout
* Neon alert indicators
* Fully mobile responsive

---

## 🔮 Future Improvements

* 📊 Multi-bed dashboard
* 🔄 Auto-refresh (AJAX/WebSocket)
* 📱 Installable PWA app
* 📈 Live vitals monitoring
* 🔐 Authentication system

---

## 🤝 Contributing

Pull requests are welcome!
For major changes, open an issue first.

---

## 📄 License

This project is open-source under the MIT License.

---

## 👨‍💻 Author

Developed by **Your Name**

---

## ⭐ Support

If you like this project, give it a ⭐ on GitHub!
