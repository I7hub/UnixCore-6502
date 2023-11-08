/* PIN 23 = D0, PIN 25 = D1, PIN 27 = D2, PIN 29 = D3, PIN 31 = D4, PIN 33 = D5, PIN 35 = D6, PIN 37 = D7 */
/* PIN 6 = PB0, PIN 7 = PB1, PIN 8 = PB2, PIN 9 = PB3, PIN 10 = PB4, PIN 11 = PB5, PIN 12 = PB6, PIN 13 = PB7 */
/* PIN 21 = PA0, PIN 20 = PA1, PIN 19 = PA2, PIN 18 = PA3, PIN 17 = PA4, PIN 16 = PA5, PIN 15 = PA6, PIN 14 = PA7 */
/* PIN 22 = A15, 
   PIN 24 = A14, 
   PIN 26 = A13, 
   PIN 28 = A12,
   PIN 30 = A11,
   PIN 32 = A10,
   PIN 34 = A9,
   PIN 36 = A8,
   PIN 38 = A7,
   PIN 40 = A6,
   PIN 42 = A5,
   PIN 44 = A4,
   PIN 46 = A3,
   PIN 48 = A2,
   PIN 50 = A1,
   PIN 52 = A0 */

#include <RPC.h>

#define AddressPins 16
#define ClockSigIN 2
#define DataPins 8
#define PinRW 3
#define ProgramDIM 19
#define IDRPORTJMask 0x007F
#define IDRPORTGMask 0x2000
#define MODERPORTJMask 0xFFFFC000
#define MODERPORTGMask 0xF3FFFFFF
#define ODRPORTJMask 0xFF80
#define ODRPORTGMask 0xDFFF

const byte UnixCore6502[ProgramDIM] = {0xa9, 0xff, 0x8d, 0x02, 0x60, 0x20, 0x0b, 0x80, 0x4c, 0x08, 0x80, 0x48, 0xa9, 0x50, 0x8d, 0x00, 0x60, 0x68, 0x60};

const char ADDR_PIN[] = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52};
const char DATA_PIN[] = {23, 25, 27, 29, 31, 33, 35, 37};

byte zeropage[256];
byte stack[256];
byte freeram[24064];
byte rom[32768];

unsigned int DATADRB = 0x00; // Data Direction Register B
unsigned int DATADRA = 0x00; // Data Direction Register A

unsigned int hexData = 0;

boolean statusBit = 0;

void setup() {

  RPC.begin();
  pinMode(ClockSigIN, INPUT);
  pinMode(PinRW, INPUT);

  for(int i = 0; i < AddressPins; i++) {
    pinMode(ADDR_PIN[i],INPUT);
  }

  for(int i = 0; i < DataPins; i++) {
    pinMode(DATA_PIN[i],INPUT);
  }
  
  memset(zeropage, 0x00, sizeof(byte)*256);
  memset(stack, 0x00, sizeof(byte)*256);
  memset(freeram, 0x00, sizeof(byte)*24064);
  memset(rom, 0x00, sizeof(byte)*32768);

  memcpy(rom, UnixCore6502, sizeof(byte)*ProgramDIM);

  rom[32765] = 0x80;
  rom[32764] = 0x00;

  RCC->AHB4ENR |= 0x0000000e;

  GPIOH->MODER = (0x00000000 | (GPIOH->MODER & ~0x00000300));
  GPIOB->MODER = (0x00000000 | (GPIOB->MODER & ~0x00C00000));
  GPIOD->MODER = (0x00000000 | (GPIOD->MODER & ~0x00003000));      
  GPIOD->MODER = (0x00000000 | (GPIOD->MODER & ~0x00000C00));      
  GPIOI->MODER = (0x00000000 | (GPIOI->MODER & ~0x000C0000));      
  GPIOH->MODER = (0x00000000 | (GPIOH->MODER & ~0x0C000000));      
  GPIOC->MODER = (0x00000000 | (GPIOC->MODER & ~0x0000C000));
  GPIOG->MODER = (0x00000000 | (GPIOG->MODER & ~0x30000000));

  GPIOD->MODER = (0x00000000 | (GPIOD->MODER & ~0x0C000000));
  GPIOB->MODER = (0x00000000 | (GPIOB->MODER & ~0x00000300));
  GPIOB->MODER = (0x00000000 | (GPIOB->MODER & ~0x00030000));
  GPIOB->MODER = (0x00000000 | (GPIOB->MODER & ~0x000C0000));
  GPIOK->MODER = (0x00000000 | (GPIOK->MODER & ~0x0000000C));
  GPIOJ->MODER = (0x00000000 | (GPIOJ->MODER & ~0x00300000));
  GPIOJ->MODER = (0x00000000 | (GPIOJ->MODER & ~0x00C00000));
  GPIOH->MODER = (0x00000000 | (GPIOH->MODER & ~0x00003000));

  attachInterrupt(digitalPinToInterrupt(ClockSigIN), clockf, RISING);
  Serial.begin(9600);

}

