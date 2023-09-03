
#include <ESP8266WiFi.h>

#define PORT  23
// Gioi han so luong clients ket noi
#define MAX_CLIENTS 3

// Ten va mat khau cua ESP8266 AP se tao
const char *ssid = "duong";
const char *password = "12345678";

//khoi tao IP adress
IPAddress local_IP(192, 168, 10, 1);
IPAddress gateway(192, 168, 10, 1);
IPAddress subnet(255, 255, 255, 0);

// Khoi tao port de clients ket noi.
WiFiServer server(PORT);
WiFiClient clients[MAX_CLIENTS];
//Coefficient value
int a = 1;
int b = 2;
int c = 1;
int d = 3;
int k=2;
float h1 = 0.0001,h11 = 0.0005; //Step size
float xm0, gm0, zm0;
float xm,gm, zm,gm1;
float xm2,gm2, zm2;
unsigned long currentTime, previousTime;
float currentMillis;
double eT;
const char*message="27.54,67,63";
int e=0;
float sats, message1;
float  firsttime,sendtime;
 int u;
 String tinnhan,gui;
//
void setup() {
 Serial.begin(115200);
  Serial.println();
  Serial.print("Setting soft-AP configuration ... ");

  //Cau hinh acces point, cai dat soft AP de client ket noi vao.
  WiFi.softAPConfig(local_IP, gateway, subnet);
  WiFi.softAP(ssid, password);

  //In ra local_IP cua AP.
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
  Serial.println("Telnet server started");
  server.begin();
  //Initial conditions
xm0=1;
gm0=1;
zm0=-1;
firsttime=micros();
}

void loop() {
  
  uint8_t i;


  // kiem tra co client moi ket noi khong
 
  if (server.hasClient())  {
    for (i = 0; i < MAX_CLIENTS; i++) {
      if (!clients[i] || !clients[i].connected())
      { if (clients[i]) clients[i].stop();
        clients[i] = server.available();
        Serial.println("New client: "); Serial1.print(i);
        continue;
      }
    }
    WiFiClient serverClient = server.available();
    serverClient.stop();
  }
  // Kiem tra neu so client ket noi MAX_CLIENTS
  // co client, client duoc ket noi va o trang thai available
  // doc du lieu tu client, va gui lai du lieu cho client do.
  for (i = 0; i < MAX_CLIENTS; i++) {
    if (clients[i] && clients[i].connected()) {
      if (clients[i].available()) {
        String line = clients[i].readStringUntil('\r'); 
        if (line.length() > 1)
        {
        //  fisrttime=micros();
            
 xm2=xm0*10000;
 gm2=gm0*10000;
 zm2=zm0*10000;
      clients[i].write(String(xm2).c_str());
         clients[i].write(",");
        clients[i].write(String(gm2).c_str());
         clients[i].write(",");
        clients[i].write(String(zm2).c_str());
         clients[i].write(",");
        clients[i].write(String(gm1).c_str());
        clients[i].write("\r");   
      Serial.print(xm0,6);
Serial.print(",");
      Serial.print(gm0,6);
Serial.print(",");
      Serial.print(zm0,6);
Serial.print(",");
Serial.println(micros());
//xm=xm0+h1*(-a*xm0+b*gm0);
//gm=gm0+h1*(-k*xm0*zm0);
//zm=zm0+h1*(c*k*xm0*xm0+k*xm0*gm0-d/k);

xm=xm0+h1*(-a*xm0+b*gm0);
gm=gm0+h1*(-k*xm0*zm0);
zm=zm0+h1*(c*k*xm0*xm0+k*xm0*gm0-d/k);

 xm0=xm;
 gm0=gm;
 zm0=zm;
 gm1=gm;
message1=0;
 sats=xm0/gm0;
        if (sats>1)
        sats=1;
        else if (sats<-1)
        sats=-1;
        else
        sats;
//        Serial.print(xm0);
//        Serial.print(",");
//        Serial.print(gm0);
//        Serial.print(",");
//        Serial.println(sats);
//sendtime=micros()-firsttime;

    if (micros()> 19000000 & micros()<125000000 ){
   if (micros()-2000000>19000000+e*2000000)
   {
   if ( e<strlen(message))
{
 gm1=gm1+(int)message[e];
 message1 = message1+(int)message[e]; 
gui=gui+char(round(gm1));
tinnhan+=char(round(message1));
 e=e+1;
}
else
{
 e=strlen(message)+1;
} 
//
   }

////Serial.print(gm1);
////   Serial.print(",");
////
////  Serial.print(message1);
////   Serial.print(",");
////Serial.println(micros());

    }
    if (micros()> 125000000 & u==0) {
      Serial.println(gui);
      Serial.println(tinnhan);
      u=1;
    }
  
//   Serial.print(gm1);
//   Serial.print(",");
//
//  Serial.print(message1);
////  Serial.print(WiFi.macAddress());
// Serial.print(",");
//Serial.println(micros());

      line="";
        }
      }
    }
  }
}
