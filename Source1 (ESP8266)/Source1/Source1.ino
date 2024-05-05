// Thư viện
#include <WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// an giu nut boost khi nap code
// Chon ESP 32 Dev Module
// OLED (SCK) -> ESP32 (22-SDA)
// OLED (SDA) -> ESP32 (21-SCL)

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
#define DHTPIN 32
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define B1 15
#define B2 2
#define light 25
#define fan 26
#define light1 14
#define fan1 27
#define light2 35
#define fan2 34
String temperature;
String humidity;
float temp = 0;
float hum = 0;
int button1 = 0;
int button2 = 0;

// Thông tin của WIFI và Node-red
const char* ssid = "1505"; ///  wifi ssid
const char* password = "0918273645";
const char* mqtt_server = "test.mosquitto.org";// mosquitto server url
const char* HostName = "SmartHomeESP32"; // Không cần thiết
const int MAX_PAYLOAD = 60000;
bool useMQTT = true;

// Setup topic phù hợp cho từng thiết bị
#define sub1 "Liight1"
#define sub2 "Faan1"
#define sub3 "Light"
#define sub4 "Fan"
#define sub5 "Liiight2"
#define sub6 "Faaan2"
#define pub1 "Liight1/pub"
#define pub2 "Faan1/pub"
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
      digitalWrite(light1, HIGH); button1 = 1;
    } else {
      digitalWrite(light1, LOW); button1 = 0;
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
      digitalWrite(fan1, HIGH); button2 = 1;
    } else {
      digitalWrite(fan1, LOW); button2 = 0;
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
      digitalWrite(light, HIGH);
    } else {
      digitalWrite(light, LOW);
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
      digitalWrite(fan, HIGH);
    } else {
      digitalWrite(fan, LOW);
    }
  }
  if (strstr(topic, sub5))
  {
    for (int i = 0; i < length; i++)
    {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    if ((char)payload[0] == '1')
    {
      digitalWrite(light2, HIGH);
    } else {
      digitalWrite(light2, LOW);
    }
  }
  if (strstr(topic, sub6))
  {
    for (int i = 0; i < length; i++)
    {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    if ((char)payload[0] == '1')
    {
      digitalWrite(fan2, HIGH);
    } else {
      digitalWrite(fan2, LOW);
    }
  }
}

// Kiểm tra kết nối
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("Connected");
      client.subscribe(sub1);
      client.subscribe(sub2);
      client.subscribe(sub3);
      client.subscribe(sub4);
      client.subscribe(sub5);
      client.subscribe(sub6);
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
  pinMode(B1, INPUT_PULLUP);
  pinMode(B2, INPUT_PULLUP);
  pinMode(light, OUTPUT);
  pinMode(fan, OUTPUT);
  pinMode(light1, OUTPUT);
  pinMode(fan1, OUTPUT);
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
  display.println(F("Bed Room"));

  temperature = String(dht.readTemperature()-1);
  client.publish("Temperature1", temperature.c_str()); // publish temp topic /ThinkIOT/temp
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0, 8);            // Start at top-left corner
  display.print(F("T: "));
  display.print(dht.readTemperature()-1);
  display.println(F("*C"));

  humidity = String(dht.readHumidity());
  client.publish("Humidity1", humidity.c_str());   // publish hum topic /ThinkIOT/hum
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(70, 8);            // Start at top-left corner
  display.print(F("H: "));
  display.print(dht.readHumidity());
  display.println(F("%"));

  Serial.print("Temperature1: ");
  Serial.println(temperature);
  Serial.print("Humidity1: ");
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
      digitalWrite(light1, LOW);
      client.publish(pub1, "0");
    }
    if (button1 == HIGH)  {
      digitalWrite(light1, HIGH);
      client.publish(pub1, "1");
    }
  }
  if (button1 == LOW) {
    display.print(F("OFF"));
  }
  if (button1 == HIGH)  {
    display.print(F("ON"));
  }

  Serial.print("button1: ");
  Serial.println(button1);

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
      digitalWrite(fan1, LOW);
      client.publish(pub2, "0");
    }
    if (button2 == HIGH)  {
      digitalWrite(fan1, HIGH);
      client.publish(pub2, "1");
    }
  }
  if (button2 == LOW) {
    display.print(F("OFF"));
  }
  if (button2 == HIGH) {
    display.print(F("ON"));
  }

  Serial.print("button2: ");
  Serial.println(button2);

  display.display();

}
