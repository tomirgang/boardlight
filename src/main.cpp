#include <Arduino.h>
#include <NeoPixelBus.h>

const uint16_t PixelCount = 179;
const uint8_t PixelPin = 7;

const uint16_t EndRight = 53;
const uint16_t EndTop = 125;

#define colorSaturation 128

    NeoPixelBus<NeoGrbFeature, NeoWs2812xMethod>
        strip(PixelCount, PixelPin);

RgbColor red(colorSaturation, 0, 0);
RgbColor green(0, colorSaturation, 0);
RgbColor blue(0, 0, colorSaturation);
RgbColor white(colorSaturation);
RgbColor black(0);

enum MODE
{
  RAINBOW,
  SOLID,
  THREE_SIDED
};

enum MODE mode = THREE_SIDED;

uint32_t rainbow_speed = 1;
RgbColor solid_color = red;
RgbColor three_sided_colors[3] = {red, green, blue};
bool three_sided_anim = true;
uint32_t three_sided_speed = 500;

RgbColor color_wheel(uint8_t pos);
void fill_rainbow(uint8_t offset);
void fill_solid(RgbColor color);
void rainbow();
void solid();
void three_sided();


void setup()
{
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
    rainbow();
    break;
  case SOLID:
    solid();
    break;
  case THREE_SIDED:
    three_sided();
    break;
  default:
    Serial.printf("Invalid mode! Mode: %d\n", mode);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

RgbColor color_wheel(uint8_t pos)
{
  uint16_t p = (uint8_t)pos;

  if (p < 85)
  {
    return RgbColor((uint8_t)(255 - p * 3), (uint8_t)(p * 3), (uint8_t)0);
  }
  else if (p < 170)
  {
    p -= 85;
    return RgbColor((uint8_t)0, (uint8_t)(255 - p * 3), (uint8_t)(p * 3));
  }
  else
  {
    p -= 170;
    return RgbColor((uint8_t)(p * 3), (uint8_t)0, (uint8_t)(255 - p * 3));
  }
}

void fill_rainbow(uint8_t offset) {
  uint16_t color_index = 0;
  for (uint16_t i = 0; i < PixelCount; i++)
  {
    color_index = (i + (uint16_t)offset) % 256;
    strip.SetPixelColor(i, color_wheel(color_index));
  }
}

void fill_solid(RgbColor color) {
  for (uint16_t i = 0; i<PixelCount; i++) {
    strip.SetPixelColor(i, color);
  }
}

void rainbow()
{
  for (int i = 0; i < 256; i++)
  {
    fill_rainbow(i);
    strip.Show();
    vTaskDelay(rainbow_speed / portTICK_PERIOD_MS);
  }
}

void solid()
{
  fill_solid(solid_color);
  strip.Show();
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}

void three_sided()
{
  uint32_t delay = 1000;
  uint8_t n = 1;
  
  if (three_sided_anim) {
    delay = three_sided_speed;
    n = 3;
  }

  for (int off = 0; off < n; off++)
  {

    for (int j = 0; j < EndRight; j++)
    {
      strip.SetPixelColor(j, three_sided_colors[(off) % 3]);
    }
    for (int j = EndRight; j < EndTop; j++)
    {
      strip.SetPixelColor(j, three_sided_colors[(off + 1) % 3]);
    }
    for (int j = EndTop; j < PixelCount; j++)
    {
      strip.SetPixelColor(j, three_sided_colors[(off + 2) % 3]);
    }
    strip.Show();
    vTaskDelay(delay / portTICK_PERIOD_MS); 
  }
}
