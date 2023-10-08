/* PIN 23 = D0, PIN 25 = D1, PIN 27 = D2, PIN 29 = D3, PIN 31 = D4, PIN 33 = D5, PIN 35 = D6, PIN 37 = D7 */
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
#define MODERPORTJMask 0x00003FFF
#define MODERPORTGMask 0x0C000000
#define ODRPORTJMask 0x00FE
#define ODRPORTGMask 0x0001

const byte UnixCore6502[ProgramDIM] = {0xa9, 0xff, 0x8d, 0x02, 0x60, 0x20, 0x0b, 0x80, 0x4c, 0x08, 0x80, 0x48, 0xa9, 0x50, 0x8d, 0x00, 0x60, 0x68, 0x60};

const char ADDR_PIN[] = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52};
const char DATA_PIN[] = {23, 25, 27, 29, 31, 33, 35, 37};

byte zeropage[256];
byte stack[256];
byte freeram[24064];
byte rom[32768];

boolean OPB[8]; // Output Port B
boolean OPA[8]; // Output Port A
boolean IPB[8]; // Input Port B
boolean IPA[8]; // Input Port A

unsigned int DATADRB = 0; // Data Direction Register B
unsigned int DATADRA = 0; // Data Direction Register A

char OLD_RW = 'u'; // set OLD_RW to undefined
char RW = 'u'; // set RW to undefined

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

  memset(OPB, 0, sizeof(boolean)*8);
  memset(OPA, 0, sizeof(boolean)*8);
  memset(IPB, 0, sizeof(boolean)*8);
  memset(IPA, 0, sizeof(boolean)*8);

  memcpy(rom, UnixCore6502, sizeof(byte)*ProgramDIM);

  rom[32765] = 0x80;
  rom[32764] = 0x00;

  OLD_RW = 'W';

  attachInterrupt(digitalPinToInterrupt(ClockSigIN), clockf, RISING);

  Serial.begin(9600);

}

void check_RW() {
  
  RW = bitRead(GPIOA->IDR, 2) ? 'r' : 'W';
  
  if ((RW == 'W') && (OLD_RW != 'W')) { // INPUT
   
    GPIOJ->MODER = (0x00000000 | (GPIOJ->MODER & ~MODERPORTJMask));
    GPIOG->MODER = (0x00000000 | (GPIOG->MODER & ~MODERPORTGMask));
    
    OLD_RW = RW;
    
  }
  
  if ((RW == 'r') && (OLD_RW != 'r')) { // OUTPUT
    
    GPIOJ->MODER = (0x00001555 | (GPIOJ->MODER & ~MODERPORTJMask));
    GPIOG->MODER = (0x04000000 | (GPIOG->MODER & ~MODERPORTGMask));
    
    OLD_RW = RW;
    
  }
  
}

void RW_mem(unsigned int address, unsigned int reference, byte memblock[]) {

unsigned int offset = address - reference;

  if( RW == 'W' ) {

    memblock[offset] = (byte) ((GPIOJ->IDR & IDRPORTJMask) << 1) + ((GPIOG->IDR & IDRPORTGMask) >> 13);

  } else if ( RW == 'r' ) {

    GPIOJ->ODR = (unsigned short) (((memblock[offset] & ODRPORTJMask) >> 1) | (GPIOJ->ODR & 0xFF80));
    GPIOG->ODR = (unsigned short) (((memblock[offset] & ODRPORTGMask) << 13) | (GPIOG->ODR & 0xDFFF));
    hexData = (byte) (((GPIOJ->ODR & IDRPORTJMask) << 1) + ((GPIOG->ODR & IDRPORTGMask) >> 13));
    
  }
  
}

void IO_handler(unsigned int address) {

if( RW == 'W' ) {

byte data = (byte) ((GPIOJ->IDR & IDRPORTJMask) << 1) + ((GPIOG->IDR & IDRPORTGMask) >> 13);

  switch(address) {

    case 0x6000:
      for(int i = 0; i < DataPins; i++) {

        if ( bitRead(DATADRB, (7-i)) == 1 ) {
          
          OPB[7-i] = bitRead(data, (7-i));
        
        }
      
      }
      break;
    case 0x6001:
      for(int i = 0; i < DataPins; i++) {

        if ( bitRead(DATADRA, (7-i)) == 1 ) {
          
          OPA[7-i] = bitRead(data, (7-i));
        
        }
      
      }
      break;
    case 0x6002: // W
      DATADRB = data;
      break;
    case 0x6003: // W
      DATADRA = data;
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
      GPIOJ->ODR = (unsigned short) (((rom[offset] & ODRPORTJMask) >> 1) | (GPIOJ->ODR & 0xFF80));
      GPIOG->ODR = (unsigned short) (((rom[offset] & ODRPORTGMask) << 13) | (GPIOG->ODR & 0xDFFF));
      hexData = (byte) (((GPIOJ->ODR & IDRPORTJMask) << 1) + ((GPIOG->ODR & IDRPORTGMask) >> 13));
    
    }

    long int t2 = micros();
    Serial.print(t2-t1);

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
