#include <NXP/iolpc2124.h>


void delay_ms(unsigned int ms);
void init_gpio();
void init_adc();
unsigned int read_adc();




void main(){
  init_gpio();
  delay_ms(50);
  init_adc();
  delay_ms(50);
  while(1){
    IO0SET = read_adc();
    delay_ms(50);
  }
}



void delay_ms(unsigned int ms) {
    volatile unsigned int i, j;
    for(i=0; i<ms; i++)
        for(j=0; j<5000; j++);
}

void init_gpio(){
  PINSEL0 = 0;
  IO0DIR= 0x000000FF;
}

void init_adc(){
  PINSEL1_bit.P0_28 = 1;
  ADCR_bit.SEL=2;
  ADCR_bit.CLKDIV=12;
  ADCR_bit.CLKS=2;
  ADCR_bit.PDN=0;
}

unsigned int read_adc(){
  ADCR_bit.PDN = 1;
  delay_ms(50);
  ADCR_bit.START=1;
  while(!(ADGDR_bit.DONE==1));
  unsigned int res = ADGDR_bit.RESULT;
  ADCR_bit.START=0;
  ADCR_bit.PDN = 0;
  return res;
}