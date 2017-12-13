#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SD.h>
#define FTPWRITE
#define chipSelect 2


//Should be pulled down to ground through a 10kOhm resistor 
int sendPin = 2;
 
const char* ssid = "panoulu";

//MQTT credentials
const char* mqttServer = "m14.cloudmqtt.com";
const int mqttPort = 17353;
const char* mqttUser = "fbaeyadu";
const char* mqttPassword = "8IEoPV8J5yDI";

//FTP stuff
const char* host = "146.185.158.26";
const char* userName = "projekti";
const char* password = "projekti2017";

char haika, myrkky;
char server[] = "146.185.158.26";
int sleepTime;
int sendMode;

WiFiClient espClient;
PubSubClient client(espClient);

//File Operation
char fileName[13] = "IMAGE03.jpg";

//############################################
//DO NOT CHANGE THESE
char outBuf[128];
char outCount;
WiFiClient dclient;
 
void setup() {
 
  Serial.begin(115200);
  delay(50);

  pinMode(sendPin, INPUT);

 //Wifi setup
  WiFi.begin(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to ");
    Serial.println(ssid);
  }
  Serial.println("Connected to the WiFi network");
 
  client.setServer(mqttServer, mqttPort);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
 
      Serial.println("Connected");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }
  
  client.publish("esp/test", "Awake activated");

  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present"); // t�ss� on kortin luvun testaus ota se mukaan 
    // don't do anything more:
    return;
  }
  
}

void mqttAwake(){

  client.setServer(mqttServer, mqttPort);
  //client.setCallback(callback);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
 
      Serial.println("Connected!");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.println(client.state());
      delay(2000);
 
    }
  }
}

void mqttSleep(){
  client.setServer(mqttServer, mqttPort);

  while(!client.connected()){
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP8266Client", mqttUser, mqttPassword)){
      Serial.println("Connected!");
    } else {
      Serial.print("Failed with state");
      Serial.println(client.state());
      delay(2000);
    }
  }

  client.publish ("esp/test", "Deep-sleep Activated");
  ESP.deepSleep(sleepTime);
}

void sleep(){

  if(haika = 0){
    sleepTime = sleepTime;
  }

  if(haika = 1){
    sleepTime = 60e6;
  }

  if(haika = 2){
    sleepTime = 300e6;
  }

  if(haika = 3){
    sleepTime = 600e6;
  }

  if(haika = 4){
    sleepTime = 900e6;
  }

  if(haika = 5){
    sleepTime = 1800e6;
  }

  if(haika = 6){
    sleepTime = 2700e6;
  }
}

bool asd(){
  
  bool flag1 = 0;
  if (espClient.connect(server, 80)) {
    espClient.println("GET /lue.php"); // Make a HTTP request:
    espClient.println();
    int flag1 = asdd();
    return flag1;
  } else {
    // if you didn't get a connection to the server:
    Serial.print("Ei saatu yhteyttä serverille");
    Serial.println();
    delay(1000);
    return flag1;
  }
}

bool asdd(){
  
  bool flag2 = 1;
  delay(1000);
  if (!espClient.available()){
    flag2 = 0;
    Serial.print("Ei keretty ladata sivua");
    Serial.println();
    return flag2;
  }
   else{
    for (int i=0; i<2; i++){
        if (espClient.available()) {
           char c = espClient.read();
           //Serial.print(c); //debuggaus
           if (i==0){
             haika = c;
           }
           else{
             myrkky = c;
            }
          }
        }
  espClient.stop();
  return flag2;
  }
}

void tulostus(){
  
  Serial.print(haika);
  Serial.print(myrkky);
  Serial.println();
  Serial.print("Datan vastaanotto onnistui");
  Serial.println();
  //delay(10000);
}

void error (){
  bool flag = 0;
  int i = 0;
  while(!flag){
    flag = asd();
    i++;
    if(i==10){
      flag = 1;
      Serial.print("Siirryttiin pois loopista, datan vastaanotto epäonnistui useasti");
      Serial.println();
      //delay(10000);
      // flaggi shell scriptille, että laite ei saa tietoja tietokannasta.
      }
  }
  if (i<=9){
  tulostus();
  }
}

