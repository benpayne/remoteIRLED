

//#include <SoftwareSerial.h>
#include <IRremote.h>

//SoftwareSerial lcdSerial(6, 5);
#include "Color.h"

Color c;
int color_angle = 0;
 boolean on = false;

enum {
 STATE_CONSTANT,
 STATE_FLASH_FAST, 
 STATE_FLASH_SLOW, 
 STATE_ROTATE,
};

int state = STATE_CONSTANT;
int state_count = 0;

int RECV_PIN = 7;
IRrecv irrecv(RECV_PIN);

decode_results results;

void setup()
{
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  c.set(255,255,255);
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
}

long codes[] = {
  0xFFF807,
  0xFFB04F,
  
  0xFF9867,
  0xFFE817,
  0xFF02FD,
  0xFF50AF,
  0xFF38C7,
  
  0xFFD827,
  0xFF48B7,
  0xFF32CD,
  0xFF7887,
  0xFF28D7,
  
  0xFF8877,
  0xFF6897,
  0xFF20DF,
  0xFF708F,
  0xFFF00F,

  0xFFA857,
  0xFFB24D,
  0xFF00FF,
  0xFF58A7,
  0xFF30CF,

  0xFF906F,
  0xFFB847,
  
  0xFFFFFFFF
};

enum IRCodes {
  POWER_OFF,
  POWER_ON,
  RED1,
  RED2,
  RED3,
  RED4,
  RED5,
  GREEN1,
  GREEN2,
  GREEN3,
  GREEN4,
  GREEN5,
  BLUE1,
  BLUE2,
  BLUE3,
  BLUE4,
  BLUE5,
  WHITE,
  FLASH,
  STROBE,
  FADE,
  SMOOTH,
  BRIGHTER,
  DIMMER,
  NO_CODE
};

// Dumps out the decode_results structure.
// Call this after IRrecv::decode()
// void * to work around compiler issue
//void dump(void *v) {
//  decode_results *results = (decode_results *)v
int dump(decode_results *results)
{    
  Serial.print("Value: ");
  Serial.println(results->value, HEX);
  
  for ( int i = 0; i < NO_CODE; i++ )
  {
    if ( codes[ i ] == results->value )
      return i;
  }
  
  return NO_CODE;
}

void setColor( Color &c )
{
  analogWrite( 10, c.r );
  analogWrite( 9, c.g );
  analogWrite( 8, c.b );
}

void setColor4096( Color &c )
{
  analogWrite( 10, c.r / 16 );
  analogWrite( 9, c.g / 16 );
  analogWrite( 8, c.b / 16 );
}

void clearColor()
{
  analogWrite( 10, 0 );
  analogWrite( 9, 0 );
  analogWrite( 8, 0 );
}


void loop() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    int code = dump(&results);
    Serial.print("Code: ");  
    Serial.println( code, DEC);
    if ( code != NO_CODE )
    {
      state = STATE_CONSTANT;
      state_count = 0;
    }
    switch( code )
    {
      case POWER_ON:
        on = true;
        break;
      case POWER_OFF:
        on = false;
        break;
      case WHITE:
        c.set(255,255,255);
        break;
      case RED1:
        c.set(255,0,0);
        break;
      case RED2:
        c.set(255,16,0);
        break;
      case RED3:
        c.set(255,64,0);
        break;
      case RED4:
        c.set(255,128,0);
        break;
      case RED5:
        c.set(255,255,0);
        break;
      case GREEN1:
        c.set(0,255,0);
        break;
      case GREEN2:
        c.set(0,128,32);
        break;
      case GREEN3:
        c.set(0,128,64);
        break;
      case GREEN4:
        c.set(0,128,128);
        break;
      case GREEN5:
        c.set(0,255,255);
        break;
      case BLUE1:
        c.set(0,0,255);
        break;
      case BLUE2:
        c.set(64,0,128);
        break;
      case BLUE3:
        c.set(128,0,128);
        break;
      case BLUE4:
        c.set(192,0,128);
        break;
      case BLUE5:
        c.set(255,0,255);
        break;
      case STROBE:
        state = STATE_FLASH_FAST;
        break;
      case FLASH:
        state = STATE_FLASH_SLOW;
        break;
      case FADE:
        state = STATE_ROTATE;
        break;
    }
        
    
    //c.setHSV( angle, 1.0, 1.0 );
    Serial.println( state, DEC );
      
    Serial.print( c.r, DEC );
    Serial.print( " " );
    Serial.print( c.g, DEC );
    Serial.print( " " );
    Serial.println( c.b, DEC );

    irrecv.resume(); // Receive the next value
  }
  
  if ( on )
  {
    switch ( state )
    {
      case STATE_FLASH_FAST:
        if ( state_count == 0 )
        {
          setColor( c );        
          state_count = 1;
        }
        else
        {
          clearColor();
          state_count = 0;
        }
        break;
      case STATE_FLASH_SLOW:
        if (state_count < 15)
          setColor(c);
        else
          clearColor();
        
        state_count += 1;
        if ( state_count >= 30 )
          state_count = 0;
        break;
      case STATE_ROTATE:
        color_angle += 1;
        if ( color_angle >= 360 )
          color_angle -= 360;
        c.setHSV( color_angle, 1.0, 1.0 );
        setColor4096( c );
        break;
      case STATE_CONSTANT:
        setColor( c );
        break;
    }
  }
  else
  {
    clearColor();
  }
  
  delay(30);
}
