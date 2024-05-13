// Thư viện
#include <WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <math.h>
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
#define DHTPIN 5
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
//Thay đổi: 2 đèn, 2 quạt, 2 nút nhấn, dht11, PIR, cảm biến ánh sáng, MQ2, còi
#define B1 4
#define B2 15
#define B3 19
#define B4 23
#define light 18
#define light1 13
#define fan1 12
#define light2 14
#define fan2 27
#define PIR 33
#define GAS 34
#define BUZZER 32
String temperature;
String humidity;
String pir;
String gas;
String Lightsensor;
int button = 0;
int button1 = 0;
int button2 = 0;
int a = 1;
int b = 1;
int c = 0; // c=0: chế độ thủ công, c=1: chế độ tự động
float d = 33;
int dem = 0;
int dem1 = 0;
int dem2 = 0;
int dem3 = 0;
int Light;
int Fan;
int value;
int value1;
int delay5 = 1;
int delay6 = 1;
int delay7 = 1;
int delay8 = 1;

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
//#define sub5 "Liiight2"
//#define sub6 "Faaan2"
#define sub7 "Mode"
#define sub8 "Vaalue1"
#define sub9  "delay5"
#define sub10 "deelay6"
#define sub11 "delaay7"
#define sub12 "deelaay8"
#define sub13 "PIR"
#define sub14 "Buzzer"
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
      Light = 1;
    } else {
      Light = 0;
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
      Fan = 1;
    } else {
      Fan = 0;
    }
  }
  //  if (strstr(topic, sub5))
  //  {
  //    for (int i = 0; i < length; i++)
  //    {
  //      Serial.print((char)payload[i]);
  //    }
  //    Serial.println();
  //    if ((char)payload[0] == '1')
  //    {
  //      digitalWrite(light2, HIGH);
  //    } else {
  //      digitalWrite(light2, LOW);
  //    }
  //  }
  //  if (strstr(topic, sub6))
  //  {
  //    for (int i = 0; i < length; i++)
  //    {
  //      Serial.print((char)payload[i]);
  //    }
  //    Serial.println();
  //    if ((char)payload[0] == '1')
  //    {
  //      digitalWrite(fan2, HIGH);
  //    } else {
  //      digitalWrite(fan2, LOW);
  //    }
  //  }
  if (strstr(topic, sub7))
  {
    for (int i = 0; i < length; i++)
    {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    if ((char)payload[0] == '1')
    {
      c = 1;
    } else {
      c = 0;
    }
  }
  if (strstr(topic, sub8))
  {
    for (int i = 0; i < length; i++)
    {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    d = ((int)payload[0] - 48) * 10 + ((int)payload[1] - 48);
  }

  if (strstr(topic, sub9))
  {
    for (int i = 0; i < length; i++)
    {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    if (length == 7) {
      delay5 = ((int)payload[0] - 48) * 1000000 + ((int)payload[1] - 48) * 100000 + ((int)payload[2] - 48) * 10000 + ((int)payload[3] - 48) * 1000 + ((int)payload[4] - 48) * 100 + ((int)payload[5] - 48) * 10 + ((int)payload[6] - 48) * 1;
    }
    if (length == 6) {
      delay5 = ((int)payload[0] - 48) * 100000 + ((int)payload[1] - 48) * 10000 + ((int)payload[2] - 48) * 1000 + ((int)payload[3] - 48) * 100 + ((int)payload[4] - 48) * 10 + ((int)payload[5] - 48) * 1;
    }
    if (length == 5) {
      delay5 = ((int)payload[0] - 48) * 10000  + ((int)payload[1] - 48) * 1000  + ((int)payload[2] - 48) * 100  + ((int)payload[3] - 48) * 10  + ((int)payload[4] - 48) * 1;
    }
    if (length == 4) {
      delay5 = ((int)payload[0] - 48) * 1000   + ((int)payload[1] - 48) * 100   + ((int)payload[2] - 48) * 10   + ((int)payload[3] - 48) * 1;
    }
    if (length == 3) {
      delay5 = ((int)payload[0] - 48) * 100    + ((int)payload[1] - 48) * 10    + ((int)payload[2] - 48) * 1;
    }
    if (length == 2) {
      delay5 = ((int)payload[0] - 48) * 10     + ((int)payload[1] - 48) * 1;
    }
    if (length == 1) {
      delay5 = ((int)payload[0] - 48) * 1;
    }
  }
  if (strstr(topic, sub10))
  {
    for (int i = 0; i < length; i++)
    {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    if (length == 7) {
      delay6 = ((int)payload[0] - 48) * 1000000 + ((int)payload[1] - 48) * 100000 + ((int)payload[2] - 48) * 10000 + ((int)payload[3] - 48) * 1000 + ((int)payload[4] - 48) * 100 + ((int)payload[5] - 48) * 10 + ((int)payload[6] - 48) * 1;
    }
    if (length == 6) {
      delay6 = ((int)payload[0] - 48) * 100000 + ((int)payload[1] - 48) * 10000 + ((int)payload[2] - 48) * 1000 + ((int)payload[3] - 48) * 100 + ((int)payload[4] - 48) * 10 + ((int)payload[5] - 48) * 1;
    }
    if (length == 5) {
      delay6 = ((int)payload[0] - 48) * 10000  + ((int)payload[1] - 48) * 1000  + ((int)payload[2] - 48) * 100  + ((int)payload[3] - 48) * 10  + ((int)payload[4] - 48) * 1;
    }
    if (length == 4) {
      delay6 = ((int)payload[0] - 48) * 1000   + ((int)payload[1] - 48) * 100   + ((int)payload[2] - 48) * 10   + ((int)payload[3] - 48) * 1;
    }
    if (length == 3) {
      delay6 = ((int)payload[0] - 48) * 100    + ((int)payload[1] - 48) * 10    + ((int)payload[2] - 48) * 1;
    }
    if (length == 2) {
      delay6 = ((int)payload[0] - 48) * 10     + ((int)payload[1] - 48) * 1;
    }
    if (length == 1) {
      delay6 = ((int)payload[0] - 48) * 1;
    }
  }
  if (strstr(topic, sub11))
  {
    for (int i = 0; i < length; i++)
    {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    if (length == 7) {
      delay7 = ((int)payload[0] - 48) * 1000000 + ((int)payload[1] - 48) * 100000 + ((int)payload[2] - 48) * 10000 + ((int)payload[3] - 48) * 1000 + ((int)payload[4] - 48) * 100 + ((int)payload[5] - 48) * 10 + ((int)payload[6] - 48) * 1;
    }
    if (length == 6) {
      delay7 = ((int)payload[0] - 48) * 100000 + ((int)payload[1] - 48) * 10000 + ((int)payload[2] - 48) * 1000 + ((int)payload[3] - 48) * 100 + ((int)payload[4] - 48) * 10 + ((int)payload[5] - 48) * 1;
    }
    if (length == 5) {
      delay7 = ((int)payload[0] - 48) * 10000  + ((int)payload[1] - 48) * 1000  + ((int)payload[2] - 48) * 100  + ((int)payload[3] - 48) * 10  + ((int)payload[4] - 48) * 1;
    }
    if (length == 4) {
      delay7 = ((int)payload[0] - 48) * 1000   + ((int)payload[1] - 48) * 100   + ((int)payload[2] - 48) * 10   + ((int)payload[3] - 48) * 1;
    }
    if (length == 3) {
      delay7 = ((int)payload[0] - 48) * 100    + ((int)payload[1] - 48) * 10    + ((int)payload[2] - 48) * 1;
    }
    if (length == 2) {
      delay7 = ((int)payload[0] - 48) * 10     + ((int)payload[1] - 48) * 1;
    }
    if (length == 1) {
      delay7 = ((int)payload[0] - 48) * 1;
    }
  }
  if (strstr(topic, sub12))
  {
    for (int i = 0; i < length; i++)
    {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    if (length == 7) {
      delay8 = ((int)payload[0] - 48) * 1000000 + ((int)payload[1] - 48) * 100000 + ((int)payload[2] - 48) * 10000 + ((int)payload[3] - 48) * 1000 + ((int)payload[4] - 48) * 100 + ((int)payload[5] - 48) * 10 + ((int)payload[6] - 48) * 1;
    }
    if (length == 6) {
      delay8 = ((int)payload[0] - 48) * 100000 + ((int)payload[1] - 48) * 10000 + ((int)payload[2] - 48) * 1000 + ((int)payload[3] - 48) * 100 + ((int)payload[4] - 48) * 10 + ((int)payload[5] - 48) * 1;
    }
    if (length == 5) {
      delay8 = ((int)payload[0] - 48) * 10000  + ((int)payload[1] - 48) * 1000  + ((int)payload[2] - 48) * 100  + ((int)payload[3] - 48) * 10  + ((int)payload[4] - 48) * 1;
    }
    if (length == 4) {
      delay8 = ((int)payload[0] - 48) * 1000   + ((int)payload[1] - 48) * 100   + ((int)payload[2] - 48) * 10   + ((int)payload[3] - 48) * 1;
    }
    if (length == 3) {
      delay8 = ((int)payload[0] - 48) * 100    + ((int)payload[1] - 48) * 10    + ((int)payload[2] - 48) * 1;
    }
    if (length == 2) {
      delay8 = ((int)payload[0] - 48) * 10     + ((int)payload[1] - 48) * 1;
    }
    if (length == 1) {
      delay8 = ((int)payload[0] - 48) * 1;
    }
  }

  if (strstr(topic, sub13))
  {
    for (int i = 0; i < length; i++)
    {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    if ((char)payload[0] == '1')
    {
      a = 1; delay(2000);
    } else {
      a = 0; pir = "0";
    }
  }

  if (strstr(topic, sub14))
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
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("Connected");
      client.subscribe(sub1);
      client.subscribe(sub2);
      client.subscribe(sub3);
      client.subscribe(sub4);
      //      client.subscribe(sub5);
      //      client.subscribe(sub6);
      client.subscribe(sub7);
      client.subscribe(sub8);
      client.subscribe(sub9);
      client.subscribe(sub10);
      client.subscribe(sub11);
      client.subscribe(sub12);
      client.subscribe(sub13);
      client.subscribe(sub14);
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
  pinMode(B3, INPUT_PULLUP);
  pinMode(B4, INPUT_PULLUP);
  pinMode(light, INPUT);
  //  pinMode(fan, OUTPUT);
  pinMode(light1, OUTPUT);
  pinMode(fan1, OUTPUT);
  pinMode(light2, OUTPUT);
  pinMode(fan2, OUTPUT);
  pinMode(PIR, INPUT);
  pinMode(GAS, INPUT);
  pinMode(BUZZER, OUTPUT);
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

  nuttrai();
  nutphai();

  nhietdo();
  doam();
  den();
  quat();

  // Menu1
  if (button == 0) {
    display.clearDisplay();
    display.setTextSize(2);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.setCursor(0, 0);            // Start at top-left corner
    display.println(F("LivingRoom"));

    innhietdo();
    indoam();
    inden();
    inquat();

    display.setTextSize(1);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.setCursor(0, 56);            // Start at top-left corner
    display.print(F("Setup value: "));
    display.print(d);
    display.println(F("*C"));
  }
  Serial.print("Temperature1: ");
  Serial.print(temperature);

  Serial.print("  Humidity1: ");
  Serial.println(humidity);

  Serial.print("Light: ");
  Serial.print(button1);

  Serial.print("  Fan: ");
  Serial.print(button2);

  Serial.print("  Mode: ");
  if (c == 0) {
    Serial.print("Manual");
  }
  if (c == 1) {
    Serial.print("Automatic");
  }
  Serial.print("  Value: ");
  Serial.println(d);

  PIRsensor();
  // Menu2
  if (button == 1) {
    display.clearDisplay();
    display.setTextSize(2);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.setCursor(0, 0);            // Start at top-left corner
    display.println(F(" Bath Room"));
    inPIRsensor();
  }

  Serial.print("PIR value: ");
  Serial.print(digitalRead(PIR));
  Serial.print("  PIR status: ");
  Serial.println(a);

  GASsensor();
  // Menu3
  if (button == 2) {
    display.clearDisplay();
    display.setTextSize(2);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.setCursor(0, 0);            // Start at top-left corner
    display.println(F("  K.Room"));
    inGASsensor();
  }

  Serial.print("GAS: ");
  Serial.print(gas);
  Serial.print("  buzzer: ");
  Serial.println(b);

// Làm không kịp để phản biện
//  cambienanhsang();
//  Serial.print("Lightsensor: ");
//  Serial.println(analogRead(light));
  
  Serial.println(" ");
  delay(100);
  display.display();
  delay(800);
}

void nhietdo() {
  temperature = String(dht.readTemperature() - 1);
  client.publish("Temperature1", temperature.c_str()); // publish temp topic
}

void innhietdo() {
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0, 16);            // Start at top-left corner
  display.print(F("Temperature: "));
  display.print(dht.readTemperature() - 1);
  display.println(F("*C"));
}

void doam() {
  humidity = String(dht.readHumidity());
  client.publish("Humidity1", humidity.c_str());   // publish hum topic
}

void indoam() {
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0, 25);            // Start at top-left corner
  display.print(F("Humidity   : "));
  display.print(dht.readHumidity());
  display.println(F(" %"));
}

void den() {
  if (digitalRead(B1) == 0) {
    button1++;
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
}

void inden() {
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0, 35);            // Start at top-left corner
  display.print(F("Light      : "));

  if (button1 == LOW) {
    display.print(F("OFF"));
  }
  if (button1 == HIGH)  {
    display.print(F("ON"));
  }
}
void quat() {
  // Chê độ thủ công
  if (c == 0) {
    if (digitalRead(B2) == 0) {
      button2++;
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
  }

  // Chế độ tự động
  if (c == 1) {
    if (dem == 0) {
      if (dht.readTemperature() - 1 >= d) {
        client.publish(pub2, "1");
        button2 == HIGH;
        dem++;
      }
      if (dht.readTemperature() - 1 < d) {
        client.publish(pub2, "0");
        button2 == LOW;
        dem++;
      }
    }
    else {
      dem++;
      if (dem > 3) {
        dem = 0;
      }
    }
  }
}

void inquat() {
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0, 45);            // Start at top-left corner
  display.print(F("Fan        : "));

  if (button2 == LOW) {
    display.print(F("OFF"));
  }
  if (button2 == HIGH) {
    display.print(F("ON"));
  }
}

void PIRsensor() {
  if (a == 1) {
    pir = String(digitalRead(PIR));
    client.publish("Pir", pir.c_str());
    if (digitalRead(PIR) == 1)
    {
      digitalWrite(fan2, HIGH);
    } else {
      digitalWrite(fan2, LOW);
    }
  }
}
void inPIRsensor() {
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0, 16);            // Start at top-left corner
  display.print(F("PIR status: "));
  if (a == 0) {
    display.print(F("OFF"));
  }

  if (a == 1) {
    display.print(F("ON"));
    if (digitalRead(PIR) == 1) {
      digitalWrite(fan2, HIGH);
    }
    if (digitalRead(PIR) == 0) {
      digitalWrite(fan2, LOW);
    }
  }

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0, 25);            // Start at top-left corner
  display.print(F("PIR value : "));
  if (a == 0) {
    display.print(F("0"));
  }
  if (a == 1) {
    display.print(digitalRead(PIR));
  }

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0, 35);            // Start at top-left corner
  display.print(F("Fan       : "));
  if (digitalRead(fan2) == 1) {
    display.print(F("ON"));
  }
  if (digitalRead(fan2) == 0) {
    display.print(F("OFF"));
  }
}

void nuttrai() {
  if (digitalRead(B3) == 0) {
    button--;
    if (button < 0) {
      button = 2;
    }
  }
}

void nutphai() {
  if (digitalRead(B4) == 0) {
    button++;
    if (button > 2) {
      button = 0;
    }
  }
}

void GASsensor() {
  value = analogRead(GAS);
  value = map(value, 0, 4095, 0, 100);
  gas = String(value);
  client.publish("Gas", gas.c_str());
  if (b == 1) {
    if (value >= 30) {
      digitalWrite(BUZZER, HIGH);
      delay(2000);
      digitalWrite(BUZZER, LOW);
    }
    else {
      digitalWrite(BUZZER, LOW);
    }
  }
}

void inGASsensor() {
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0, 16);            // Start at top-left corner
  display.print(F("Gas   : "));
  display.print(value);

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0, 25);            // Start at top-left corner
  display.print(F("Buzzer: "));
  if (b == LOW) {
    display.print(F("OFF"));
  }
  if (b == HIGH) {
    display.print(F("ON"));
  }
}

void cambienanhsang() {
  value1 = analogRead(light);
  value1 = map(value1, 0, 4095, 0, 100);
  Lightsensor = String(value1);
  client.publish("Lightsensor", Lightsensor.c_str());
//  if (value1 == 1) {
//    digitalWrite(light2, HIGH);
//  }
//  else {
//    digitalWrite(light2, LOW);
//  }
}
