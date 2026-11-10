#include <WiFi.h>
#include <WebServer.h>
#include <esp_now.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const char* ssid = "ESP-Reply";
const char* pass = "12345678";
uint8_t peerMac[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // send to all

#define GREEN_LED 25
#define RED_LED   26
#define BUZZER    27

Adafruit_SSD1306 display(128, 64, &Wire, -1);
WebServer server(80);
typedef struct { uint8_t type; char text[31]; } Packet; // 0 beat, 1 msg, 2 ack, 3 reply

volatile int rssi = -100;
volatile unsigned long lastRx = 0;
volatile bool newMsg = false, ackPending = false, ackIsMsg = false;
char lastMsg[31] = "---";
char incomingMsg[31] = "";
unsigned long lastDraw = 0;
String chat = "";

const char page[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head>
<meta name="viewport" content="width=device-width,initial-scale=1">
<style>
body{font-family:sans-serif;margin:12px}
#log{height:55vh;overflow:auto;border:1px solid #ccc;padding:8px;white-space:pre-wrap}
input{width:70%;padding:10px;font-size:16px}button{padding:10px}
</style></head><body>
<h3>Receiver Chat</h3>
<div id="log"></div>
<input id="m" maxlength="30" placeholder="Type reply">
<button onclick="send()">Send</button>
<script>
function send(){
  var m=document.getElementById('m');
  if(!m.value)return;
  fetch('/send?m='+encodeURIComponent(m.value));
  m.value='';
}
setInterval(function(){
  fetch('/log').then(r=>r.text()).then(t=>{
    var l=document.getElementById('log');
    l.textContent=t; l.scrollTop=l.scrollHeight;
  });
},1000);
</script></body></html>
)rawliteral";

void onRecv(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
  Packet p = {};
  memcpy(&p, data, min(len, (int)sizeof(p)));
  rssi = info->rx_ctrl->rssi;
  lastRx = millis();
  if (p.type == 1) {
    strncpy(incomingMsg, p.text, 30);
    newMsg = true;
    ackIsMsg = true;
  } else {
    ackIsMsg = false;
  }
  ackPending = true;
}

void trimChat() {
  if (chat.length() > 1000) chat = chat.substring(chat.length() - 800);
}

void sendReply(const char *t) {
  Packet m = {};
  m.type = 3;
  strncpy(m.text, t, 30);
  esp_now_send(peerMac, (uint8_t *)&m, sizeof(m));
  chat += "You: " + String(t) + "\n";
  trimChat();
}

void handleSend() {
  String m = server.arg("m");
  if (m.length() > 30) m = m.substring(0, 30);
  sendReply(m.c_str());
  server.send(200, "text/plain", "ok");
}

void handleLog() {
  bool connected = lastRx != 0 && millis() - lastRx < 10000;
  String s = connected ? "[Transmitter CONNECTED]\n" : "[Transmitter LOST]\n";
  server.send(200, "text/plain", s + chat);
}

void setup() {
  Serial.begin(115200);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  Wire.begin(21, 22);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextColor(SSD1306_WHITE);

  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(ssid, pass, 1);   // channel 1, same as transmitter

  esp_now_init();
  esp_now_register_recv_cb(onRecv);
  esp_now_peer_info_t peer = {};
  memcpy(peer.peer_addr, peerMac, 6);
  peer.channel = 0;
  peer.encrypt = false;
  esp_now_add_peer(&peer);

  server.on("/", []() { server.send_P(200, "text/html", page); });
  server.on("/send", handleSend);
  server.on("/log", handleLog);
  server.begin();

  Serial.println("Join ESP-Reply and open http://192.168.4.1");
}

void loop() {
  server.handleClient();

  if (ackPending) {
    ackPending = false;
    Packet a = {};
    a.type = 2;
    strcpy(a.text, ackIsMsg ? "msg" : "beat");
    esp_now_send(peerMac, (uint8_t *)&a, sizeof(a));
  }

  if (newMsg) {
    newMsg = false;
    strncpy(lastMsg, incomingMsg, 30);
    chat += "Friend: " + String(lastMsg) + "\n";
    trimChat();
    Serial.print("Received: ");
    Serial.println(lastMsg);
    digitalWrite(BUZZER, HIGH);
    delay(150);
    digitalWrite(BUZZER, LOW);
  }

  bool connected = lastRx != 0 && millis() - lastRx < 10000;
  digitalWrite(GREEN_LED, connected);
  digitalWrite(RED_LED, !connected);

  if (millis() - lastDraw > 300) {
    lastDraw = millis();
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print("Msg: ");
    display.println(lastMsg);
    display.setCursor(0, 16);
    display.print("Signal: ");
    if (connected) { display.print(rssi); display.println(" dBm"); }
    else display.println("--");
    display.setCursor(0, 28);
    display.print("Status: ");
    display.println(connected ? "CONNECTED" : "LOST");
    int bars = connected ? constrain(map(rssi, -90, -40, 1, 4), 1, 4) : 0;
    for (int i = 0; i < 4; i++) {
      int h = 6 + i * 5;
      if (i < bars) display.fillRect(90 + i * 8, 62 - h, 6, h, SSD1306_WHITE);
      else display.drawRect(90 + i * 8, 62 - h, 6, h, SSD1306_WHITE);
    }
    display.display();
  }

  // Serial Monitor reply still works
  if (Serial.available()) {
    char buf[31] = "";
    int len = Serial.readBytesUntil('\n', buf, 30);
    buf[len] = '\0';
    if (len > 0) sendReply(
