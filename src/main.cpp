#include <Arduino.h>
#include <NeoPixelBus.h>
#include <ESblinkt.hpp>

#define SATURATION 128
#define PIXEL_COUNT 179
#define PIXEL_PIN 23

NeoPixelBus<NeoGrbFeature, NeoWs2812xMethod> strip(PIXEL_COUNT, PIXEL_PIN);
struct strip leds;

RgbColor red(SATURATION, 0, 0);
RgbColor green(0, SATURATION, 0);
RgbColor blue(0, 0, SATURATION);
RgbColor white(SATURATION);
RgbColor black(0);

enum MODE
{
  RAINBOW,
  SOLID,
  THREE_SIDED
};

enum MODE mode = THREE_SIDED;

uint32_t rainbow_speed = 1;
struct three_sided three_sided_config;
RgbColor solid_color = red;

void setPixelColor(uint16_t pixel, Neo3ByteElements::ColorObject color)
{
  strip.SetPixelColor(pixel, color);
}

void commit(void)
{
  strip.Show();
}

void setup()
{
  leds.pixelCount = PIXEL_COUNT;
  leds.setPixelColor = &setPixelColor;
  leds.commit = &commit;

  three_sided_config.endRight = 53;
  three_sided_config.endTop = 125;
  three_sided_config.colors[0] = red;
  three_sided_config.colors[1] = green;
  three_sided_config.colors[2] = blue;
  three_sided_config.anim = true;
  three_sided_config.speed = 500;

  Serial.begin(115200);

  // this resets all the neopixels to an off state
  strip.Begin();
  strip.Show();
}

void loop()
{
  mode = (enum MODE)random(3);
  Serial.printf("Mode %d\n", mode);

  switch (mode)
  {
  case RAINBOW:
    rainbow(rainbow_speed, leds);
    break;
  case SOLID:
    solid(solid_color, leds);
    break;
  case THREE_SIDED:
    three_sided(three_sided_config, leds);
    break;
  default:
    Serial.printf("Invalid mode! Mode: %d\n", mode);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
