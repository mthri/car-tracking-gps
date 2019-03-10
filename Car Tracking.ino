/*
    --------------------------
   |   Githube: a-motahari    |
    --------------------------

Connections:


  GSM a6 mini:
  
    VCC => 5v
    G   => GND
    URX => 11
    UTX => 10


   GPS NEO-6M:
    VCC => 3.3v
    GND => GND
    RX  => 3 
    TX  => 2


    When you send lc_psw_12345 by sms to this device ,recive link location
    
*/
//#include <Arduino.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

SoftwareSerial mySerial(10, 11); //A6 Tx & Rx is connected to Arduino #10 & #11
TinyGPSPlus gps;
SoftwareSerial gpsSerial(2, 3); //GPS Tx & Rx is connected to Arduino #2 & #3
String GsmResponse = "";
String PhoneNumber = "";
String PhoneSms="";
String Cmd_Psw="";
String Pswrd="";
String Command="";

//------------------GPS---------------------//
String Latitude = "NULL";
String Longitude = "NULL";
String Link_Location = "NULL";
//------------------------------------------//


void setup()
{
 Serial.begin(9600);
  mySerial.begin(9600);
  gpsSerial.begin(9600);
  gpsSerial.end();
  mySerial.listen();

  Serial.println("Initializing..."); 
  mySerial.println("AT");
  delay(100);
  mySerial.println("AT+CMGF=1");
  delay(100);
  mySerial.println("AT+CNMI=1,2,0,0,0");
  delay(100);
  SmsClear(); 
}
 
void loop()
{   
  if(mySerial.isListening())   
    GsmSerialFunction();
}
 
void GsmSerialFunction()
{
  delay(500);
  while (Serial.available()) 
  {
    mySerial.println(Serial.read());//Forward what Serial received to Software Serial Port
  }

  //Gsm Response
  if(mySerial.available())
  {
    GsmResponse = "";
    PhoneNumber = "";
    
    while(mySerial.available()) 
    {
      GsmResponse += String((char)mySerial.read());
    }
    
    GsmResponse.trim();
    //if receve the call
    if(GsmResponse == "RING")
    {
      mySerial.println("ATH");
      delay(300);
    } 
    //if is not called
    else
    {
      if(GsmResponse.length() > 47)
      {
        if(GsmResponse.substring(0,4) == "+CMT")
        {
          SmsSplit(GsmResponse);
        }
       else
        Serial.println(GsmResponse);
      }
      
    }
  }
}


void SmsSplit(String data)
{
  SmsClear();
  PhoneSms  = data.substring(10,20);
  Cmd_Psw    = data.substring(49);
  Pswrd = Cmd_Psw.substring(2);
  Command   = Cmd_Psw.substring(0,2);

  String txt="";
  if(Command == "lc")
  {
    if(Pswrd == "_psw_12345")
    {
      mySerial.end();
      gpsSerial.listen();
      txt = GetLocation();
      SendLocation(txt,PhoneSms);
    }
    else
      Serial.println("Wrong password.");
  }
  else
    Serial.println("Unkonwe Command");
  SmsClear();
}

String GetLocation()
{
  bool ch = true;
  while(ch)
  {
  while (gpsSerial.available() > 0)
      if (gps.encode(gpsSerial.read()))
        {
            if (gps.location.isValid())
            {
              ch = false;
              gpsSerial.end();
              mySerial.listen();
              
              Latitude = String(gps.location.lat(),6);
              Longitude = String(gps.location.lng(),6);
              Link_Location = "https://www.google.com/maps/search/";
              Link_Location += Latitude;    
              Link_Location += "+";
              Link_Location += Longitude;
              Serial.println(Link_Location);
            }
            else
            {
              Serial.println("Location: Not Available");
            }

            delay(1000);
        }
  }

  return Link_Location;
}

//This function will delete all SMS
void SmsClear()
{
  mySerial.println("AT+CMGD=1,4");
  delay(100);
}

void SendLocation(String TextMessage,String Number)
{
  Serial.println("SendLocation");
  Serial.println(TextMessage);
  String Nmbr="AT+CMGS=\"+xx"; // **Important**   Replace xx with your country code
  Nmbr += Number;
  Nmbr += "\"";
  mySerial.println("AT");
  delay(100);
  mySerial.println("AT+CMGF=1");
  delay(100);
  mySerial.println(Nmbr);
  GsmSerialFunction();
  mySerial.print(TextMessage);
  GsmSerialFunction();
  mySerial.write(26);
}
