#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

char buf[5]="a1d1";
char webbuf[100];
char *strings[30];

int ind[4];
WiFiClient client;
LiquidCrystal_I2C lcd(0x3F,16,2);
#define port 80

const char* ssid = "KANAK_P_NET";
const char* password = "kanak123456";

//const char* server = "192.168.1.108";  // server's address
//const char* resource = "/parseToJson.php?vim=3&vcu=10&vru=20";


const char* server = "192.168.1.104";  // server's address
const char* resource = "/EnergyHackathon/parseToJson.php?vmi=3&vcua=10&vrua=20&vcud=49&vrud=78&vtu=39";

const char* resource2 = "/EnergyHackathon/parseToJson.php?vmi=";
const char* resource3="3&vcua=";
const char* resource4="10&vrua=";
const char* resource5="&vcud=";
const char* resource6="&vrud=";
const char* resource7="&vtu=";

const unsigned long HTTP_TIMEOUT = 10000;  // max respone time from server
const size_t MAX_CONTENT_SIZE = 512;       // max size of the HTTP response

// The type of data that we want to extract from the page
struct UserData {
  char meter_id[32];
  char current_usage[32];
};

void setup()
{
//  Wire.begin(); 
  pinMode(A0, INPUT);
  Serial.begin(115200);
  Serial.println();
  lcd.init();
  lcd.backlight();
  Serial.printf("Connecting to %s ", ssid);
  lcd.print("Connecting to");
  lcd.setCursor(0, 1);
  lcd.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFI-connected");
}


void loop() {
  if (connect(server)) {
    if (sendRequest(server, resource) && skipResponseHeaders()) {
      UserData userData;
      if (readReponseContent(&userData)) {
        printUserData(&userData);
      }
    }
  }
  disconnect();
  Wire.requestFrom(8, 8);    // request 6 bytes from slave device #8
  while (Wire.available()) { // slave may send less than requested
  char c = Wire.read(); // receive a byte as character
  Serial.print(c);         // print the character
  }
  //  data.toCharArray(buf, data.length());
  Serial.print("buf=");
  Serial.print(buf);
  Serial.println("i2c data sending");
  Wire.beginTransmission(8); // transmit to device #9
  Wire.write(buf,5); // sends x
  Wire.endTransmission();
  wait();
}


bool connect(const char* hostName) {
  Serial.print("Connect to ");
  Serial.println(hostName);

  bool ok = client.connect(hostName, port);

  Serial.println(ok ? "Connected" : "Connection Failed!");
  if(ok==true){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ESC Conected");
  }
  else{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ESC Failed");  
  }
  return ok;
}

// Send the HTTP GET request to the server
bool sendRequest(const char* host, const char* resource) {
  Serial.print("GET ");
  Serial.println(resource);

  client.print("GET ");
  client.print(resource);
  client.println(" HTTP/1.0");
  client.print("Host: ");
  client.println(host);
  client.println("Connection: close");
  client.println();

  return true;
}

// Skip HTTP headers so that we are at the beginning of the response's body
bool skipResponseHeaders() {
  // HTTP headers end with an empty line
  char endOfHeaders[] = "\r\n\r\n";

  client.setTimeout(HTTP_TIMEOUT);
  bool ok = client.find(endOfHeaders);

  if (!ok) {
    Serial.println("No response or invalid response!");
  }

  return ok;
}

bool readReponseContent(struct UserData* userData) {

  while(client.connected())
  {
    while(client.available())
    {
      char inchar = client.read();
      if(inchar=='#'){
        int i=0;
        while(client.available()){
          inchar = client.read();
          webbuf[i]=inchar;
          i++;
        }
//        Serial.println("full buffer=");
//        Serial.println(webbuf);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Data Processed");

        int j=0;
        int k=1;
        for(j=0;j<=i;j++){
          if(webbuf[j]=='#'){
            ind[k]=j;
            k++;
          }
        }
      }
    }
  }
 
    char delims[] = "#";

    int x = 0;
    strings[x] = strtok( webbuf, delims );
    while( strings[x] != NULL  ) 
    {
        strings[++x] = strtok( NULL, delims );          
    }
       for ( int y = 0; y < x; y++ ) 
    {
        Serial.printf("%d '%s'\n", y, strings[y]);
    }
Serial.print(strings[0][5]);
    
if(strings[0][5]=='n'){
  buf[1]='1';
}
else{
    buf[1]='0';
}

if(strings[1][5]=='n'){
  buf[3]='1';
}
else{
    buf[3]='0';
}

lcd.clear();
lcd.setCursor(0, 0);
lcd.print(strings[0]);
lcd.setCursor(0, 1);
lcd.print(strings[1]);
delay(3000);

lcd.clear();
lcd.setCursor(0, 0);
lcd.print(strings[2]);
lcd.setCursor(0, 1);
lcd.print(strings[3]);
delay(3000);

lcd.clear();
lcd.setCursor(0, 0);
lcd.print(strings[4]);
lcd.setCursor(0, 1);
lcd.print(strings[5]);
delay(3000);



  
  return true;
}

// Print the data extracted from the JSON
void printUserData(const struct UserData* userData) {
//  lcd.clear();
//  Serial.print("meter_id ");
//  Serial.println(userData->meter_id);
//  lcd.print("meter_id");
//  lcd.setCursor(8, 0);
//  lcd.print(userData->meter_id);
//  Serial.print("current_usage");
//  Serial.println(userData->current_usage);
//  lcd.setCursor(0, 1);
//  lcd.print("current_usage");
//  lcd.setCursor(7, 1);
//  lcd.print(userData->current_usage);
}

// Close the connection with the HTTP server
void disconnect() {
  Serial.println("Disconnect");
  client.stop();
}

// Pause for a 1 minute
void wait() {
  Serial.println("Wait 60 seconds");
  delay(6000);
}



