#include <WiFi.h>
#include <WebServer.h>
#include <esp_now.h>

const char* ssid = "ESP-Chat";
const char* pass = "12345678";
uint8_t peerMac[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // send to all

typedef struct { uint8_t type; char text[31]; } Packet; // 0 beat, 1 msg, 2 ack, 3 reply

WebServer server(80);
volatile unsigned long lastAck = 0;
volatile bool newReply = false;
char replyBuf[31] = "";
unsigned long lastBeat = 0;
String chat = "";

const char page[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head>
<meta name="viewport" content="width=device-width,initial-scale=1">
<style>
body{font-family:sans-serif;margin:12px}
#log{height:55vh;overflow:auto;border:1px solid #ccc;padding:8px;white-space:pre-wrap}
input{width:70%;padding:10px;font-size:16px}button{padding:10px}
</style></head><body>
<h3>Satellite Chat</h3>
<div id="log"></div>
<input id="m" maxlength="30" placeholder="Type message">
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
  if (p.type == 2) lastAck = millis();
  if (p.type == 3) {
    strncpy(replyBuf, p.text, 30);
    newReply = true;
    lastAck = millis();
  }
}

void sendPacket(uint8_t type, const char *t) {
  Packet p = {};
  p.type = type;
  strncpy(p.text, t, 30);
  esp_now_send(peerMac, (uint8_t *)&p, sizeof(p));
}

void trimChat() {
  if (chat.length() > 1000) chat = chat.substring(chat.length() - 800);
}

void handleSend() {
  String m = server.arg("m");
  if (m.length() > 30) m = m.substring(0, 30);
  sendPacket(1, m.c_str());
  chat += "You: " + m + "\n";
  trimChat();
  server.send(200, "text/plain", "ok");
}

void handleLog() {
  bool connected = lastAck != 0 && millis() - lastAck < 10000;
  String s = connected ? "[Receiver CONNECTED]\n" : "[Receiver LOST]\n";
  server.send(200, "text/plain", s + chat);
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(ssid, pass, 1);   // channel 1, same as receiver

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
  Serial.println("Join ESP-Chat and open http://192.168.4.1");
}

void loop() {
  server.handleClient();

  if (millis() - lastBeat > 2000) {
    lastBeat = millis();
    sendPacket(0, "beat");
  }

  if (newReply) {
    newReply = false;
    chat += "Receiver: " + String(replyBuf) + "\n";
    trimChat();
  }
}
