/*
   ---- Graphic LCD Library for EasyPICv7 With CCS C ----

   -Not for commercial purpose ! This file is Updated version of standart CCS Library (HDM64GS12.c) for EasyPICv7 development board.

   -This file is the required library to run Winstar WDG0151-TMI and Xiamen PGM12864B display with KS0108 display controller (or equivalent).

   -This library contains drivers for Graphic Displays has KS0108 display controller or equivalent. Like Winstar WDG0151-TMI, Xiamen PGM12864B, Hantronix HDM64GS12...etc

   -Its 128 by 64 pixels and the driver treats the upper left pixel as origin (0,0).

   -Pins are defined as on the Development Board (EasyPICv7)
   https://download.mikroe.com/documents/full-featured-boards/easy/easypic-v7/easypic-v7-schematic-v104c.pdf

   -Can be used with standart library of "graphics.c"

   * CS1          = B0
   * CS2          = B1
   * Vss          = GND
   * Vdd          = 5V
   * Vo           = Middle pin of potentiometer (Constrast adjustment) 
   * DI veya RS   = B2
   * R/W          = B3
   * Enable       = B4
   * Rst          = B5
   * Data pins    = D0 to D7
   * Vee          = -5V is also connected to potentiometer.
   * A            = 5V
   * K            = GND
   ____________________________________________________________________________

   ! Use #define FAST_GLCD if the target chip has at least 1k of RAM to decrease the time it takes to update the display. 

   glcd_init(ON/OFF);
         *To turn the LCD ON or OFF
         *Must be called before any other function! 

   glcd_pixel(x,y, ON/OFF);
         *Sets on or off the desired pixel

   glcd_update();
          *Write the display data stored in RAM to the LCD
          *Only available if FAST_GLCD is defined  

   glcd_fillScreen(ON/OFF);
          *To turn On or Off all pixels of the screen.


   If output of GLCD looks like cloned on the both halves of the screen. Change Chip Select (Line 83 and 87) definitions as below


   It must be like this ;

   #ifndef GLCD_CS1
   #define GLCD_CS1     PIN_B0
   #endif
   ...
   #ifndef GLCD_CS2
   #define GLCD_CS2     PIN_B1
   #endif

   Or ;

   #ifndef GLCD_CS1
   #define GLCD_CS1     PIN_B1
   #endif
   ...
   #ifndef GLCD_CS2
   #define GLCD_CS2     PIN_B0
   #endif

   -Prepared by Emre İŞSEVER 
*/

#ifndef HDM64GS12
#define HDM64GS12

#ifndef GLCD_WIDTH
#define GLCD_WIDTH   128
#endif

#ifndef GLCD_CS1
#define GLCD_CS1     PIN_B1   // Definition of Chip Select 1
#endif

#ifndef GLCD_CS2
#define GLCD_CS2     PIN_B0   // Definition of Chip Select 2
#endif

#ifndef GLCD_DI
#define GLCD_DI      PIN_B2   // Definition of D/I Pin (Represented as RS in the Development Board)
#endif

#ifndef GLCD_RW
#define GLCD_RW      PIN_B3  
#endif

#ifndef GLCD_E
#define GLCD_E       PIN_B4   
#endif

#ifndef GLCD_RST
#define GLCD_RST     PIN_B5  
#endif

#define GLCD_LEFT    0
#define GLCD_RIGHT   1

#ifndef ON
#define ON           1
#endif

#ifndef OFF
#define OFF          0
#endif

/////////////////////////////////////////////////////////////////////////

void glcd_init(int1 mode);
void glcd_pixel(unsigned int8 x, unsigned int8 y, int1 color);
void glcd_fillScreen(int1 color);
void glcd_writeByte(int1 side, BYTE data);
BYTE glcd_readByte(int1 side);
void glcd_update();

#ifdef FAST_GLCD
struct {
   unsigned int8 left[512];
   unsigned int8 right[512];
}
displayData;
#endif

/////////////////////////////////////////////////////////////////////////

void glcd_init(int1 mode) {

   //Required values are assigned to pins
   output_high(GLCD_RST);
   output_low(GLCD_E);
   output_low(GLCD_CS1);
   output_low(GLCD_CS2);

   output_low(GLCD_DI);
   glcd_writeByte(GLCD_LEFT,  0xC0);
   glcd_writeByte(GLCD_RIGHT, 0xC0);
   glcd_writeByte(GLCD_LEFT,  0x40);
   glcd_writeByte(GLCD_RIGHT, 0x40);
   glcd_writeByte(GLCD_LEFT,  0xB8);
   glcd_writeByte(GLCD_RIGHT, 0xB8);

   if(mode == ON)
   {
      glcd_writeByte(GLCD_LEFT,  0x3F);
      glcd_writeByte(GLCD_RIGHT, 0x3F);
   }
   else
   {
      glcd_writeByte(GLCD_LEFT,  0x3E);
      glcd_writeByte(GLCD_RIGHT, 0x3E);
   }

   glcd_fillScreen(OFF);

   #ifdef FAST_GLCD
   glcd_update();
   #endif
}

