#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <FS.h>

// Rede Wifi
char* const ssid = "???????????";
char* const password = "???????????";

char* const filenameFoto = "/foto.json"; // Faça upload do arquivo no menu: Tools -> "ESP8266 Sketch Data Upload"

// AdaFruit IO
char* const urlFotoAdaFruitIO = "http://io.adafruit.com/api/v2/<user>/feeds/<feed_name>/data";                             
char* const keyAdaFruitIO = "????????????????????????????????";                            

void setup() {
  Serial.begin(115200);

  // Inicia sistema de arquivos
  initFileSystem();  

  // Inicia conexão WIFI
  initWifiConnection();

  // Lista arquivos do diretorio
  listFilesDirectory();

  // Envia a foto para a plataforma http://io.adafruit.com
  sendDataToAdafruit();
}

// Inicia sistema de arquivos
void initFileSystem()
{
  Serial.println();
  Serial.print("Abrindo sistema de arquivos da memoria FLASH...");
  if (SPIFFS.begin()) {
    Serial.println("OK");
  }
  else {
    Serial.println("Falha ao abrir sistema de arquivos da memoria FLASH!");
    return;
  }    
}

// Inicia conexão WIFI
void initWifiConnection()
{
  Serial.println();
  Serial.printf("Conectando na rede Wifi [%s]...\r\n", ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());   
}

// Lista todos os arquivos do diretório
void listFilesDirectory()
{
  Dir dir = SPIFFS.openDir("/");
  while (dir.next()) {
      Serial.print(dir.fileName());
      Serial.print(": ");
      File f = dir.openFile("r");
      Serial.println(f.size());
      f.close();
  }
}

// Envia a foto para a plataforma http://io.adafruit.com
void sendDataToAdafruit()
{
    Serial.println();
    Serial.println("Enviando dados para a plataforma AdaFruit IO...");

    HTTPClient http;

    Serial.printf("[POST] %s \n", urlFotoAdaFruitIO);
    http.begin(urlFotoAdaFruitIO);
    http.addHeader("X-AIO-Key", keyAdaFruitIO);
    http.addHeader("Content-Type", "application/json");

    File file = SPIFFS.open(filenameFoto, "r");    
          
    int httpCode = http.sendRequest("POST", &file, file.size());
    Serial.printf("HTTP Code: %d \n", httpCode);
    if (httpCode == 200) {
      Serial.println("Arquivo enviado para o Adafruit IO co sucesso!");
    }
    else {
      Serial.println("Falha ao enviar o arquivo!");
    }

    http.end();

    Serial.println();
}

void loop() {
}




