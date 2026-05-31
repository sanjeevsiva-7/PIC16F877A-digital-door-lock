#include <xc.h>

// 1. Configuration Bits (Tells the chip how to boot)
#pragma config FOSC = HS        // High-Speed Crystal Oscillator
#pragma config WDTE = OFF       // Watchdog Timer Off
#pragma config PWRTE = OFF      // Power-up Timer Off
#pragma config BOREN = OFF      // Brown-out Reset Off
#pragma config LVP = OFF        // Low-Voltage Programming Off

#define _XTAL_FREQ 20000000     // Running at 20MHz

// 2. Define LCD Control Pins
#define RS PORTBbits.RB0
#define EN PORTBbits.RB1

// 3. Define Keypad Pins based on YOUR Proteus schematic
#define ROW_A PORTDbits.RD4     // Row A Output
#define ROW_B PORTDbits.RD5     // Row B Output
#define ROW_C PORTDbits.RD6     // Row C Output
#define ROW_D PORTDbits.RD7     // Row D Output

#define COL_1 PORTDbits.RD0     // Column 1 Input (with R1 Pull-up)
#define COL_2 PORTDbits.RD1     // Column 2 Input (with R2 Pull-up)
#define COL_3 PORTDbits.RD2     // Column 3 Input (with R3 Pull-up)

// ? NEW: Define the Relay Actuator Pin Connection
#define LOCK_ACTUATOR PORTCbits.RC0  // High = Unlock (Relay clicks / LED turns ON)

// 4. Global Variables for Password Logic
const char master_code[4] = {'1', '2', '3', '4'}; // Our secret password
char user_code[4];                               // Storage for user entry
unsigned char key_count = 0;                     // Tracker for total keys typed

// Function Declarations
char scan_keypad_simple(void);
void lcd_write(char value, unsigned char is_data);
void lcd_cmd(char cmd);
void lcd_char(char data);
void lcd_string(char *str);
void lcd_init(void);

// ? Main Program
void main() {
    lcd_init();
    
    // Configure PORTD Pins matching your layout:
    // RD7, RD6, RD5, RD4 are Outputs (0) -> Rows
    // RD3, RD2, RD1, RD0 are Inputs (1)  -> Columns
    // Binary: 0b00001111 = 0x0F
    TRISD = 0x0F; 
    
    // Set all Row output pins High initially
    PORTD = 0xF0; 
    
    // ? NEW: Configure PORTC Pin RC0 as an Output (0)
    TRISCbits.TRISC0 = 0; 
    LOCK_ACTUATOR = 0;    // Keep the lock locked (Relay OFF) at boot up
    
    lcd_string("ENTER PASSWORD:");
    lcd_cmd(0xC0); // Move cursor down to the second line
    
    char pressed_key = 0;
    
    while(1) {
        pressed_key = scan_keypad_simple(); 
        
        if(pressed_key != 0) {
            
            // 1. Save the key into our user array buffer at the current index position
            user_code[key_count] = pressed_key; 
            
            // 2. Print an asterisk on the LCD for visual privacy masking
            lcd_char('*'); 
            
            // 3. Move our pointer index forward by one slot
            key_count++; 
            
            // 4. Check if the user has completed typing all 4 digits
            if(key_count == 4) {
                __delay_ms(500); // Small pause for the simulation user to see their last asterisk
                lcd_cmd(0x01);   // Clear screen entirely
                
                // 5. Compare user_code array against master_code array
                unsigned char match = 1; // Assume it's correct first
                for(int i = 0; i < 4; i++) {
                    if(user_code[i] != master_code[i]) {
                        match = 0; // Found a mismatch flag!
                    }
                }
                
                // 6. Output decision based on verification check
                // ? MODIFIED: Controlling your physical lock hardware right here!
                if(match == 1) {
                    lcd_string("ACCESS GRANTED");
                    LOCK_ACTUATOR = 1;   // Turn on Transistor -> 12V Relay clicks -> Green LED lights up!
                    __delay_ms(5000);    // Keep the door unlocked safely for 5 seconds
                    LOCK_ACTUATOR = 0;   // Relock the door automatically
                } else {
                    lcd_string("ACCESS DENIED");
                    __delay_ms(2000);    // Keep error message on screen for 2 seconds
                }
                
                // 7. System Reset Loop Protocol
                lcd_cmd(0x01);   // Clear screen
                lcd_string("ENTER PASSWORD:");
                lcd_cmd(0xC0);   // Return cursor to line 2
                
                key_count = 0;   // Reset our index counter back to slot 0 for next attempt
            }
        }
    }
}

