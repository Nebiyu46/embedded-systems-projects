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

void lcd_print(char *str) {
    while(*str) {
        lcd_send_data(*str++);
    }
}
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
// LCD Functions
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

void int_to_string(int num, char* str) {
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
        return;
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
}

int operation(int operand1, int operand2, char operator){
  if (operator == '+'){
    return operand1 + operand2;
  }
  else if (operator == '-'){
    return operand1 - operand2;
  }
  else if (operator == '*'){
    return (int) operand1 * operand2;
  }
  else {
    return (int) operand1 / operand2;
  }
  
}


int main(void) {
    char key;
    int pos = 0;
    
    lcd_init();
    keypad_init();
    
    lcd_cmd(LCD_CLEAR);
    //lcd_print("Press Key:");
    //lcd_cmd(LCD_LINE2);
    char num1[4] = {};
    char num2[4] = {};
    int posi1 = 0;
    int posi2 = 0;
    const char operations[4] = {'/', '+', '-', '*'};
    char operator;
    bool onsec = false;
    bool resett = false;
    
    while(1) {
        key = keypad_scan();
        if(key) {
          if (resett){
          key = 'O';
          resett = false;
          }
          if(pos >= 16) {
              pos = 0;
              lcd_cmd(LCD_LINE1);
              for(int i = 0; i < 16; i++) lcd_data(' ');
              lcd_cmd(LCD_LINE1);
              continue;
          }
          if((key == operations[0] || key == operations [1] || key == operations[2] || key == operations[3])){
            onsec = true;
            operator = key;
          }
          else if (key == '='){
            int operand1 = array_to_int(num1, posi1);
            int operand2 = array_to_int(num2, posi2);
            
            int result = operation(operand1, operand2, operator);
            
            char result_str[16];  // Make it big enough for any result
            int_to_string(result, result_str);
            
            lcd_data(key);
            pos++;
            lcd_print(result_str);
            resett = true;
            continue;
          }
          else if(key == 'O'){
            pos = 0;
            lcd_cmd(LCD_LINE1);
            for(int i = 0; i < 16; i++) lcd_data(' ');
            lcd_cmd(LCD_LINE1);
            num1[0]='\0';
            num2[0]='\0';
            posi1 = 0;
            posi2 = 0;
            onsec = false;
            continue;
          }
          else{
            if(onsec){
              num2[posi2]=key;
              posi2++;
              num2[posi2] = '\0';
            }
            else{
              num1[posi1]=key;
              posi1++;
              num1[posi1]= '\0';
            }
          }
          
          lcd_data(key);
          pos++;
        }
    }
}