void check_RW() {
   
  GPIOJ->MODER = bitRead(GPIOA->IDR, 2) ? (0x00001555 | (GPIOJ->MODER & MODERPORTJMask)) : (0x00000000 | (GPIOJ->MODER & MODERPORTJMask));
  GPIOG->MODER = bitRead(GPIOA->IDR, 2) ? (0x04000000 | (GPIOG->MODER & MODERPORTGMask)) : (0x00000000 | (GPIOG->MODER & MODERPORTGMask));
  
}

void RW_mem(unsigned int address, unsigned int reference, byte memblock[]) {

  char RW = bitRead(GPIOA->IDR, 2) ? 'r' : 'W';
  unsigned int offset = address - reference;

  if( RW == 'W' ) {

    memblock[offset] = (byte) ((GPIOJ->IDR & IDRPORTJMask) << 1) + ((GPIOG->IDR & IDRPORTGMask) >> 13);

  } else if ( RW == 'r' ) {

    GPIOJ->ODR = (unsigned short) (((memblock[offset] & 0x00FE) >> 1) | (GPIOJ->ODR & ODRPORTJMask));
    GPIOG->ODR = (unsigned short) (((memblock[offset] & 0x0001) << 13) | (GPIOG->ODR & ODRPORTGMask));
    hexData = (byte) (((GPIOJ->ODR & IDRPORTJMask) << 1) + ((GPIOG->ODR & IDRPORTGMask) >> 13));
    
  }
  
}

