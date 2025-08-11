#include "NXP/iolpc2124.h"
#include "math.h"
#include <stdbool.h>


// LCD Pin Definitions
#define LCD_RS   0    // P0.0 - Register Select
#define LCD_RW   1    // P0.1 - Read/Write
#define LCD_E    2    // P0.2 - Enable
#define LCD_D4   3    // P0.3 - Data bit 4
#define LCD_D5   4    // P0.4 - Data bit 5
#define LCD_D6   5    // P0.5 - Data bit 6
#define LCD_D7   6    // P0.6 - Data bit 7

// Keypad Pin Definitions
#define KP_R1    7    // P0.7  - Row 1
#define KP_R2    8    // P0.8  - Row 2
#define KP_R3    9    // P0.9  - Row 3
#define KP_R4    10   // P0.10 - Row 4
#define KP_C1    11   // P0.11 - Column 1
#define KP_C2    12   // P0.12 - Column 2
#define KP_C3    13   // P0.13 - Column 3
#define KP_C4    14   // P0.14 - Column 4

// LCD Commands
#define LCD_CLEAR       0x01
#define LCD_HOME        0x02
#define LCD_ENTRY_MODE  0x06
#define LCD_DISPLAY_ON  0x0C
#define LCD_FUNC_SET    0x28
#define LCD_LINE1       0x80
#define LCD_LINE2       0xC0


// Keypad layout
const char keymap[4][4] = {
    {'7', '8', '9', '/'},
    {'4', '5', '6', '*'},
    {'1', '2', '3', '-'},
    {'O', '0', '=', '+'}
};

// Delay functions
void delay_ms(unsigned int ms) {
    volatile unsigned int i, j;
    for(i = 0; i < ms; i++)
        for(j = 0; j < 100; j++);  // Much faster
}
void delay_us(unsigned int us) {
    volatile unsigned int i;
    for(i = 0; i < us/10; i++);  // Much faster
}

void lcd_write(unsigned char data, int rs) {
    if(rs) IO0SET = (1 << LCD_RS);
    else   IO0CLR = (1 << LCD_RS);
    
    IO0CLR = (1 << LCD_RW);
    delay_ms(1);
    
    // Send high nibble
    IO0CLR = (0xF << LCD_D4);
    IO0SET = ((data >> 4) & 0xF) << LCD_D4;
    IO0SET = (1 << LCD_E);
    delay_us(1);
    IO0CLR = (1 << LCD_E);
    
    delay_us(1);
    
    // Send low nibble
    IO0CLR = (0xF << LCD_D4);
    IO0SET = (data & 0xF) << LCD_D4;
    IO0SET = (1 << LCD_E);
    delay_us(1);
    IO0CLR = (1 << LCD_E);
    
    delay_ms(2);
}

void lcd_cmd(unsigned char cmd) {
    lcd_write(cmd, 0);
}

void lcd_data(unsigned char data) {
    lcd_write(data, 1);
}

void lcd_init(void) {
    // Set LCD pins as outputs
    IO0DIR |= (0x7F << 0);
    
    delay_ms(20);
    
    // Initialize 4-bit mode
    IO0CLR = (1 << LCD_RS) | (1 << LCD_RW);
    
    IO0CLR = (0xF << LCD_D4);
    IO0SET = (0x3 << LCD_D4);
    IO0SET = (1 << LCD_E);
    delay_us(1);
    IO0CLR = (1 << LCD_E);
    delay_ms(5);
    
    IO0SET = (1 << LCD_E);
    delay_us(1);
    IO0CLR = (1 << LCD_E);
    delay_us(100);
    IO0SET = (1 << LCD_E);
    delay_us(1);
    IO0CLR = (1 << LCD_E);
    delay_us(100);
    
    IO0CLR = (0xF << LCD_D4);
    IO0SET = (0x2 << LCD_D4);
    IO0SET = (1 << LCD_E);
    delay_us(1);
    IO0CLR = (1 << LCD_E);
    delay_us(100);
    
    lcd_cmd(LCD_FUNC_SET);
    lcd_cmd(LCD_DISPLAY_ON);
    lcd_cmd(LCD_CLEAR);
    delay_ms(2);
    lcd_cmd(LCD_ENTRY_MODE);
}

void lcd_print(const char *str) {
    while(*str) lcd_data(*str++);
}

// Keypad Functions
void keypad_init(void) {
    // Configure GPIO
    PINSEL0 &= ~(
        (3 << (KP_R1*2)) | (3 << (KP_R2*2)) |
        (3 << (KP_R3*2)) | (3 << (KP_R4*2)) |
        (3 << (KP_C1*2)) | (3 << (KP_C2*2)) |
        (3 << (KP_C3*2)) | (3 << (KP_C4*2))
    );
    
    // Set rows as outputs
    IO0DIR |= (
        (1 << KP_R1) | (1 << KP_R2) |
        (1 << KP_R3) | (1 << KP_R4)
    );
    
    // Set columns as inputs
    IO0DIR &= ~(
        (1 << KP_C1) | (1 << KP_C2) |
        (1 << KP_C3) | (1 << KP_C4)
    );
    
    // Set all rows HIGH initially
    IO0SET = (
        (1 << KP_R1) | (1 << KP_R2) |
        (1 << KP_R3) | (1 << KP_R4)
    );
}

//convertors 

