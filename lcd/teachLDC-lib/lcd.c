#include "LCD.h"

void init_mc()
{
  PINSEL0=0x00000000;
  IO0DIR=0X0000FFFF;
  //IO0SET=0x0000ffff;
}
void delay(unsigned int val)
{
  for(int i=1;i<=val;i++)
  {
  }
}
void write_data(char value)
{
  IO0SET_bit.P0_8=1;
  IO0CLR_bit.P0_9=1;
  IO0SET=value;
  IO0SET_bit.P0_10=1;
  delay(100);
  IO0CLR_bit.P0_10=1;
  delay(10000);
  IO0CLR=0x0000FFFF;   
}

void write_cmd(char value)
{
  IO0CLR_bit.P0_8=1;
  IO0CLR_bit.P0_9=1;
  IO0SET=value;
  IO0SET_bit.P0_10=1;
  delay(100);
  IO0CLR_bit.P0_10=1;
  delay(10000);
  IO0CLR=0x0000FFFF;   
}

void init_lcd()
{
  write_cmd(0x01);
  delay(100);
  write_cmd(0x38);
  delay(100);
  write_cmd(0x06);
  delay(100);
  write_cmd(0x0E);
  delay(100);
}