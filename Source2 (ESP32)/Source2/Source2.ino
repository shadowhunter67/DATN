// Thư viện
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// nap ko dc thi chinh uploadspeed xuong
// Chon NodeMCU 1.0
// Chon LEDBUILTIN 16
// OLED (SDA) -> ESP8266 (D2-SDA)
// OLED (SDK) -> ESP8266 (D1-SCL)

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16

// Logo Adfruit
static const unsigned char PROGMEM logo_bmp[] =
{ 0b00000000, 0b11000000,
  0b00000001, 0b11000000,
  0b00000001, 0b11000000,
  0b00000011, 0b11100000,
  0b11110011, 0b11100000,
  0b11111110, 0b11111000,
  0b01111110, 0b11111111,
  0b00110011, 0b10011111,
  0b00011111, 0b11111100,
  0b00001101, 0b01110000,
  0b00011011, 0b10100000,
  0b00111111, 0b11100000,
  0b00111111, 0b11110000,
  0b01111100, 0b11110000,
  0b01110000, 0b01110000,
  0b00000000, 0b00110000
};

// Định nghĩa cho các chân và gán kiểu dữ liệu cho từng biến
#include "DHT.h"
#define DHTPIN D4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define GAS A0
#define PIR D5
#define BUZZER D6
#define B1 D0
#define B2 D3
//#define light D5
//#define fan D6

String temperature;
String humidity;
String pir;
String gas;
int button1 = 0;
int button2 = 0;
int a = 1;
int b = 1;

// Thông tin của WIFI và Node-red
const char* ssid = "1505"; ///  wifi ssid
const char* password = "0918273645";
const char* mqtt_server = "test.mosquitto.org";// mosquitto server url
const char* HostName = "SmartHomeESP8266"; // Không cần thiết
const int MAX_PAYLOAD = 60000;
bool useMQTT = true;

// Setup topic phù hợp cho từng thiết bị
//#define sub "PIR"
//#define pub "PIR/pub"
#define sub1 "Light"
#define sub2 "Fan"
#define sub3 "PIR"
#define sub4 "Buzzer"
#define pub1 "Light/pub"
#define pub2 "Fan/pub"
WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;//----------------

// Setup WIFI
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// Đọc message được gửi từ Node-red
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  //  for (int i = 0; i < length; i++) {
  //    Serial.print((char)payload[i]);
  //  }

  if (strstr(topic, sub1))
  {
    for (int i = 0; i < length; i++)
    {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    if ((char)payload[0] == '1')
    {
      //      digitalWrite(light, HIGH);
      button1 = 1;
    } else {
      //      digitalWrite(light, LOW);
      button1 = 0;
    }
  }
  if (strstr(topic, sub2))
  {
    for (int i = 0; i < length; i++)
    {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    if ((char)payload[0] == '1')
    {
      //      digitalWrite(fan, HIGH);
      button2 = 1;
    } else {
      //      digitalWrite(fan, LOW);
      button2 = 0;
    }
  }
  if (strstr(topic, sub3))
  {
    for (int i = 0; i < length; i++)
    {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    if ((char)payload[0] == '1')
    {
      a = 1; delay(4000);
    } else {
      a = 0; pir = "0";
    }
  }
  if (strstr(topic, sub4))
  {
    for (int i = 0; i < length; i++)
    {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    if ((char)payload[0] == '1')
    {
      b = 0;
    } else {
      b = 1;
    }
  }
}

// Kiểm tra kết nối
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("Connected");
      client.subscribe(sub1);
      client.subscribe(sub2);
      client.subscribe(sub3);
      client.subscribe(sub4);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 3 seconds");
      delay(3000);
    }
  }
}

// Setup các chân linh kiện
void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  pinMode(PIR, INPUT);
  pinMode(GAS, INPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(B1, INPUT_PULLUP);
  pinMode(B2, INPUT);
  //  pinMode(light, OUTPUT);
  //  pinMode(fan, OUTPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(1000); // Pause for 1 seconds

  // Clear the buffer
  display.clearDisplay();

  // Draw a single pixel in white
  display.drawPixel(10, 10, SSD1306_WHITE);

  display.display();
  delay(1000);

  dht.begin();
}

