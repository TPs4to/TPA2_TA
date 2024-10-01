#ifndef CLASES_H
#define CLASES_H
#include <UniversalTelegramBot.h>
#include <DHT.h>
#include <Adafruit_SH110X.h>
#include <Wire.h>
#include "init.h"
#include <HTTPClient.h>


class ControladorPotenciometro {
  public:
  ControladorPotenciometro();

  int getPotencia();
};

class ControladorLed {

public:
  ControladorLed();

  void prenderLed(int pin);

  void apagarLed(int pin);
};

class Controlador {

private:
  bool ledVerde = false;
  bool ledAzul = false;
  UniversalTelegramBot *bot;
  ControladorLed *controladorLed;
  ControladorPotenciometro *controladorPotenciometro;
  DHT *sensorDHT = new DHT(SENSOR_HUMEDAD, DHT22);
  Adafruit_SH1106G *display = new Adafruit_SH1106G(ANCHO_PANTALLA, ALTO_PANTALLA, &Wire, -1);
  HTTPClient *http = new HTTPClient();

public:
  Controlador(
    UniversalTelegramBot *bot,
    ControladorLed *controladorLed,
    ControladorPotenciometro *controladorPotenciometro);

  void manejar_nuevos_mensajes(int cant_mensajes);

  String saludar();

  String informeSensor();

  void mostrarMensaje(String mensaje);

  void beginDisplay();

  String mensajeLed();

  String mensajeDht22();

  String mensajePote();

  void enviarPlatIOT();

};


#endif