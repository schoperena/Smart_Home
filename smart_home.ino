/*
Smart Home
Version 2: Se añaden sensores pir y funcion de alarma
Sebastian choperena Solano
*/

// Librerias necesrias para el funcionamientos del codigo
#include <EasyBuzzer.h> //Libreria para usar buzzer
#include <WiFi.h>       // Librerias para usar las funcionalidades wifi del modulo
// Variables datos wifi
const char *ssid = "nombre_red";
const char *password = "contraseña";
// Puerto del servidor web
WiFiServer server(80);
// Variable relacionada con las pagina web
String header;
// Variable de estado de los dispositivos
String ESTADO_DISP1 = "off";
String ESTADO_DISP2 = "off";
String ESTADO_DISP3 = "off";
String ESTADO_DISP4 = "off";
String ESTADO_ARM = "off";
// Varibales que guardan los pines de los Relays
const byte PIN_DISP1 = 14;
const byte PIN_DISP2 = 27;
const byte PIN_DISP3 = 33;
const byte PIN_DISP4 = 25;
// Variables que guardan los pines de los sensores
const byte PIN_SENS1 = 26;
const byte PIN_SENS2 = 17;
// Variable que guarda el estado de armado de la alarma(true - false)(verdadero - falso)
bool armado = false;
// Varibale que guarda el pin del buzzer
const byte PIN_BUZZ1 = 32;

void setup()
{
  Serial.begin(115200); // Iniciamos la comunicacion serial
  // Configuramos los pines de los relay como salidas
  pinMode(PIN_DISP1, OUTPUT);
  pinMode(PIN_DISP2, OUTPUT);
  pinMode(PIN_DISP3, OUTPUT);
  pinMode(PIN_DISP4, OUTPUT);
  /* La logica del modulo de relay está inverso,
  para que inicie en off se debe poner en HIGH en el inicio*/
  digitalWrite(PIN_DISP1, LOW);
  digitalWrite(PIN_DISP2, LOW);
  digitalWrite(PIN_DISP3, LOW);
  digitalWrite(PIN_DISP4, LOW);
  // Configuracion pines de los sensores como entradas
  pinMode(PIN_SENS1, INPUT);
  pinMode(PIN_SENS2, INPUT);
  // Definimos el pin del buzzer, usando el metodo de la libreria
  EasyBuzzer.setPin(PIN_BUZZ1);
  // Imprime que se esta conectando a: nombre_red
  Serial.print("conectando a ");
  Serial.println(ssid);
  // Inicia la conexion a la red wifi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print("Tratando de conectar a la red");
  }
  Serial.println("");
  Serial.println("Conexion establecida");
  // Imprime la direccion IP asignada
  Serial.println("Direccion IP: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop()
{
  // Este metodo debe ser llamado en loop para que funcione la libreria
  EasyBuzzer.update();
  // Comprueba si un cliente se conecta al servidor
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
              digitalWrite(PIN_DISP1, HIGH);
            }
            if (header.indexOf("DISP1=OFF") != -1)
            {
              Serial.println("DISP1  OFF");
              ESTADO_DISP1 = "off";
              digitalWrite(PIN_DISP1, LOW);
            }
            // Dispositivo 2
            if (header.indexOf("DISP2=ON") != -1)
            {
              Serial.println("DISP2  ON");
              ESTADO_DISP2 = "on";
              digitalWrite(PIN_DISP2, HIGH);
            }
            if (header.indexOf("DISP2=OFF") != -1)
            {
              Serial.println("DISP2  OFF");
              ESTADO_DISP2 = "off";
              digitalWrite(PIN_DISP2, LOW);
            }
            // Dispositivo 3
            if (header.indexOf("DISP3=ON") != -1)
            {
              Serial.println("DISP3  ON");
              ESTADO_DISP3 = "on";
              digitalWrite(PIN_DISP3, HIGH);
            }
            if (header.indexOf("DISP3=OFF") != -1)
            {
              Serial.println("DISP3  OFF");
              ESTADO_DISP3 = "off";
              digitalWrite(PIN_DISP3, LOW);
            }
            // Dispositivo 4
            if (header.indexOf("DISP4=ON") != -1)
            {
              Serial.println("DISP4  ON");
              ESTADO_DISP4 = "on";
              digitalWrite(PIN_DISP4, HIGH);
            }
            if (header.indexOf("DISP4=OFF") != -1)
            {
              Serial.println("DISP4  OFF");
              ESTADO_DISP4 = "off";
              digitalWrite(PIN_DISP4, LOW);
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
            // HTML - Pagina web
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
    // Se resetea la variable header
    header = "";
    // Cierra la conexion
    client.stop();
    Serial.println("Cliente Desconectado.");
    Serial.println("");
  }
  // verificar armado
  if (armado == true)
  {
    // Verifica que el sensor detecta movimiento
    if (digitalRead(PIN_SENS1) == HIGH)
    {
      // Hace Sonar el buzzer
      // frecuencia, duracionOn, duracionOff, beeps, duracionPausa, secuencia
      EasyBuzzer.beep(2000, 200, 200, 4, 100, 1);
    }
  }
}