// Đọc giá trị từ cảm biến và từ Message rồi in ra Serial và Oled, đồng thời cũng gửi dữ liệu lên Node-red
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  display.clearDisplay();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0, 0);            // Start at top-left corner
  display.println(F("Living Room"));

  temperature = String(dht.readTemperature()-2);
  client.publish("Temperature", temperature.c_str()); // publish temp topic /ThinkIOT/temp
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0, 8);            // Start at top-left corner
  display.print(F("T: "));
  display.print(dht.readTemperature()-2);
  display.println(F("*C"));

  humidity = String(dht.readHumidity());
  client.publish("Humidity", humidity.c_str());   // publish hum topic /ThinkIOT/hum
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(70, 8);            // Start at top-left corner
  display.print(F("H: "));
  display.print(dht.readHumidity());
  display.println(F("%"));

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print("  Humidity: ");
  Serial.println(humidity);

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0, 16);            // Start at top-left corner
  display.print(F("Light: "));
  if (digitalRead(B1) == 0) {
    button1++;delay(500);
    if (button1 > 1) {
      button1 = 0;
    }
    if (button1 == LOW) {
      //        digitalWrite(light, LOW);
      client.publish(pub1, "0");
    }
    if (button1 == HIGH)  {
      //        digitalWrite(light, HIGH);
      client.publish(pub1, "1");
    }
  }
  if (button1 == LOW) {
    display.print(F("OFF"));
  }
  if (button1 == HIGH)  {
    display.print(F("ON"));
  }
  Serial.print("Light: ");
  Serial.print(button1);

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(70, 16);            // Start at top-left corner
  display.print(F("Fan: "));
  if (digitalRead(B2) == 0) {
    button2++;delay(500);
    if (button2 > 1) {
      button2 = 0;
    }
    if (button2 == LOW) {
      //        digitalWrite(fan, LOW);
      client.publish(pub2, "0");
    }
    if (button2 == HIGH)  {
      //        digitalWrite(fan, HIGH);
      client.publish(pub2, "1");
    }
  }
  
  if(temperature>"35.0"){client.publish(pub2, "1");button2 == HIGH;}
  
  if (button2 == LOW) {
    display.print(F("OFF"));
  }
  if (button2 == HIGH) {
    display.print(F("ON"));
  }

  Serial.print("  Fan: ");
  Serial.println(button2);

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0, 24);            // Start at top-left corner
  display.println(F("Kitchen Room"));

  gas = String(analogRead(GAS));
  client.publish("Gas", gas.c_str());
  if (b == 1) {
    if (analogRead(GAS) > 100) {
      digitalWrite(BUZZER, HIGH);
      delay(2000);
      digitalWrite(BUZZER, LOW);
    }
    else {
      digitalWrite(BUZZER, LOW);
    }
  }
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0, 32);            // Start at top-left corner
  display.print(F("Gas: "));
  display.print(analogRead(GAS));

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(70, 32);            // Start at top-left corner
  display.print(F("Buz: "));
  if (b == LOW) {
    display.print(F("OFF"));
  }
  if (b == HIGH) {
    display.print(F("ON"));
  }

  Serial.print("GAS: ");
  Serial.print(gas);
  Serial.print("  buzzer: ");
  Serial.println(b);

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0, 40);            // Start at top-left corner
  display.println(F("Bath Room"));

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0, 48);            // Start at top-left corner
  display.print(F("Pir: "));
  display.print(digitalRead(PIR));

  if (a == 1) {
    pir = String(digitalRead(PIR));
    client.publish("Pir", pir.c_str());
  }

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(70, 48);            // Start at top-left corner
  display.print(F("PIR: "));
  if (a == LOW) {
    display.print(F("OFF"));
  }
  if (a == HIGH) {
    display.print(F("ON"));
  }

  Serial.print("Pir: ");
  Serial.print(digitalRead(PIR));
  Serial.print("  PIR: ");
  Serial.println(a);

  Serial.println(" ");
  display.display();
  
}