void IO_handler(unsigned int address) {

  char RW = bitRead(GPIOA->IDR, 2) ? 'r' : 'W';

  if( RW == 'W' ) {

    byte data = (byte) ((GPIOJ->IDR & IDRPORTJMask) << 1) + ((GPIOG->IDR & IDRPORTGMask) >> 13);

    switch(address) {

      case 0x6000:
        GPIOD->ODR = bitRead(DATADRB, 0) ? (unsigned short) (((data & 0x01) << 13) | (GPIOD->ODR & 0xDFFF)) : GPIOD->ODR;
        GPIOB->ODR = bitRead(DATADRB, 1) ? (unsigned short) (((data & 0x02) << 3) | (GPIOB->ODR & 0xFFEF)) : GPIOB->ODR;
        GPIOB->ODR = bitRead(DATADRB, 2) ? (unsigned short) (((data & 0x04) << 6) | (GPIOB->ODR & 0xFEFF)) : GPIOB->ODR;
        GPIOB->ODR = bitRead(DATADRB, 3) ? (unsigned short) (((data & 0x08) << 6) | (GPIOB->ODR & 0xFDFF)) : GPIOB->ODR;
        GPIOK->ODR = bitRead(DATADRB, 4) ? (unsigned short) (((data & 0x10) >> 3) | (GPIOK->ODR & 0xFFFD)) : GPIOK->ODR;
        GPIOJ->ODR = bitRead(DATADRB, 5) ? (unsigned short) (((data & 0x20) << 5) | (GPIOJ->ODR & 0xFBFF)) : GPIOJ->ODR;
        GPIOJ->ODR = bitRead(DATADRB, 6) ? (unsigned short) (((data & 0x40) << 5) | (GPIOJ->ODR & 0xF7FF)) : GPIOJ->ODR;
        GPIOH->ODR = bitRead(DATADRB, 7) ? (unsigned short) (((data & 0x80) >> 1) | (GPIOH->ODR & 0xFFBF)) : GPIOH->ODR;
        break;
      case 0x6001:
        GPIOH->ODR = bitRead(DATADRA, 0) ? (unsigned short) (((data & 0x01) << 4) | (GPIOH->ODR & 0xFFEF)) : GPIOH->ODR;
        GPIOB->ODR = bitRead(DATADRA, 1) ? (unsigned short) (((data & 0x02) << 10) | (GPIOB->ODR & 0xF7FF)) : GPIOB->ODR;
        GPIOD->ODR = bitRead(DATADRA, 2) ? (unsigned short) (((data & 0x04) << 4) | (GPIOD->ODR & 0xFFBF)) : GPIOD->ODR;
        GPIOD->ODR = bitRead(DATADRA, 3) ? (unsigned short) (((data & 0x08) << 2) | (GPIOD->ODR & 0xFFDF)) : GPIOD->ODR;      
        GPIOI->ODR = bitRead(DATADRA, 4) ? (unsigned short) (((data & 0x10) << 5) | (GPIOI->ODR & 0xFDFF)) : GPIOI->ODR;      
        GPIOH->ODR = bitRead(DATADRA, 5) ? (unsigned short) (((data & 0x20) << 8) | (GPIOH->ODR & 0xDFFF)) : GPIOH->ODR;      
        GPIOC->ODR = bitRead(DATADRA, 6) ? (unsigned short) (((data & 0x40) << 1) | (GPIOC->ODR & 0xFF7F)) : GPIOC->ODR;      
        GPIOG->ODR = bitRead(DATADRA, 7) ? (unsigned short) (((data & 0x80) << 7) | (GPIOG->ODR & 0xBFFF)) : GPIOG->ODR;
        break;
      case 0x6002: // W
        GPIOD->ODR = bitRead(DATADRB, 0) ? (unsigned short) (0x0000 | (GPIOD->ODR & 0xDFFF)) : GPIOD->ODR;
        GPIOB->ODR = bitRead(DATADRB, 1) ? (unsigned short) (0x0000 | (GPIOB->ODR & 0xFFEF)) : GPIOB->ODR;
        GPIOB->ODR = bitRead(DATADRB, 2) ? (unsigned short) (0x0000 | (GPIOB->ODR & 0xFEFF)) : GPIOB->ODR;
        GPIOB->ODR = bitRead(DATADRB, 3) ? (unsigned short) (0x0000 | (GPIOB->ODR & 0xFDFF)) : GPIOB->ODR;
        GPIOK->ODR = bitRead(DATADRB, 4) ? (unsigned short) (0x0000 | (GPIOK->ODR & 0xFFFD)) : GPIOK->ODR;
        GPIOJ->ODR = bitRead(DATADRB, 5) ? (unsigned short) (0x0000 | (GPIOJ->ODR & 0xFBFF)) : GPIOJ->ODR;
        GPIOJ->ODR = bitRead(DATADRB, 6) ? (unsigned short) (0x0000 | (GPIOJ->ODR & 0xF7FF)) : GPIOJ->ODR;
        GPIOH->ODR = bitRead(DATADRB, 7) ? (unsigned short) (0x0000 | (GPIOH->ODR & 0xFFBF)) : GPIOH->ODR;
        DATADRB = data;
        GPIOD->MODER = bitRead(DATADRB, 0) ? (0x04000000 | (GPIOD->MODER & ~0x0C000000)) : (0x00000000 | (GPIOD->MODER & ~0x0C000000));
        GPIOB->MODER = bitRead(DATADRB, 1) ? (0x00000100 | (GPIOB->MODER & ~0x00000300)) : (0x00000000 | (GPIOB->MODER & ~0x00000300));
        GPIOB->MODER = bitRead(DATADRB, 2) ? (0x00010000 | (GPIOB->MODER & ~0x00030000)) : (0x00000000 | (GPIOB->MODER & ~0x00030000));
        GPIOB->MODER = bitRead(DATADRB, 3) ? (0x00040000 | (GPIOB->MODER & ~0x000C0000)) : (0x00000000 | (GPIOB->MODER & ~0x000C0000));
        GPIOK->MODER = bitRead(DATADRB, 4) ? (0x00000004 | (GPIOK->MODER & ~0x0000000C)) : (0x00000000 | (GPIOK->MODER & ~0x0000000C));
        GPIOJ->MODER = bitRead(DATADRB, 5) ? (0x00100000 | (GPIOJ->MODER & ~0x00300000)) : (0x00000000 | (GPIOJ->MODER & ~0x00300000));
        GPIOJ->MODER = bitRead(DATADRB, 6) ? (0x00400000 | (GPIOJ->MODER & ~0x00C00000)) : (0x00000000 | (GPIOJ->MODER & ~0x00C00000));
        GPIOH->MODER = bitRead(DATADRB, 7) ? (0x00001000 | (GPIOH->MODER & ~0x00003000)) : (0x00000000 | (GPIOH->MODER & ~0x00003000));
        break;
      case 0x6003: // W
        GPIOH->ODR = bitRead(DATADRA, 0) ? (unsigned short) (0x0000 | (GPIOH->ODR & 0xFFEF)) : GPIOH->ODR;
        GPIOB->ODR = bitRead(DATADRA, 1) ? (unsigned short) (0x0000 | (GPIOB->ODR & 0xF7FF)) : GPIOB->ODR;
        GPIOD->ODR = bitRead(DATADRA, 2) ? (unsigned short) (0x0000 | (GPIOD->ODR & 0xFFBF)) : GPIOD->ODR;
        GPIOD->ODR = bitRead(DATADRA, 3) ? (unsigned short) (0x0000 | (GPIOD->ODR & 0xFFDF)) : GPIOD->ODR;      
        GPIOI->ODR = bitRead(DATADRA, 4) ? (unsigned short) (0x0000 | (GPIOI->ODR & 0xFDFF)) : GPIOI->ODR;      
        GPIOH->ODR = bitRead(DATADRA, 5) ? (unsigned short) (0x0000 | (GPIOH->ODR & 0xDFFF)) : GPIOH->ODR;      
        GPIOC->ODR = bitRead(DATADRA, 6) ? (unsigned short) (0x0000 | (GPIOC->ODR & 0xFF7F)) : GPIOC->ODR;      
        GPIOG->ODR = bitRead(DATADRA, 7) ? (unsigned short) (0x0000 | (GPIOG->ODR & 0xBFFF)) : GPIOG->ODR;
        DATADRA = data;
        GPIOH->MODER = bitRead(DATADRA, 0) ? (0x00000100 | (GPIOH->MODER & ~0x00000300)) : (0x00000000 | (GPIOH->MODER & ~0x00000300));
        GPIOB->MODER = bitRead(DATADRA, 1) ? (0x00400000 | (GPIOB->MODER & ~0x00C00000)) : (0x00000000 | (GPIOB->MODER & ~0x00C00000));
        GPIOD->MODER = bitRead(DATADRA, 2) ? (0x00001000 | (GPIOD->MODER & ~0x00003000)) : (0x00000000 | (GPIOD->MODER & ~0x00003000));      
        GPIOD->MODER = bitRead(DATADRA, 3) ? (0x00000400 | (GPIOD->MODER & ~0x00000C00)) : (0x00000000 | (GPIOD->MODER & ~0x00000C00));      
        GPIOI->MODER = bitRead(DATADRA, 4) ? (0x00040000 | (GPIOI->MODER & ~0x000C0000)) : (0x00000000 | (GPIOI->MODER & ~0x000C0000));      
        GPIOH->MODER = bitRead(DATADRA, 5) ? (0x04000000 | (GPIOH->MODER & ~0x0C000000)) : (0x00000000 | (GPIOH->MODER & ~0x0C000000));      
        GPIOC->MODER = bitRead(DATADRA, 6) ? (0x00004000 | (GPIOC->MODER & ~0x0000C000)) : (0x00000000 | (GPIOC->MODER & ~0x0000C000));
        GPIOG->MODER = bitRead(DATADRA, 7) ? (0x10000000 | (GPIOG->MODER & ~0x30000000)) : (0x00000000 | (GPIOG->MODER & ~0x30000000));
        break;
      default:
        break;
      
    }
  
}
  
}

