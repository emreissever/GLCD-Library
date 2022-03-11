#include <16f877.h>

#fuses HS,NOWDT,NOPROTECT,NOBROWNOUT,NOLVP,NOPUT,NOWRT,NODEBUG,NOCPD

#use delay (clock=8000000)

#include <GLCD.h> // Include the Library.
#include <graphics.c>

char text1[]="Hazirlayan";
char text2[]="Emre";
char text3[]="ISSEVER";
int i,x1,y1,x2,y2;

void main ( )
{
   setup_psp(PSP_DISABLED);
   setup_timer_1(T1_DISABLED);     
   setup_timer_2(T2_DISABLED,0,1); 
   setup_adc_ports(NO_ANALOGS);    
   setup_adc(ADC_OFF);             
   setup_CCP1(CCP_OFF);            
   setup_CCP2(CCP_OFF);            

   glcd_init(ON); // Initialize Graphic LCD and clear the screen.

   while(1)
   {
      glcd_init(ON); 
      glcd_text57(10, 5, text1, 2, ON);  // Writing some texts. 
      glcd_text57(39, 25, text2, 2, ON); 
      glcd_text57(25,40, text3, 2, ON);
      delay_ms(5000);

      glcd_init(ON); // Clear the screen.
      x1=5;
      y1=5;
      x2=100;
      y2=5;

      for(i=0;i<6;i++)
      {
         glcd_line(x1, y1, x2, y2, ON); // Drawing a line.
         y2=y1+=10; // y2 = y1 = y2 + 10
         x2-=10;    // x2 = x2 - 10
         delay_ms(1000);
      }

      glcd_init(ON); // Clear the screen.
      x1=y1=10;

      for (i=0;i<120;i++)
      {
         glcd_pixel(x1,y1,ON); // Desired pixels are activated.
         y1=x1++;   // y1 = x1 + 1
         delay_ms(50);
      }

      glcd_init(ON); // Clear the screen.
      y2=60;
      x1=5;

      for (i=0;i<8;i++)
      {
         glcd_bar(x1, 0, x1, y2, 10, ON); // Drawing a bar.
         delay_ms(1000);
         x1+=15; // x1=x1+15
         y2-=5;  // y2=y2-5
      }

      glcd_init(ON); // Clear the screen.
      
      for(i=0;i<=30;i=i+5)
      {
         glcd_circle(60, 30, i, NO, ON); // Drawing a circle.
         delay_ms(1000);
      }

      glcd_init(ON); // Clear the screen.
      x1=5;
      y1=5;
      x2=120;
      y2=63;

      for(i=0;i<6;i++)
      {
         glcd_rect(x1, y1, x2, y2, NO, ON); // Drawing a rectangle.
         y1=x1+=5; // y1=x1+5 
         x2-=5;    // x2=x2-5
         y2-=5;    // y2=y2-5
         delay_ms(500);
      }
   }
}
