#include <Nxp/iolpc2124.h>
#include "LCD.h"


void main()
{
  init_mc();
  delay(10);
  init_lcd();
  delay(10);
  char data [] ="Hello World!";
  for (int i=0;i<12;i++)
  {
    write_data(data[i]);
    delay(10000);
  }
  
}