// ? The Simple, Manual Keypad Scanner Engine
char scan_keypad_simple(void) {
    
    // ==================== CHECK ROW A ====================
    ROW_A = 0; ROW_B = 1; ROW_C = 1; ROW_D = 1; // Drop Row A to 0V
    __delay_us(5); // Wait for voltage to settle
    
    if(COL_1 == 0) 
    { 
      while(COL_1 == 0); //Debounce Trap: wait for finger release
      return '1'; 
    } // Row A + Col 1 = '1'
    if (COL_2==0)
    {
       while(COL_2==0);
       return '2';
    }// Row A + Col 2 = '2'
    
    if(COL_3 == 0)
    {
       while(COL_3==0);
       return '3';
    }// Row A + Col 3 = '3'
    
    // ==================== CHECK ROW B ====================
    ROW_A = 1; ROW_B = 0; ROW_C = 1; ROW_D = 1; // Drop Row B to 0V
    __delay_us(5);
    
    if(COL_1 == 0) { while(COL_1 == 0); return '4'; } // Row B + Col 1 = '4'
    if(COL_2 == 0) { while(COL_2 == 0); return '5'; } // Row B + Col 2 = '5'
    if(COL_3 == 0) { while(COL_3 == 0); return '6'; } // Row B + Col 3 = '6'
    
    // ==================== CHECK ROW C ====================
    ROW_A = 1; ROW_B = 1; ROW_C = 0; ROW_D = 1; // Drop Row C to 0V
    __delay_us(5);
    
    if(COL_1 == 0) { while(COL_1 == 0); return '7'; } // Row C + Col 1 = '7'
    if(COL_2 == 0) { while(COL_2 == 0); return '8'; } // Row C + Col 2 = '8'
    if(COL_3 == 0) { while(COL_3 == 0); return '9'; } // Row C + Col 3 = '9'
    
    // ==================== CHECK ROW D ====================
    ROW_A = 1; ROW_B = 1; ROW_C = 1; ROW_D = 0; // Drop Row D to 0V
    __delay_us(5);
    
    if(COL_1 == 0) { while(COL_1 == 0); return '*'; } // Row D + Col 1 = '*'
    if(COL_2 == 0) { while(COL_2 == 0); return '0'; } // Row D + Col 2 = '0'
    if(COL_3 == 0) { while(COL_3 == 0); return '#'; } // Row D + Col 3 = '#'
    
    return 0; // Return 0 if no keys are being pressed
}

// --- Your Working LCD Driver Functions ---
void lcd_write(char value, unsigned char is_data) {
    RS = is_data; 
    PORTB = (PORTB & 0x03) | ((value & 0xF0) >> 2); 
    EN = 1; __delay_ms(2); EN = 0;
    
    PORTB = (PORTB & 0x03) | ((value & 0x0F) << 2); 
    EN = 1; __delay_ms(2); EN = 0;   
}

void lcd_cmd(char cmd) 
{ 
    lcd_write(cmd, 0); 
}
void lcd_char(char data) 
{
    lcd_write(data, 1); 
}
void lcd_string(char *str) 
{
    while(*str) 
    {
        lcd_char(*str++);
    }
}

void lcd_init() {
    TRISB = 0x00;      
    PORTB = 0x00;      
    __delay_ms(20);    
    lcd_cmd(0x33);     
    lcd_cmd(0x32);     
    lcd_cmd(0x28);     
    lcd_cmd(0x0C);     
    lcd_cmd(0x06);     
    lcd_cmd(0x01);     
    __delay_ms(2);     
}