void sendPicture(){

  File f = SD.open(fileName);
    if (!f) {
      Serial.println("file open failed");
    }
  f.println("Success!!");
  f.close();

  if(doFTP()) Serial.println(F("FTP OK"));
  else Serial.println(F("FTP FAIL"));
  
}

void loop() {
  
  mqttAwake();
  sendMode = digitalRead(sendPin);
  if(sendMode == LOW){
    Serial.println("Sending the picture");
    sendPicture();
  }
  error();
  sleep();
  mqttSleep();
  
}

//#######################################
/*
 * This is the function where the magic happens.
 * all credit goes to SurferTim. This will open
 * and write the file to your FTP server
 */
byte doFTP()
{

    File fh = SD.open(fileName);
    if (!fh) {
      Serial.println("file open failed");
    }
  if (espClient.connect(host,21)) {
    Serial.println(F("Command connected"));
  } 
  else {
    fh.close();
    Serial.println(F("Command connection failed"));
    return 0;
  }

  if(!eRcv()) return 0;

  espClient.print("USER ");
  espClient.println(userName);
  
  if(!eRcv()) return 0;
  
  espClient.print("PASS ");
  espClient.println(password);
  
  if(!eRcv()) return 0;
  
  espClient.println("SYST");

  if(!eRcv()) return 0;

  espClient.println("Type I");

  if(!eRcv()) return 0;

  espClient.println("PASV");

  if(!eRcv()) return 0;

  char *tStr = strtok(outBuf,"(,");
  int array_pasv[6];
  for ( int i = 0; i < 6; i++) {
    tStr = strtok(NULL,"(,");
    array_pasv[i] = atoi(tStr);
    if(tStr == NULL)
    {
      Serial.println(F("Bad PASV Answer"));    

    }
  }

  unsigned int hiPort,loPort;
  hiPort=array_pasv[4]<<8;
  loPort=array_pasv[5]&255;
  Serial.print(F("Data port: "));
  hiPort = hiPort|loPort;
  Serial.println(hiPort);
  if(dclient.connect(host, hiPort)){
    Serial.println("Data connected");
  }
  else{
    Serial.println("Data connection failed");
    espClient.stop();
    fh.close();
  }
  
  espClient.print("STOR ");
  espClient.println(fileName);
  if(!eRcv())
  {
    dclient.stop();
    return 0;
  }
  Serial.println(F("Writing"));
  
  byte clientBuf[64];
  int clientCount = 0;
  
  while(fh.available())
  {
    clientBuf[clientCount] = fh.read();
    clientCount++;
  
    if(clientCount > 63)
    {
      dclient.write((const uint8_t *)clientBuf, 64);
      clientCount = 0;
    }
  }
  if(clientCount > 0) dclient.write((const uint8_t *)clientBuf, clientCount);

  dclient.stop();
  Serial.println(F("Data disconnected"));
  espClient.println();
  if(!eRcv()) return 0;

  espClient.println("QUIT");

  if(!eRcv()) return 0;

  espClient.stop();
  Serial.println(F("Command disconnected"));

  fh.close();
  Serial.println(F("File closed"));
  return 1;
}

byte eRcv()
{
  byte respCode;
  byte thisByte;

  while(!espClient.available()) delay(1);

  respCode = espClient.peek();

  outCount = 0;

  while(espClient.available())
  {  
    thisByte = espClient.read();    
    Serial.write(thisByte);

    if(outCount < 127)
    {
      outBuf[outCount] = thisByte;
      outCount++;      
      outBuf[outCount] = 0;
    }
  }

  if(respCode >= '4')
  {
    efail();
    return 0;  
  }

  return 1;
}


void efail()
{
  byte thisByte = 0;

  espClient.println(F("QUIT"));

  while(!espClient.available()) delay(1);

  while(espClient.available())
  {  
    thisByte = espClient.read();    
    Serial.write(thisByte);
  }

  espClient.stop();
  Serial.println(F("Command disconnected"));
}
