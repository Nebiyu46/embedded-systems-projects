#include <NXP/iolpc2124.h>

// LCD Pin Connections (Modify these based on your hardware)
#define LCD_RS   4   
#define LCD_E    5   
#define LCD_D4   0   
#define LCD_D5   1   
#define LCD_D6   2   
#define LCD_D7   3   

// Function Prototypes
void lcd_init(void);
void lcd_send_cmd(unsigned char cmd);
void lcd_send_data(unsigned char data);
void lcd_print(char *str);
void lcd_pulse_enable(void);
void delay_ms(unsigned int ms);
void delay_us(unsigned int us);

int main(void) {
    // Initialize LCD
    lcd_init();
    
    // Display "Hello World"
    lcd_send_cmd(0x80);          // Move to line 1
    lcd_print("Hello World!");
    
    // Optional: Display on second line
    lcd_send_cmd(0xC0);          // Move to line 2
    lcd_print("LPC2124 LCD Test");

   
}

// Initialize LCD in 4-bit mode
void lcd_init(void) {
    // Configure GPIO pins as outputs
    PINSEL0 &= ~(0xFF << 0);     // Set P0.0-P0.7 as GPIO
    IO0DIR |= (1<<LCD_RS) | (1<<LCD_E) | (1<<LCD_D4) | (1<<LCD_D5) | (1<<LCD_D6) | (1<<LCD_D7);

    delay_ms(50);                // Wait for LCD power-up
    
    // Initialization sequence for 4-bit mode
    IO0CLR = (1<<LCD_RS);        // RS=0 (command mode)
    
    // Send 0x03 three times
    IO0CLR = (0xF << LCD_D4);    // Clear data bits
    IO0SET = (0x3 << LCD_D4);    // Set D7-D4 = 0011
    lcd_pulse_enable();
    delay_ms(5);
    
    lcd_pulse_enable();
    delay_us(100);
    
    lcd_pulse_enable();
    delay_us(100);
    
    // Switch to 4-bit mode
    IO0CLR = (0xF << LCD_D4);
    IO0SET = (0x2 << LCD_D4);    // D7-D4 = 0010
    lcd_pulse_enable();
    delay_us(100);

    // 4-bit mode, 2 lines, 5x8 font
    lcd_send_cmd(0x28);
    delay_us(50);
    
    // Display on, cursor off
    lcd_send_cmd(0x0C);
    delay_us(50);
    
    // Clear display
    lcd_send_cmd(0x01);
    delay_ms(2);
    
    // Entry mode: increment cursor
    lcd_send_cmd(0x06);
    delay_us(50);
}

// Send command to LCD
void lcd_send_cmd(unsigned char cmd) {
    IO0CLR = (1<<LCD_RS);        // RS=0 for command
    
    // Send high nibble
    IO0CLR = (0xF << LCD_D4);
    IO0SET = ((cmd >> 4) & 0x0F) << LCD_D4;
    lcd_pulse_enable();
    
    // Send low nibble
    IO0CLR = (0xF << LCD_D4);
    IO0SET = (cmd & 0x0F) << LCD_D4;
    lcd_pulse_enable();
    
    delay_us(50);                // Wait for command execution
}

// Send data to LCD
void lcd_send_data(unsigned char data) {
    IO0SET = (1<<LCD_RS);        // RS=1 for data
    
    // Send high nibble
    IO0CLR = (0xF << LCD_D4);
    IO0SET = ((data >> 4) & 0x0F) << LCD_D4;
    lcd_pulse_enable();
    
    // Send low nibble
    IO0CLR = (0xF << LCD_D4);
    IO0SET = (data & 0x0F) << LCD_D4;
    lcd_pulse_enable();
    
    delay_us(5000);
}

// Pulse Enable pin
void lcd_pulse_enable(void) {
    IO0SET = (1<<LCD_E);
    delay_us(1);
    IO0CLR = (1<<LCD_E);
    delay_us(100);
}

// Print string to LCD
void lcd_print(char *str) {
    while(*str) {
        lcd_send_data(*str++);
    }
}

// Simple delay functions
void delay_ms(unsigned int ms) {
    volatile unsigned int i, j;
    for(i=0; i<ms; i++)
        for(j=0; j<5000; j++);
}

void delay_us(unsigned int us) {
    volatile unsigned int i;
    for(i=0; i<us; i++);
}