// Version 0.2

#include <WiFi.h>

const char* ssid= "CH-Home"; 
const char* password = "hi5-L7-U06Zl0fPf";
WiFiServer server(80);

String header;

String ESTADO_DISP1 = "off";
String ESTADO_DISP2 = "off";
String ESTADO_DISP3 = "off";
String ESTADO_DISP4 = "off";


const int PIN_DISP1 = 14;
const int PIN_DISP2 = 27;
const int PIN_DISP3 = 26;
const int PIN_DISP4 = 25;


void setup() {
  Serial.begin(115200);
  pinMode(PIN_DISP1, OUTPUT);
  pinMode(PIN_DISP2, OUTPUT);
  pinMode(PIN_DISP3, OUTPUT);
  pinMode(PIN_DISP4, OUTPUT);

  digitalWrite(PIN_DISP1, HIGH);
  digitalWrite(PIN_DISP2, HIGH);
  digitalWrite(PIN_DISP3, HIGH);
  digitalWrite(PIN_DISP4, HIGH);


  Serial.print("conectando a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print("Tratando de conectar a la red");
  }
  Serial.println("");
  Serial.println("Conexion establecida");
  Serial.println("Direccion IP: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   
  if (client) {                            
    Serial.println("Nuevo cliente conectado, solicitando pagina");          
    String current_data_line = "";               
    while (client.connected()) {            
      if (client.available()) {             
        char new_byte = client.read();             
        Serial.write(new_byte);                    
        header += new_byte;
        if (new_byte == '\n') {                    
         
          if (current_data_line.length() == 0){
            
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Conexion: cerrada");
            client.println();
            
            if (header.indexOf("DISP1=ON") != -1){
              
              Serial.println("DISP1  ON");
              ESTADO_DISP1 = "on";
              digitalWrite(PIN_DISP1, LOW);
            } 
            if (header.indexOf("DISP1=OFF") != -1){
              
              Serial.println("DISP1  OFF");
              ESTADO_DISP1 = "off";
              digitalWrite(PIN_DISP1, HIGH);
            } 

            
            if (header.indexOf("DISP2=ON") != -1){
              
              Serial.println("DISP2  ON");
              ESTADO_DISP2 = "on";
              digitalWrite(PIN_DISP2, LOW);
            }
            if (header.indexOf("DISP2=OFF") != -1){
              
              Serial.println("DISP2  OFF");
              ESTADO_DISP2 = "off";
              digitalWrite(PIN_DISP2, HIGH);
            }

            
            if (header.indexOf("DISP3=ON") != -1){
              
              Serial.println("DISP3  ON");
              ESTADO_DISP3 = "on";
              digitalWrite(PIN_DISP3, LOW);
            }
            if(header.indexOf("DISP3=OFF") != -1) {
              Serial.println("DISP3  OFF");
              ESTADO_DISP3 = "off";
              digitalWrite(PIN_DISP3, HIGH);
            }


            if (header.indexOf("DISP4=ON") != -1){
              
              Serial.println("DISP4  ON");
              ESTADO_DISP4 = "on";
              digitalWrite(PIN_DISP4, LOW);
            }
            if(header.indexOf("DISP4=OFF") != -1) {
              Serial.println("DISP4  OFF");
              ESTADO_DISP4 = "off";
              digitalWrite(PIN_DISP4, HIGH);
            }

            //HTML
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: 2px solid #4CAF50;; color: white; padding: 15px 32px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin: 4px 2px; cursor: pointer; }");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");            
            // HEAD DE LA WEB
            client.println("</style></head>");
            client.println("<body><center><h1>SMART HOME</h1></center>");
            client.println("<center><h2>WEB SERVER VERSION V0.2</h2></center>" );
            client.println("<form><center>");
            client.println("<p>estado de DISP1: " + ESTADO_DISP1 + "</p>");    
            client.println("<center> <button class=\"button\" name=\"DISP1\" value=\"ON\" type=\"submit\">DISP1 ON</button>") ;
            client.println("<button class=\"button\" name=\"DISP1\" value=\"OFF\" type=\"submit\">DISP1 OFF</button><br><br>");
            client.println("<p>estado de DISP2: " + ESTADO_DISP2 + "</p>");
            client.println("<button class=\"button\" name=\"DISP2\" value=\"ON\" type=\"submit\">DISP2 ON</button>");
            client.println("<button class=\"button\" name=\"DISP2\" value=\"OFF\" type=\"submit\">DISP2 OFF</button> <br><br>");
            client.println("<p>estado de DISP3: " + ESTADO_DISP3 + "</p>");
            client.println ("<button class=\"button\" name=\"DISP3\" value=\"ON\" type=\"submit\">DISP3 ON</button>");
            client.println ("<button class=\"button\" name=\"DISP3\" value=\"OFF\" type=\"submit\">DISP3 OFF</button> <br><br>");
            client.println("<p>estado de DISP4: " + ESTADO_DISP4 + "</p>");
            client.println ("<button class=\"button\" name=\"DISP4\" value=\"ON\" type=\"submit\">DISP4 ON</button>");
            client.println ("<button class=\"button\" name=\"DISP4\" value=\"OFF\" type=\"submit\">DISP4 OFF</button> <br><br>");
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
}
