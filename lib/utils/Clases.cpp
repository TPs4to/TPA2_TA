#include "Clases.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include "init.h"


ControladorPotenciometro::ControladorPotenciometro() {
  pinMode(POTENCIOMETRO,INPUT);
};

int ControladorPotenciometro::getPotencia() {
  return analogRead(POTENCIOMETRO);
}

Controlador::Controlador(
  UniversalTelegramBot *bot,
  ControladorLed *controladorLed,
  ControladorPotenciometro *controladorPotenciometro) {
  this->bot = bot;
  this->controladorLed = controladorLed;
  this->controladorPotenciometro = controladorPotenciometro;
  this->sensorDHT->begin();
  this->display->setTextSize(1);
  this->display->setTextColor(SH110X_WHITE);
};

void Controlador::manejar_nuevos_mensajes(int cant_mensajes) {
  for (int i = 0; i < cant_mensajes; i++) {
        String text = this->bot->messages[i].text;
        String chat_id = this->bot->messages[i].chat_id;

        Serial.println("Mensaje recibido:" + text);

        if (text == "/led23on") {
          this->controladorLed->prenderLed(LED_VERDE);
          this->ledVerde = true;
        } else if (text == "/led2on") {
          this->controladorLed->prenderLed(LED_AZUL);
          this->ledAzul = true;
        } else if (text == "/led23off") {
          this->ledVerde = false;
          this->controladorLed->apagarLed(LED_VERDE);
        } else if (text == "/led2off") {
          this->ledAzul = false;
          this->controladorLed->apagarLed(LED_AZUL);
        } else if (text == "/pote") {
          int valor = this->controladorPotenciometro->getPotencia();
          String mensaje = "El potenciometro tiene un valor de: " + String(valor);
          Serial.println(mensaje);
          this->bot->sendMessage(chat_id, mensaje);
        } else if (text == "/start") {
          String mensaje = this->saludar();
          this->bot->sendMessage(chat_id, mensaje);
        } else if(text == "/dht22") {
          String mensaje = this->informeSensor();
          this->bot->sendMessage(chat_id, mensaje);
        } else if (text == "/displayLed") {
          String mensaje = this->mensajeLed();
          this->mostrarMensaje(mensaje);
        } else if(text == "/displayDht22") {
          String mensaje = this->mensajeDht22();
          this->mostrarMensaje(mensaje);
        } else if(text == "/displayPote") {
          String mensaje = this->mensajePote();
          this->mostrarMensaje(mensaje);
        } else if (text == "/platiot") {
          this->enviarPlatIOT();
          this->bot->sendMessage(chat_id, "URL: https://thingspeak.com/channels/2679081");
        } else {
          String mensaje = "Comando invalido";
          this->bot->sendMessage(chat_id, mensaje);
        } 
      }
};

void Controlador::enviarPlatIOT() {
  float temperatura = this->sensorDHT->readTemperature();
  float humedad = this->sensorDHT->readHumidity();

  String url = URL;
  url += "?api_key=" + String(API_KEY);
  url += "&field1=" + String(temperatura);
  url += "&field2=" + String(humedad);
  
  this->http->begin(url);
  int httpCode = http->GET();

  if (httpCode > 0) {
    String datos = http->getString();
    Serial.println("Datos enviados a ThingSpeak: " + datos);
    Serial.println("URL: https://thingspeak.com/channels/2679081");
  } else {
    Serial.println("Error en la conexion: " + String(httpCode));
  }
  this->http->end();
}

void Controlador::beginDisplay() {
  display->begin(0x3c, true);
}

String Controlador::mensajePote() {
  int statusPote = this->controladorPotenciometro->getPotencia();
  String mensaje = " Potenciometro STATUS\n\n" \
  "Potenciometro: " + String(statusPote);

  return mensaje;
}

String Controlador::mensajeDht22() {
  float statusTem = this->sensorDHT->readTemperature();
  float statusHum = this->sensorDHT->readHumidity();

  String mensaje = "-----DHT22 STATUS----\n\n" \
  "Temperatura: " + String(statusTem) + "*C\n" \
  "Humedad: " + String(statusHum) + "%";

  return mensaje;
}


String Controlador::mensajeLed() {

  String statusVerde;
  String statusAzul;

  if (ledAzul) {
    statusAzul = "ENCENDIDO";
  } else {
    statusAzul = "APAGADO";
  }

  if (ledVerde) {
    statusVerde = "ENCENDIDO";
  } else {
    statusVerde = "APAGADO";
  }

  String mensaje = "-----LED STATUS-----\n\n" \
  "Led verde: " + statusVerde + "\n" \
  "Led azul: " + statusAzul + "\n";

  return mensaje;
}


void Controlador::mostrarMensaje(String mensaje) {
  display->clearDisplay();
  display->setCursor(0, 0);
  display->println(mensaje);
  display->display();
};



String Controlador::informeSensor() {
  float temperatura = this->sensorDHT->readTemperature();
  float humedad = this->sensorDHT->readHumidity();

  String mensaje = "La temperatura actual es:" + String(temperatura, 2) + "°C\n" \
  "La humedad actual es:" + String(humedad, 1) + "%";
  return mensaje;
};


String Controlador::saludar() {
  String mensaje = "----Bienvenido----\n" \
  "----Comandos----\n" \
  "/start: Lista los comandos disponibles para este bot.\n"
  "/led<led><on/off>: Permite encender o apagar el led verde(23) o azul(2).\n" \
  "/dht22: Informa los valores de humedad y temperatura del sensor.\n" \
  "/pote: Informa el valor actual del potenciometro.\n" \
  "/platiot: enviar los valores de humedad y temperatura del sensor DHT22 a la plataforma IoT (NOMBRE PLATAFORMA).\n" \
  "/display<conmando>: Informar en el display el estado de los componentes.\n";
  return mensaje;
}

ControladorLed::ControladorLed() {
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AZUL, OUTPUT);
};

void ControladorLed::prenderLed(int pin) {
  digitalWrite(pin, HIGH);
};

void ControladorLed::apagarLed(int pin) {
  digitalWrite(pin, LOW);
};