int array_to_int(char* array, int size){
  int result = 0;
  for (int i = 0; i < size; i++){
    if (array[i] == '\0'){
      break;
    }
    result += (array[i]-'0') * pow(10, size-i-1);
  }
  return result;
}

char* int_to_string(int num) {
  char* str;
  int i = 0;
  int temp = num;
  int length = 0;
  
  // Handle negative numbers
  if(num < 0) {
      str[i++] = '-';
      num = -num;
  }
  // Handle zero separately
  if(num == 0) {
      str[i++] = '0';
      str[i] = '\0';
      return str;
  }
  
  // Find length of number
  temp = num;
  while(temp > 0) {
      length++;
      temp /= 10;
  }
  
  // Add digits in reverse order
  for(int j = length-1; j >= 0; j--) {
      str[j] = (num % 10) + '0';
      num /= 10;
  }
  str[length] = '\0';
  return str;
}

void ADC_Init() {
    PINSEL1 |= (1 << 19);  // P0.25 as AD0.0 (Analog Input)
    ADCR = (1 << 21)      // Enable ADC (PDN = 1)
          | (4 << 8);      // CLKDIV = 4 (for 2.4 MHz ADC clock @ 12 MHz PCLK)
    delay_ms(1);           // ADC stabilization
}

int ADC_Read(int channel) {
    ADCR &= ~(0xFF);          // Clear channel selection
    ADCR |= (1 << channel);   // Select channel (0-3)
    ADCR |= (1 << 24);        // Start conversion (START = 1)
    while (!(ADDR & (1 << 31))); // Wait for DONE flag
    return (ADDR >> 6) & 0x3FF;  // Return 10-bit result
}

char keypad_scan(void) {
    int row, col;
    int rows[] = {KP_R1, KP_R2, KP_R3, KP_R4};
    int cols[] = {KP_C1, KP_C2, KP_C3, KP_C4};
    
    // Make sure all rows are HIGH initially
    IO0CLR = (1 << KP_R1) | (1 << KP_R2) | (1 << KP_R3) | (1 << KP_R4);
    delay_ms(10);
    
    // Scan each row
    for(row = 0; row < 4; row++) {
        // Set only current row LOW
        IO0SET = (1 << rows[row]);
        delay_ms(5);
        
        // Read all columns for this row
        unsigned long cols_state = IO0PIN;
        
        // Check each column
        for(col = 0; col < 4; col++) {
            if((IO0PIN & (1 << cols[col]))) {
                // Key pressed - verify
                delay_ms(10);
                if((IO0PIN & (1 << cols[col]))) {
                    // Wait for key release
                    while((IO0PIN & (1 << cols[col])));
                    delay_ms(10);  // Debounce release
                    
                    // Reset row to HIGH
                    IO0SET = (1 << rows[row]);
                    delay_ms(5);
                    return keymap[row][col];
                }
            }
        }
        
        // Set row back to HIGH before moving to next
        IO0CLR = (1 << rows[row]);
        delay_ms(5);
    }
    
    return 0;
}

int process_gain(int percent,int digVal){
  if (digVal < (3.3*percent/100){
    return 0
  }
  else {
    return 1
  }
}

void PWM_Init() {
  // Step 1: Enable PWM Timer
  PWMPCR = (1 << 13);  // Enable PWM5 output (single-edge mode)
  PWMPR = 0;           // No prescaler (PWM clock = PCLK = 12MHz)
  
  // Step 2: Set PWM Frequency (e.g., 1kHz)
  PWMMR0 = 12000 - 1;  // PWM frequency = PCLK / (PWMMR0 + 1) = 1kHz
  
  // Step 3: Set Initial Duty Cycle (e.g., 50%)
  PWMMR5 = 6000;       // Duty cycle = (PWMMR5 / PWMMR0) = 50%
  
  // Step 4: Enable PWM
  PWMTCR = (1 << 1);   // Reset PWM counter
  PWMTCR = (1 << 0) | (1 << 3);  // Enable PWM + Counter
}

void PWM_SetDuty(int duty_percent) {
    // Constrain duty cycle to 0-100%
    if (duty_percent < 0) duty_percent = 0;
    if (duty_percent > 100) duty_percent = 100;
    
    // Update duty cycle
    PWMMR5 = (PWMMR0 * duty_percent) / 100;
    PWMLER = (1 << 5);  // Latch new value for PWM5
}

int main(void) {
    int threshold;
    
    char key;
    int pos = 0;
    
    lcd_init();
    keypad_init();
    
    const char operations[] = "Threshold: ";
    char inp_buf[4];
    lcd_print(operations);
    
    lcd_cmd(LCD_CLEAR);
    
    while(1) {
      key = keypad_scan();
      if(key) {
        if (key == '='){
          threshold = array_to_int(inp_buf, pos);
          break;          
        }
        inp_buf[pos]=key;
        pos++;
        inp_buf[pos] = '\0';                
      }
    }
    ADC_Init();
    PWM_Init();
    while(1){
      int adc_val = ADC_Read(0);  // Read AD0.0 (P0.25)
      adc_val = adc_val * process_gain(threshold,adc_val);
      
      PWM_SetDuty(100*(adc_val/1023));
      lcd_cmd(LCD_CLEAR);
      
      float voltage = (adc_val / 1023.0) * 3.3;
      lcd_print(int_to_string(voltage));
      
      delay_ms(100);  // Sample every 100ms (adjust as needed)
    } 
}