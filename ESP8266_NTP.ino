#define BUFFER 512 // size of the buffer

#define SSID   "xxxxxxxx"  // Put your SSID and Password here
#define PASS   "xxxxxxxx"

char buffer[BUFFER];

#define DEBUG Serial      // Send debug messages to serial monitor
#define ESP8266 Serial1   // Use Serial1 to talk to ESP8266 (Mega)

// By default we are looking for OK\r\n
char OKrn[] = "OK\r\n";

void setup() {
  ESP8266.begin(115200);
  
  DEBUG.begin(9600);
  DEBUG.println("Starting...");
    
  initESP8266();
}

byte waitForResponse(int timeout, char* term=OKrn) {
  unsigned long t=millis();
  bool found=false;
  int i=0;
  int len=strlen(term);
  // wait for at most timeout milliseconds
  // or if OK\r\n is found
  while(millis()<t+timeout) {
    if(ESP8266.available()) {
      buffer[i++]=ESP8266.read();
      if(i>=len) {
        if(strncmp(buffer+i-len, term, len)==0) {
          found=true;
          break;
        }
      }
    }
  }
  buffer[i]=0;
  DEBUG.print(buffer);
  return found;
}

void initESP8266() {
   // reset module
  ESP8266.println("AT+RST");
  waitForResponse(3000);
 
  // set mode 1 (client)
  ESP8266.println("AT+CWMODE=1");
  waitForResponse(1000);  
 
  // Join the Access Point
  ESP8266.print("AT+CWJAP=\"");
  ESP8266.print(SSID);
  ESP8266.print("\",\"");
  ESP8266.print(PASS);
  ESP8266.println("\"");
  waitForResponse(5000);
  
  // Start SNTP server
  ESP8266.println("AT+CIPNTP=0");
  waitForResponse(10000);  
}

void loop() { 
  // Reqyest time from SNTP server
  ESP8266.print("AT+CIPNTP?");
  waitForResponse(1000);
  delay(60000); // Check for time once a minute 
}
