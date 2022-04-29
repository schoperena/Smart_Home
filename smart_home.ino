/*
Smart Home
Version 2: Se añaden sensores pir y funcion de alarma
Sebastian choperena Solano
*/

#include <EasyBuzzer.h>
#include <WiFi.h>

const char *ssid = "CH-Home";
const char *password = "hi5-L7-U06Zl0fPf";
WiFiServer server(80);

String header;

String ESTADO_DISP1 = "off";
String ESTADO_DISP2 = "off";
String ESTADO_DISP3 = "off";
String ESTADO_DISP4 = "off";
String ESTADO_ARM = "off";

// Relays
const byte PIN_DISP1 = 14;
const byte PIN_DISP2 = 27;
const byte PIN_DISP3 = 26;
const byte PIN_DISP4 = 25;

// Sensores - pines por definir
const byte PIN_SENS1 = 0;
const byte PIN_SENS2 = 0;

bool armado = false;

const byte PIN_BUZZ1 = 0;

void setup()
{
  Serial.begin(115200);
  pinMode(PIN_DISP1, OUTPUT);
  pinMode(PIN_DISP2, OUTPUT);
  pinMode(PIN_DISP3, OUTPUT);
  pinMode(PIN_DISP4, OUTPUT);
  /* La logica del modulo de relay está inverso,
  para que inicie en off se debe poner en HIGH en el inicio*/
  digitalWrite(PIN_DISP1, HIGH);
  digitalWrite(PIN_DISP2, HIGH);
  digitalWrite(PIN_DISP3, HIGH);
  digitalWrite(PIN_DISP4, HIGH);
  // Configuracion pin sensores como entradas
  pinMode(PIN_SENS1, INPUT);
  pinMode(PIN_SENS2, INPUT);

  EasyBuzzer.setPin(PIN_BUZZ1);

  Serial.print("conectando a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print("Tratando de conectar a la red");
  }
  Serial.println("");
  Serial.println("Conexion establecida");
  Serial.println("Direccion IP: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop()
{
  WiFiClient client = server.available();
  if (client)
  {
    Serial.println("Nuevo cliente conectado, solicitando pagina");
    String current_data_line = "";
    while (client.connected())
    {
      if (client.available())
      {
        char new_byte = client.read();
        Serial.write(new_byte);
        header += new_byte;
        if (new_byte == '\n')
        {
          if (current_data_line.length() == 0)
          {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Conexion: cerrada");
            client.println();
            // Dispositivo 1
            if (header.indexOf("DISP1=ON") != -1)
            {
              Serial.println("DISP1  ON");
              ESTADO_DISP1 = "on";
              digitalWrite(PIN_DISP1, LOW);
            }
            if (header.indexOf("DISP1=OFF") != -1)
            {
              Serial.println("DISP1  OFF");
              ESTADO_DISP1 = "off";
              digitalWrite(PIN_DISP1, HIGH);
            }
            // Dispositivo 2
            if (header.indexOf("DISP2=ON") != -1)
            {
              Serial.println("DISP2  ON");
              ESTADO_DISP2 = "on";
              digitalWrite(PIN_DISP2, LOW);
            }
            if (header.indexOf("DISP2=OFF") != -1)
            {
              Serial.println("DISP2  OFF");
              ESTADO_DISP2 = "off";
              digitalWrite(PIN_DISP2, HIGH);
            }
            // Dispositivo 3
            if (header.indexOf("DISP3=ON") != -1)
            {
              Serial.println("DISP3  ON");
              ESTADO_DISP3 = "on";
              digitalWrite(PIN_DISP3, LOW);
            }
            if (header.indexOf("DISP3=OFF") != -1)
            {
              Serial.println("DISP3  OFF");
              ESTADO_DISP3 = "off";
              digitalWrite(PIN_DISP3, HIGH);
            }
            // Dispositivo 4
            if (header.indexOf("DISP4=ON") != -1)
            {
              Serial.println("DISP4  ON");
              ESTADO_DISP4 = "on";
              digitalWrite(PIN_DISP4, LOW);
            }
            if (header.indexOf("DISP4=OFF") != -1)
            {
              Serial.println("DISP4  OFF");
              ESTADO_DISP4 = "off";
              digitalWrite(PIN_DISP4, HIGH);
            }
            // Armado alarma
            if (header.indexOf("ARM=ON") != -1)
            {
              Serial.println("ARMADO  ON");
              armado = true;
              ESTADO_ARM = "on";
            }
            if (header.indexOf("ARM=OFF") != -1)
            {
              Serial.println("ARMADO  OFF");
              armado = false;
              ESTADO_ARM = "off";
            }

            // HTML
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // ccs que nunca funcinó lmao
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #: 15px 32px; tex4CAF50; border: 2px solid #4CAF50;; color: white; paddingt-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin: 4px 2px; cursor: pointer;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println("</style></head>");
            client.println("<body><center><h1>SMART HOME</h1></center>");
            client.println("<center><h2>WEB SERVER VERSION V2</h2></center>");
            client.println("<form><center>");
            // botones
            client.println("<p>estado de DISP1: " + ESTADO_DISP1 + "</p>");
            client.println("<center> <button class=\"button\" name=\"DISP1\" value=\"ON\" type=\"submit\">DISP1 ON</button>");
            client.println("<button class=\"button\" name=\"DISP1\" value=\"OFF\" type=\"submit\">DISP1 OFF</button><br><br>");
            client.println("<p>estado de DISP2: " + ESTADO_DISP2 + "</p>");
            client.println("<button class=\"button\" name=\"DISP2\" value=\"ON\" type=\"submit\">DISP2 ON</button>");
            client.println("<button class=\"button\" name=\"DISP2\" value=\"OFF\" type=\"submit\">DISP2 OFF</button> <br><br>");
            client.println("<p>estado de DISP3: " + ESTADO_DISP3 + "</p>");
            client.println("<button class=\"button\" name=\"DISP3\" value=\"ON\" type=\"submit\">DISP3 ON</button>");
            client.println("<button class=\"button\" name=\"DISP3\" value=\"OFF\" type=\"submit\">DISP3 OFF</button> <br><br>");
            client.println("<p>estado de DISP4: " + ESTADO_DISP4 + "</p>");
            client.println("<button class=\"button\" name=\"DISP4\" value=\"ON\" type=\"submit\">DISP4 ON</button>");
            client.println("<button class=\"button\" name=\"DISP4\" value=\"OFF\" type=\"submit\">DISP4 OFF</button> <br><br>");
            client.println("<p>estado de alarma: " + ESTADO_ARM + "</p>");
            client.println("<button class=\"button\" name=\"ALARMA\" value=\"ON\" type=\"submit\">ARMADO</button>");
            client.println("<button class=\"button\" name=\"ALARMA\" value=\"OFF\" type=\"submit\">DESARMADO</button> <br><br>");
            client.println("</center></form></body></html>");
            client.println();
            break;
          }
          else
          {
            current_data_line = "";
          }
        }
        else if (new_byte != '\r')
        {
          current_data_line += new_byte;
        }
      }
    }

    header = "";
    // Cierra la conexion
    client.stop();
    Serial.println("Cliente Desconectado.");
    Serial.println("");
  }

  // verificar armado
  if (armado == true)
  {
    if (digitalRead(PIN_SENS1) == HIGH || digitalRead(PIN_SENS2) == HIGH)
    {
      //frecuencia, duracionOn, duracionOff, beeps, duracionPausa, secuencia
      EasyBuzzer.beep(2000, 100, 100, 2, 300, 1);
    }
  }
}