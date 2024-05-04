#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsClient.h>
int ledbuilt= 23;
char t;

const char *ssid = "#ToCoToCo";
const char *password = "1900636936";
const char *webSocketServerAddress = "10.100.59.32"; // Địa chỉ IP của ESP8266
const uint16_t webSocketServerPort = 81;

WebSocketsClient webSocket;

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length)
{
  switch (type)
  {
  case WStype_DISCONNECTED:
    Serial.printf("[WSc] Disconnected!\n");
    break;
  case WStype_CONNECTED:
    Serial.printf("[WSc] Connected to url: %s\n", payload);
    break;
  case WStype_TEXT:
    t = (char)payload[0];
    Serial.printf("[WSc] get text: %s\n", payload);

    if (payload[0] == '1')
    {
      digitalWrite(ledbuilt, LOW);
    }
    else if (payload[0]  == '0')
    {
      digitalWrite(ledbuilt, HIGH);
    }
    break;
  case WStype_BIN:
    Serial.printf("[WSc] get binary length: %u\n", length);
    Serial.println("Data:");
    for (size_t i = 0; i < length; i++)
    {
      Serial.printf("%02X ", payload[i]);
    }
    Serial.println();
    break;
  default:
    break;
  }
}

void setup()
{
  Serial.begin(115200);
  delay(10);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  pinMode(ledbuilt, OUTPUT);

  webSocket.begin(webSocketServerAddress, webSocketServerPort);
  webSocket.onEvent(webSocketEvent);
}

void loop()
{
  webSocket.loop();
  char key = getchar();
  if (key == '1')
    webSocket.sendTXT("1");
  else if (key == '0')
    webSocket.sendTXT("0");
}