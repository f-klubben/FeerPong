#include "Arduino.h"
#include <WiFi.h>
#include <FS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#define GNDNUM 4
#define PWRNUM 5

String start = "<!DOCTYPE html>\n"
"<html>\n"
"<head>\n"
"    <script>\n"
"        function httpGet(pin)\n"
"        {\n"
"            console.log(\"test\");\n"
"            var xmlHttp = new XMLHttpRequest();\n"
"            xmlHttp.open( \"GET\", \"http://192.168.4.1/flip?led=\" + pin, true); // false for synchronous request\n"
"            xmlHttp.send( null );\n"
"            return xmlHttp.responseText;\n"
"        }\n"
"        setInterval(async function(){\n"
"            let res = await fetch(\"http://192.168.4.1/status\");\n"
"            sta = await res.text();\n"
"            sta = sta.split(' ');\n"
"            sta.pop();\n"
"            sta.forEach((x,i) => {\n"
"                obj = eval(('id'+i));\n"
"                if (x == 0) {\n"
"                    obj.style.fill = 'gray';\n"
"                } else if (i < 10) {\n"
"                    obj.style.fill = 'red';\n"
"                } else {\n"
"                    obj.style.fill = 'green';\n"
"                }\n"
"            });\n"
"        }, 300);\n"
"    </script>\n"
"</head>\n"
"<body>\n"
"\n"
"<div class=\"triangle-container2\">\n"
"\t  <svg height=\"500\" width=\"500\">\n"
"\t\t    <polygon points=\"250,340 100,0 400,0\" class=\"triangle\" />\n"
"\n"
"            <!-- 4 rows !--> \n"
"            <circle cx=\"140\" id='id0' cy=\"20\" r=\"20\" stroke=\"black\" stroke-width=\"3\" fill=\"red\" onclick=\"httpGet(0)\"/>\n"
"            <circle cx=\"213\" id='id1' cy=\"20\" r=\"20\" stroke=\"black\" stroke-width=\"3\" fill=\"red\" onclick=\"httpGet(1)\" />\n"
"            <circle cx=\"286\" id='id2' cy=\"20\" r=\"20\" stroke=\"black\" stroke-width=\"3\" fill=\"red\" onclick=\"httpGet(2)\" />\n"
"            <circle cx=\"360\" id='id3' cy=\"20\" r=\"20\" stroke=\"black\" stroke-width=\"3\" fill=\"red\" onclick=\"httpGet(3)\" />\n"
"\n"
"            <circle cx=\"180\" id='id4' cy=\"100\" r=\"20\" stroke=\"black\" stroke-width=\"3\" fill=\"red\" onclick=\"httpGet(4)\" />\n"
"            <circle cx=\"250\" id='id5' cy=\"100\" r=\"20\" stroke=\"black\" stroke-width=\"3\" fill=\"red\" onclick=\"httpGet(5)\" />\n"
"            <circle cx=\"320\" id='id6' cy=\"100\" r=\"20\" stroke=\"black\" stroke-width=\"3\" fill=\"red\" onclick=\"httpGet(6)\" />\n"
"\n"
"            <circle cx=\"215\" id='id7' cy=\"180\" r=\"20\" stroke=\"black\" stroke-width=\"3\" fill=\"red\" onclick=\"httpGet(7)\" />\n"
"            <circle cx=\"286\" id='id8' cy=\"180\" r=\"20\" stroke=\"black\" stroke-width=\"3\" fill=\"red\" onclick=\"httpGet(8)\" />\n"
"\n"
"            <circle cx=\"250\" id='id9' cy=\"270\" r=\"20\" stroke=\"black\" stroke-width=\"3\" fill=\"red\" onclick=\"httpGet(9)\" />\n"
"\t  </svg>\n"
"</div>\n"
"\n"
"\n"
"<div class=\"triangle-container\" >\n"
"\t  <svg height=\"500\" width=\"500\" style=\" transform: rotate(180deg);\">\n"
"\t\t    <polygon points=\"250,340 100,0 400,0\" class=\"triangle\" />\n"
"\n"
"            <!-- 4 rows !--> \n"
"            <circle cx=\"140\" id='id10' cy=\"20\" r=\"20\" stroke=\"black\" stroke-width=\"3\" fill=\"red\" onclick=\"httpGet(10)\"/>\n"
"            <circle cx=\"213\" id='id11' cy=\"20\" r=\"20\" stroke=\"black\" stroke-width=\"3\" fill=\"red\" onclick=\"httpGet(11)\" />\n"
"            <circle cx=\"286\" id='id12' cy=\"20\" r=\"20\" stroke=\"black\" stroke-width=\"3\" fill=\"red\" onclick=\"httpGet(12)\" />\n"
"            <circle cx=\"360\" id='id13' cy=\"20\" r=\"20\" stroke=\"black\" stroke-width=\"3\" fill=\"red\" onclick=\"httpGet(13)\" />\n"
"\n"
"            <circle cx=\"180\" id='id14' cy=\"100\" r=\"20\" stroke=\"black\" stroke-width=\"3\" fill=\"red\" onclick=\"httpGet(14)\" />\n"
"            <circle cx=\"250\" id='id15' cy=\"100\" r=\"20\" stroke=\"black\" stroke-width=\"3\" fill=\"red\" onclick=\"httpGet(15)\" />\n"
"            <circle cx=\"320\" id='id16' cy=\"100\" r=\"20\" stroke=\"black\" stroke-width=\"3\" fill=\"red\" onclick=\"httpGet(16)\" />\n"
"\n"
"            <circle cx=\"215\" id='id17' cy=\"180\" r=\"20\" stroke=\"black\" stroke-width=\"3\" fill=\"red\" onclick=\"httpGet(17)\" />\n"
"            <circle cx=\"286\" id='id18' cy=\"180\" r=\"20\" stroke=\"black\" stroke-width=\"3\" fill=\"red\" onclick=\"httpGet(18)\" />\n"
"\n"
"            <circle cx=\"250\" id='id19' cy=\"270\" r=\"20\" stroke=\"black\" stroke-width=\"3\" fill=\"red\" onclick=\"httpGet(19)\" />\n"
"\t  </svg>\n"
"</div>\n"
"\n"
"</body>\n"
"</html>\n"
"";

