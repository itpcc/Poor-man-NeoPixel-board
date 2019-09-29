#include <ESP8266WiFi.h>
#include <ESPAsyncUDP.h>
#include <FastLED.h>

const char * ssid = "<YOUR WIFISSID>";
const char * password = "<YOUR WIFI PASSWORD>";

// How many leds in your strip?
#define NUM_LEDS 96 // 12*8

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.
#define DATA_PIN 4
#define BRIGHTNESS 100

AsyncUDP udp;
CRGBArray<NUM_LEDS> leds;

typedef enum JINX_TPM2_PACKET_TYPE_ENUM_T { 
  TPM2_UNKNOW, 
  TPM2_DATA, 
  TPM2_CMD, 
  TPM2_RESPONSE 
} JINX_TPM2_PACKET_TYPE_T;

typedef struct JINX_TPM2_STRUCT_T {
  JINX_TPM2_PACKET_TYPE_T type;
  uint16_t size;
  uint8_t packet_no;
  uint8_t packet_cnt;
  uint8_t *data;
  uint16_t data_size;
} JINX_TPM2_T;


/**
 * Parse raw UDP packet stream to JINX_TPM2_T struct
 * @return parsed JINX_TPM2_T Jinx's TPM2 data
 * @see https://gist.github.com/jblang/89e24e2655be6c463c56#tpm2net-packet-structure
 */
JINX_TPM2_T tpm2_parse(uint8 *packet, size_t packetLength){
  JINX_TPM2_T result = { .type = TPM2_UNKNOW, .size = 0, .packet_no = 0, .packet_cnt = 0, .data = NULL, .data_size = 0 };
  if(packet[0] != 0x9C ) return result;
  
  result.size = ((uint16_t) packet[2]) << 8 | ((uint16_t) packet[3]);
  if(packet[6 + result.size] != 0x36) return result; // Packet end byte must end with 0x36
  result.data_size = result.size - 6;
  
  switch(packet[1]){
    case 0xDA: result.type = TPM2_DATA; break;
    case 0xC0: result.type = TPM2_CMD; break;
    case 0xAA: result.type = TPM2_RESPONSE; break;
  }
  if(result.type == TPM2_UNKNOW) return result;
  
  result.packet_no = packet[4];   // packet number 0-255 0x00 = no frame split (0x01)
  result.packet_cnt = packet[5];   // total packets 1-255 (0x01)
  result.data = &packet[6];

  return result;
}

/**
 * Clear displaying LED to stop display.
 */
void led_clear(){
  leds = 0x000000;
  FastLED.show();
}

/**
 * Display LED matrix based on TPM2 packet received.
 * It'll check packet number and start from segment of data defined in `packet_no`.
 * For example, if it's packet #3 and each packet is 16 bytes, it'll start display at pixel number 16*3 = 48.
 * @param matrixPacket TPM2 struct that contain matrix's data.
 */
void led_display(JINX_TPM2_T &matrixPacket){
  for(
    size_t i = ((matrixPacket.packet_no - 1) * matrixPacket.data_size); 
    i < matrixPacket.data_size && i < (NUM_LEDS * 3); 
    i += 3
  ){
    leds[i/3] = CRGB(matrixPacket.data[i], matrixPacket.data[i + 1], matrixPacket.data[i + 2]);
  }
  FastLED.show();
}

void setup()
{
    Serial.begin(115200);
    WiFi.disconnect(true);
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
    led_clear();
    FastLED.setBrightness(BRIGHTNESS);
    delay(1000);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("\n\n ------- Jinx tester -----------");
    
    uint8_t i = 0;
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      if(++i % NUM_LEDS == 0){ led_clear(); i = 0; }
      leds[i - 1] = 0x0F0F00; FastLED.show();
      Serial.print(".");
    }
    Serial.println("");
    led_clear();
    
    if(udp.listen(65506)) { // Default (and only?) port for TPM2.net
        Serial.print("UDP Listening on IP: ");
        Serial.println(WiFi.localIP());
        udp.onPacket([](AsyncUDPPacket packet) {
            /* Serial.print("UDP Packet Type: ");
            Serial.print(packet.isBroadcast()?"Broadcast":packet.isMulticast()?"Multicast":"Unicast");
            Serial.print(", From: ");
            Serial.print(packet.remoteIP());
            Serial.print(":");
            Serial.print(packet.remotePort());
            Serial.print(", To: ");
            Serial.print(packet.localIP());
            Serial.print(":");
            Serial.print(packet.localPort());
            Serial.print(", Length: ");
            Serial.print(packet.length());
            Serial.print(", Data: ");
            Serial.write(packet.data(), packet.length());
            Serial.println();
            //reply to the client
            packet.printf("Got %u bytes of data", packet.length()); */
            uint8_t *packetBuffer = packet.data();
            size_t packetLength = packet.length();
            JINX_TPM2_T matrixPacket = tpm2_parse(packetBuffer, packetLength);
            if(matrixPacket.type == TPM2_DATA && matrixPacket.data_size > 0)
              led_display(matrixPacket);
        });
    }
}

void loop()
{
    delay(1000);
    //Send broadcast
    // udp.broadcast("Anyone here?");
}
