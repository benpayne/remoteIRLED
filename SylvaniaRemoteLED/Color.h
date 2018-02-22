#include "Arduino.h"

class Color {
  public:
    Color() : r( 0 ), g( 0 ), b( 0 ) {}
    
    void set( uint16_t _r, uint16_t _g, uint16_t _b )
    {
      r = _r;
      g = _g;
      b = _b;
    }

    void set( const Color &c )
    {
      r = c.r;
      g = c.g;
      b = c.b;
    }

    void set( uint16_t *buf )
    {
      r = buf[ 0 ];
      g = buf[ 1 ];
      b = buf[ 2 ];
    }

    void setHSV(float h, float s, float v)
    {
      float i,f,p,q,t;
  
      h = max(0.0, min(360.0, h));
      s = max(0.0, min(1.0, s));
      v = max(0.0, min(1.0, v));
    
      if(s == 0) {
        // Achromatic (grey)
        r = g = b = round(v*4095);
        return;
      }
 
      h /= 60; // sector 0 to 5
      i = floor(h);
      f = h - i; // factorial part of h
      p = v * (1 - s);
      q = v * (1 - s * f);
      t = v * (1 - s * (1 - f));
      switch((int)i) {
        case 0:
          r = round(4095*v);
          g = round(4095*t);
          b = round(4095*p);
          break;
        case 1:
          r = round(4095*q);
          g = round(4095*v);
          b = round(4095*p);
          break;
        case 2:
          r = round(4095*p);
          g = round(4095*v);
          b = round(4095*t);
      break;
        case 3:
          r = round(4095*p);
          g = round(4095*q);
          b = round(4095*v);
          break;
        case 4:
          r = round(4095*t);
          g = round(4095*p);
          b = round(4095*v);
          break;
        default: // case 5:
          r = round(4095*v);
          g = round(4095*p);
          b = round(4095*q);
        }
    }

    void multiply( uint16_t value )
    {
      float fv = value / 4095.0;
      r = r * fv;
      g = g * fv;
      b = b * fv;
    }
        
    uint16_t r;
    uint16_t g;
    uint16_t b;
};

