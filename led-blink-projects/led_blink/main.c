#include<NXP/iolpc2124.h>
void init(void);
void on_off(void);
void delay(unsigned int);

void main()
{
  init();
  while(1)
  {
  on_off();
  }
}

void init()
{
  PINSEL0_bit.P0_0=0;
  IO0DIR_bit.P0_0=1;
}

void on_off()
{
  IO0CLR_bit.P0_0=1;
  delay(1000);
  IO0SET_bit.P0_0=1;
  delay(1000);
}

void delay(unsigned int val)
{
  for(int i=0;i<=val;i++)
  {
  }
}