void clockf() {

  statusBit = 1;
  
}

void loop() {

  if( statusBit == 1 ) {

    long int t1 = micros();
    statusBit = 0;

    char output[15];
    unsigned int address = 0;
    int bit = 0;

    check_RW();
  
    address = (unsigned int) ((GPIOJ->IDR & 0x1000) << 3) + ((GPIOG->IDR & 0x1000) << 2) + ((GPIOJ->IDR & 0x4000) >> 1) + ((GPIOJ->IDR & 0x8000) >> 3) + ((GPIOK->IDR & 0x0008) << 8) + ((GPIOK->IDR & 0x0010) << 6) + ((GPIOK->IDR & 0x0020) << 4) + ((GPIOK->IDR & 0x0040) << 2) + (GPIOJ->IDR & 0x0080) + (GPIOE->IDR & 0x0040) + ((GPIOI->IDR & 0x8000) >> 10) + ((GPIOG->IDR & 0x0400) >> 6) + ((GPIOH->IDR & 0x8000) >> 12) + ((GPIOK->IDR & 0x0001) << 2) + ((GPIOI->IDR & 0x0800) >> 10) + ((GPIOK->IDR & 0x0004) >> 2);

    if ((address >= 0x0000) && (address <= 0x00FF)) {

      RW_mem(address, 0x0000, zeropage);
    
    } else if ((address >= 0x0100) && (address <= 0x01FF)) {

      RW_mem(address, 0x0100, stack);
    
    } else if ((address >= 0x0200) && (address <= 0x5FFF)) {
    
      RW_mem(address, 0x0200, freeram);
    
    } else if ((address >= 0x6000) && (address <= 0x7FFF)) {

      IO_handler(address);
    
    } else if ((address >= 0x8000) && (address <= 0xFFFF)) {
    
      unsigned int offset = address - 0x8000;
      GPIOJ->ODR = (unsigned short) (((rom[offset] & 0x00FE) >> 1) | (GPIOJ->ODR & ODRPORTJMask));
      GPIOG->ODR = (unsigned short) (((rom[offset] & 0x0001) << 13) | (GPIOG->ODR & ODRPORTGMask));
      hexData = (byte) (((GPIOJ->ODR & IDRPORTJMask) << 1) + ((GPIOG->ODR & IDRPORTGMask) >> 13));
    
    }

    long int t2 = micros();
    Serial.print(t2-t1);

    char RW = bitRead(GPIOA->IDR, 2) ? 'r' : 'W';
    
    if ( RW == 'W' ) {

      for(int i = 0; i < DataPins; i++) {
      
        bit = digitalRead(DATA_PIN[7-i]) ? 1 : 0;
        hexData = (hexData << 1) + bit;
      
      }
    
    }

    sprintf(output, " %04x %c %02x", address, RW, hexData);
    Serial.println(output);
    hexData = 0;

  }

}