struct PinPair {
  int gndPin;
  int pwrPin;
  int on;
  PinPair(int g, int pw, int on): gndPin(g), pwrPin(pw), on(on) {};
  PinPair(int g, int pw): gndPin(g), pwrPin(pw), on(0) {};
};

PinPair mapper[20] = {
  PinPair(15,4),
  PinPair(15,17),
  PinPair(15,5),
  PinPair(15,18),
  PinPair(2,4),
  PinPair(15,16),
  PinPair(2,18),
  PinPair(2,17),
  PinPair(2,5),
  PinPair(2,16),

  PinPair(19,4),
  PinPair(19,16),
  PinPair(19,5),
  PinPair(19,18),
  PinPair(21,4),
  PinPair(19,17),
  PinPair(21,18),
  PinPair(21,16),
  PinPair(21,5),
  PinPair(21,17),
};

int iteration = 0;
int gndSet[] = {15, 2, 19, 21};
void UpdateGND(){
  digitalWrite(gndSet[iteration], 1);
  iteration++;
  iteration = iteration % GNDNUM;
  for(int i = 0; i < 20; i++) {
    PinPair p = mapper[i];
    if (p.gndPin == gndSet[iteration] && p.on) {
      digitalWrite(p.pwrPin, 1);
    } else if (p.gndPin == gndSet[iteration] && !p.on) {
      digitalWrite(p.pwrPin, 0);
    }
  }
  digitalWrite(gndSet[iteration], 0);
}

long last = 0;
void loopPins() {
  if(micros() >  last + 1000) {
    UpdateGND();
    last = micros();
  }
}

void TurnOn(int led){
  mapper[led].on = 1;
}

void TurnOff(int led){
  mapper[led].on = 0;
}

AsyncWebServer server(80);
int gameOn = 0;

int red[10] = {0,0,0,0,0,0,0,0,0,0};
int green[10] = {0,0,0,0,0,0,0,0,0,0};
int redActive = 0;
int greenActive = 0;

