
#include "Adafruit_GFX.h"
#include "Adafruit_NeoMatrix.h"
#include "Adafruit_NeoPixel.h"

#define PIN 6
#define PIN_POT A0
//Инициализация пинов для регистра
#define DATA_PIN  9
#define LATCH_PIN 11
#define CLOCK_PIN 10

// Define matrix width and height.
#define mw 8
#define mh 8

int color(int);
void del(int);
void set(int);
//void full(int);

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix( mw, mh, PIN,
  NEO_MATRIX_TOP  + NEO_MATRIX_LEFT +
  NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
  NEO_GRB         + NEO_KHZ800);

const uint16_t colors[] = {
  matrix.Color(255, 0, 0), 
  matrix.Color(0, 255, 0), 
  matrix.Color(0, 0, 255) };

//Работа с регистром 165
void setup()
{
	Serial.begin(57600);
  pinMode(PIN_POT, INPUT);
  matrix.begin();
  matrix.setTextWrap( false );
  
  matrix.setTextColor( colors[0] );
  matrix.show();
  pinMode(LATCH_PIN, OUTPUT); 
  pinMode(CLOCK_PIN, OUTPUT); 
  pinMode(DATA_PIN, INPUT); 
  digitalWrite(CLOCK_PIN,LOW); 
  digitalWrite(LATCH_PIN,LOW);
}

void loop() 
{
  int rot, y, x, n, r, col, j, max;
  //byte in_165_shift = shiftIn(DATA_PIN, CLOCK_PIN, MSBFIRST);
  digitalWrite( LATCH_PIN, HIGH ); 
  byte in_165_shift = shiftInFixed( DATA_PIN, CLOCK_PIN ); 
  digitalWrite(LATCH_PIN,LOW);
  rot = analogRead(PIN_POT);
  matrix.setBrightness( 255 );
  
  set(in_165_shift);
  //full(in_165_shift);
  del(~in_165_shift);

  matrix.show();
}

void set(int in_165_shift)
{
  int r, x, y, n;

  n = in_165_shift / 4;
  r = n / 8;
  x = n;// - 8 * r;
  y = n;// / 8;

  matrix.drawPixel(x, y, color(n));
}

void del(int in_165_shift)
{
  int x, r, y, n;

  n = in_165_shift / 4 + 1;
  r = n / 8;
  y = n;// / 8;
  x = n;// - 8 * r;
  
  matrix.drawPixel(x, y, 0);
}

/*void full(byte in_165_shift)
{
  int r, x, y, n, max;

  n = rot / 16;

  for(max = 0; max < n; max++)
  {
    r = max / 8;
    x = max - 8 * r;
    y = max / 8;
    matrix.drawPixel(x, y, color(max));
  }

}*/

int color(int n)
{
  if (n >= 0 && n <= 20)
  {
    return colors[1];
  }

  if (n >= 21 && n <= 45)
  {
    return colors[1] + colors[0];
  }
  if (n >= 46 && n <= 64)
  {
    return colors[0];
  }

}

byte shiftInFixed( byte dataPin, byte clockPin ) 
{ 
    byte value = 0; 
    int j= 7; 
    for( byte i=0; i<8; ++i ) 
    { 
        value |= digitalRead( dataPin ) << j;
        j--;
        digitalWrite( clockPin, HIGH ); 
        digitalWrite( clockPin, LOW ); 
    } 
    return value; 
} 