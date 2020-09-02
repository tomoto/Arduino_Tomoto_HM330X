#include "Platform.h"
#ifdef M5ATOM

#include <FastLED.h>

#include "Display.h"

const int NUM_LEDS = 25;
const int LED_PIN = 27;
static CRGB leds[NUM_LEDS];

static const CRGB signalColors[] = {
    CRGB::Green,       // Good
    CRGB::Yellow,      // Moderate
    CRGB::DarkOrange,  // Unhealthy for sensitive groups
    CRGB::Red,         // Unhealthy
    CRGB::Purple,      // Very Unhealthy
};

inline CRGB correct(CRGB c) { return CRGB(c.g, c.r, c.b); }

void initDisplay() {
  FastLED.addLeds<WS2812, LED_PIN>(leds, NUM_LEDS);
  setDisplayBrightness(0);
  FastLED.clear(true);
}

void setDisplayBrightness(int brightness) {
  static uint8_t brighnesses[] = {8, 20};
  FastLED.setBrightness(brighnesses[brightness]);
}

static void displayPanic(CRGB color) {
  FastLED.clear();

  // Draw a cross
  for (int i = 0; i < 5; i++) {
    leds[i * 6] = leds[i * 4 + 4] = correct(color);
  }

  FastLED.show();
}

struct BlinkSpec {
  float ratio;
  int position;
  CRGB color;
};

static const BlinkSpec NO_BLINK = {0, -1, CRGB::Black};
static BlinkSpec blinkPixel = NO_BLINK;

void displayStatus(float aqi, int pm2_5, int pm10, State sensorState) {
  if (sensorState == State::BAD) {
    blinkPixel = NO_BLINK;
    displayPanic(CRGB::OrangeRed);  // Draw orange cross
    return;
  }

  FastLED.clear();

  // Render the level meter
  int value = int(aqi);
  int count = min(value / 10, NUM_LEDS - 1);
  CRGB c = signalColors[count / 5];
  for (int i = 0; i < count; i++) {
    leds[i] = correct(c);
  }

  // Let the last pixel blink
  if (value < NUM_LEDS * 10) {
    blinkPixel = {0.1f * (value % 10), count, correct(c)};
  } else {
    blinkPixel = {1, count, correct(c)};
  }

  FastLED.show();
}

void animateDisplay() {
  if (blinkPixel.position < 0) {
    return;
  }

  // Let the specified pixel blink in one second cycle
  leds[blinkPixel.position] = (millis() % 1000 < blinkPixel.ratio * 1000)
                                  ? blinkPixel.color
                                  : CRGB::Black;
  FastLED.show();
}

void displayInitializationError() {
  displayPanic(CRGB::Blue);  // Draw blue cross
}

#endif