int i = 0;
long last2 = 0;
int interval = 1000;

int randomGreen() {
  int ran = random(0, 10);
  int check = 0;
  for (int i = 0; i < 10; i++) {
    check |= green[i];
  }
  if (!check) {
    TurnOff(greenActive);
    return 0;
  }
  if (green[ran]) {
    TurnOff(greenActive+10);
    greenActive = ran;
    TurnOn(greenActive+10);
    return 1;
  } else {
      return randomGreen();
  }
}

int randomRed() {
  int ran = random(0, 10);
  int check = 0;
  for (int i = 0; i < 10; i++) {
    check |= red[i];
  }
  if (!check) {
    TurnOff(redActive);
    return 0;
  }
  if (red[ran]) {
    TurnOff(redActive);
    redActive = ran;
    TurnOn(redActive);
    return 1;
  } else {
      return randomRed();
  }
}

void setup() {

  for(int i = 0; i < 10; i++) {
    red[i] = 1;
    green[i] = 1;
  }

  Serial.begin(115200);
  for (int i = 0; i < 20; i++) {
    PinPair p = mapper[i];
    pinMode(p.gndPin, OUTPUT);
    pinMode(p.pwrPin, OUTPUT);
    digitalWrite(p.gndPin, 1);
    digitalWrite(p.pwrPin, 0);
  }

  WiFi.softAP("FeerPong", "paaske");
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());


    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(200, "text/html", start);
    });

  server.on("/disable", HTTP_GET, [](AsyncWebServerRequest *request){
    AsyncWebParameter* p = request->getParam("led");
    int k = p->value().toInt();
    if (k >= 10) {
      green[k-10] = 0;
    } else {
      red[k] = 0;
    }
    request->send(200, "text/plain", "");
  });

  server.on("/flip", HTTP_GET, [](AsyncWebServerRequest *request){
    AsyncWebParameter* p = request->getParam("led");
    int k = p->value().toInt();
    if (k >= 10) {
      green[k-10] = !green[k-10];
    } else {
      red[k] = !red[k];
    }
    if (gameOn == 2) {
      randomGreen();
      randomRed();
    }
    request->send(200, "text/plain", "");
  });

  server.on("/interval", HTTP_GET, [](AsyncWebServerRequest *request){
    AsyncWebParameter* p = request->getParam("int");
    int k = p->value().toInt();
    interval = k;
    request->send(200, "text/plain", "");
  });

  server.on("/enable", HTTP_GET, [](AsyncWebServerRequest *request){
    AsyncWebParameter* p = request->getParam("led");
    int k = p->value().toInt();
    if (k >= 10) {
      green[k-10] = 1;
    } else {
      red[k] = 1;
    }
    request->send(200, "text/plain", "");
  });

  server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request){
    String t = "";
    for (int i = 0; i < 10; i++) {
      t += red[i];
      t += " ";
    }
    for (int i = 0; i < 10; i++) {
      t += green[i];
      t += " ";
    }


    request->send(200, "text/plain", t);
  });

  server.on("/gamemode", HTTP_GET, [](AsyncWebServerRequest *request){
    AsyncWebParameter* p = request->getParam("mode");
    for(int i = 0; i < 20; i++) {
      TurnOff(i);
    }
    int k = p->value().toInt();
    gameOn = k;
    for(int i = 0; i < 10; i++) {
      red[i] = 1;
      green[i] = 1;
    }
    if(k == 0) {
      for (int i = 0; i < 10; i++) {
        red[i] = 0;
        green[i] = 0;
      }
    }
    request->send(200, "text/plain", "");
  });

  server.begin();
  TurnOn(0);
}

void loop() {
  loopPins();
  if (gameOn == 2) {
    if (millis() > last2 + interval) {
      randomRed();
      randomGreen();
      last2 = millis();
    }
  } else {
    for(int i = 0; i < 10; i++) {
      if(red[i]) {
        TurnOn(i);
      } else {
        TurnOff(i);
      }
      if(green[i]) {
        TurnOn(i+10);
      }else {
        TurnOff(i+10);
      }
    }
  }

}
