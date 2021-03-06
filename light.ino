#include <LoRaNow.h>

#define AD5 2  //定义模拟口A5 

//vspi for lora radio module
#define MISO 19
#define MOSI 23
#define SCK 18
#define SS 5

#define DIO0 4

//for 18b20
#include <OneWire.h>
#include <DallasTemperature.h>

int Intensity = 0;


// GPIO where the DS18B20 is connected to
const int oneWireBus= 25 ; 

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);
double tmp;

void setup() {
  pinMode(AD5,INPUT);
  Serial.begin(115200);
  Serial.println("LoRaNow便携节点");
  pinMode(oneWireBus,INPUT);
  sensors.begin();
   LoRaNow.setFrequencyCN(); // Select the frequency 486.5 MHz - Used in China
  // LoRaNow.setFrequencyEU(); // Select the frequency 868.3 MHz - Used in Europe
  // LoRaNow.setFrequencyUS(); // Select the frequency 904.1 MHz - Used in USA, Canada and South America
  // LoRaNow.setFrequencyAU(); // Select the frequency 917.0 MHz - Used in Australia, Brazil and Chile

  // LoRaNow.setFrequency(frequency);
  // LoRaNow.setSpreadingFactor(sf);
  // LoRaNow.setPins(ss, dio0);

   LoRaNow.setPinsSPI(SCK, MISO, MOSI, SS, DIO0); // Only works with ESP32

  if (!LoRaNow.begin()) {
    Serial.println("LoRa加载失败。检查你的连接。");
    while (true);
  }

  LoRaNow.onMessage(onMessage);
  LoRaNow.onSleep(onSleep);
  LoRaNow.showStatus(Serial);
}

void loop() {
  sensors.requestTemperatures(); 
  //tmp = sensors.getTempCByIndex(0);
  Intensity =  10240000/(1.1*analogRead(AD5))-1000;
  
  LoRaNow.loop();
}


void onMessage(uint8_t *buffer, size_t size)
{
  Serial.print("收到信息: ");
  Serial.write(buffer, size);
  Serial.println();
  Serial.println();
}

void onSleep()
{
  Serial.println("休息中...");
  Serial.print("现在的光照强度是：");
  Serial.print(Intensity);
  Serial.println("Lux");
  Serial.println("发送信息：");
  LoRaNow.print("信息编号");
  LoRaNow.print(millis());
  LoRaNow.print("：");
  LoRaNow.print("现在的光照强度是：");
  //Serial.println("LoRaNow Message sended");
  LoRaNow.print(Intensity);
  LoRaNow.print("Lux");
  LoRaNow.send();
}
