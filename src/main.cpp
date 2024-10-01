#include <WiFiClientSecure.h>
#include <WiFi.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include "Clases.h"
#include "init.h"


unsigned long ultimo_scan;
const long scan_delay = 1000;

WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

ControladorPotenciometro *controladorPotenciometro = new ControladorPotenciometro();
ControladorLed *controladorLed = new ControladorLed();
Controlador *controlador = new Controlador(&bot, controladorLed, controladorPotenciometro);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.begin(WIFI_USSER, WIFI_PASSWORD);
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  controlador->beginDisplay();
  

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());

  ultimo_scan = millis();

}

void loop() {

  //bucle para el bot
  
  if (millis() > ultimo_scan + scan_delay) {
      int mensajes = bot.getUpdates(bot.last_message_received + 1);
      Serial.println(mensajes);
      while (mensajes) {
        controlador->manejar_nuevos_mensajes(mensajes);
        mensajes = bot.getUpdates(bot.last_message_received + 1);
      }
      ultimo_scan = millis();
   }

}