#ifdef FAST_GLCD

/////////////////////////////////////////////////////////////////////////

void glcd_update() {

   unsigned int8 i, j;
   unsigned int8 *p1, *p2;

   p1 = displayData.left;
   p2 = displayData.right;

   for(i = 0; i < 8; ++i)
   {
      output_low(GLCD_DI);
      glcd_writeByte(GLCD_LEFT, 0x40);
      glcd_writeByte(GLCD_RIGHT, 0x40);
      glcd_writeByte(GLCD_LEFT, i | 0xB8);
      glcd_writeByte(GLCD_RIGHT, i | 0xB8);
      output_high(GLCD_DI);

      for(j = 0; j < 64; ++j)
      {
         glcd_writeByte(GLCD_LEFT, *p1++);
         glcd_writeByte(GLCD_RIGHT, *p2++);
      }
   }
}
#endif

/////////////////////////////////////////////////////////////////////////

void glcd_pixel(unsigned int8 x, unsigned int8 y, int1 color)
#ifdef FAST_GLCD
{
   unsigned int8* p;
   unsigned int16 temp;
   temp =  y/8;
   temp *= 64;
   temp += x;

   if(x > 63)
   {
      p = displayData.right + temp - 64;
   }
   else
   {
      p = displayData.left + temp;
   }

   if(color)
   {
      bit_set(*p, y%8);
   }
   else
   {
      bit_clear(*p, y%8);
   }
}

#else
{
   BYTE data;
   int1 side = GLCD_LEFT;

   if(x > 63)
   {
      x -= 64;
      side = GLCD_RIGHT;
   }

   output_low(GLCD_DI);
   bit_clear(x,7);
   bit_set(x,6);
   glcd_writeByte(side, x);
   glcd_writeByte(side, (y/8 & 0xBF) | 0xB8);
   output_high(GLCD_DI);
   glcd_readByte(side);
   data = glcd_readByte(side);

   if(color == ON)
      bit_set(data, y%8);
   else
      bit_clear(data, y%8);

   output_low(GLCD_DI);
   glcd_writeByte(side, x);
   output_high(GLCD_DI);
   glcd_writeByte(side, data);
}
#endif

/////////////////////////////////////////////////////////////////////////

void glcd_fillScreen(int1 color)
#ifdef FAST_GLCD
{
   unsigned int8  data;
   unsigned int8  *p1, *p2;
   unsigned int16 i;

   p1 = displayData.left;
   p2 = displayData.right;
   data = 0xFF * color;

   for(i=0; i<512; ++i)
   {
      *p1++ = data;
      *p2++ = data;
   }
}
#else
{
   unsigned int8 i, j;

   for(i = 0; i < 8; ++i)
   {
      output_low(GLCD_DI);
      glcd_writeByte(GLCD_LEFT, 0b01000000);
      glcd_writeByte(GLCD_RIGHT, 0b01000000);
      glcd_writeByte(GLCD_LEFT, i | 0b10111000);
      glcd_writeByte(GLCD_RIGHT, i | 0b10111000);
      output_high(GLCD_DI);

      for(j = 0; j < 64; ++j)
      {
         glcd_writeByte(GLCD_LEFT, 0xFF*color);
         glcd_writeByte(GLCD_RIGHT, 0xFF*color);
      }
   }
}
#endif

/////////////////////////////////////////////////////////////////////////

void glcd_writeByte(int1 side, BYTE data)
{
   set_tris_d(0x00);

   output_low(GLCD_RW);

    if(side)
      output_high(GLCD_CS2);
   else
      output_high(GLCD_CS1);

    delay_us(1);

   output_d(data);
   delay_us(1);
   output_high(GLCD_E);
   delay_us(1);
   output_low(GLCD_E);

   output_low(GLCD_CS1);
   output_low(GLCD_CS2);
}

/////////////////////////////////////////////////////////////////////////

BYTE glcd_readByte(int1 side)
{
   BYTE data;

   set_tris_d(0xFF);
   output_high(GLCD_RW);

   if(side)
      output_high(GLCD_CS2);
   else
      output_high(GLCD_CS1);

   delay_us(1);
   output_high(GLCD_E);
   delay_us(1);
   data = input_d();
   output_low(GLCD_E);

   output_low(GLCD_CS1);
   output_low(GLCD_CS2);
   return data;
}

#endif
