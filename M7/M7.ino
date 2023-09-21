/* PIN 23 = D7, PIN 25 = D6, PIN 27 = D5 e così via */
/* PIN 22 = A15, PIN 24 = A14, PIN 26 = A13 e così via */

#include <RPC.h>

#define ADDRN 16
#define CLOCK 2
#define DATAN 8
#define READ_WRITE_PIN 3
#define DIMPROG 19

const byte PROG[DIMPROG] = {0xa9, 0xff, 0x8d, 0x02, 0x60, 0x20, 0x0b, 0x80, 0x4c, 0x08, 0x80, 0x48, 0xa9, 0x50, 0x8d, 0x00, 0x60, 0x68, 0x60};

const char ADDR_PIN[] = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52};
const char DATA_PIN[] = {23, 25, 27, 29, 31, 33, 35, 37};

byte stack[256];
byte zeropage[256];
byte freeram[24064];

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
  pinMode(CLOCK, INPUT);
  pinMode(READ_WRITE_PIN, INPUT);

  for(int i = 0; i < ADDRN; i++) {
      pinMode(ADDR_PIN[i],INPUT);
  }

  for(int i = 0; i < DATAN; i++) {
      pinMode(DATA_PIN[i],INPUT);
  }
  
  memset(zeropage, 0x00, sizeof(byte)*256);
  memset(stack, 0x00, sizeof(byte)*256);
  memset(freeram, 0x00, sizeof(byte)*24064);
  memset(OPB, 0, sizeof(boolean)*8);
  memset(OPA, 0, sizeof(boolean)*8);
  memset(IPB, 0, sizeof(boolean)*8);
  memset(IPA, 0, sizeof(boolean)*8);

  OLD_RW = 'W';

  attachInterrupt(digitalPinToInterrupt(CLOCK), clockf, RISING);

  Serial.begin(9600);

}

void check_RESET(unsigned int address) { // OK
  
  if (address == 0xFFFC) { 
    
    for (int i = 0; i < 8; i++) {  
      
      digitalWrite(DATA_PIN[i], LOW); // 0x00 for 0xFFFC
      
    }    
    
  } else if (address == 0xFFFD) {
    
    digitalWrite(DATA_PIN[0], HIGH); // 0x80 for 0xFFFD
    hexData = 0x80;
    
  }
  
}

void check_RW() { // OK
  
  RW = digitalRead(READ_WRITE_PIN) ? 'r' : 'W';
  
  if ((RW == 'W') && (OLD_RW != 'W')) {
   
    for (int i = 0; i < 8; i++) {  
      
      pinMode(DATA_PIN[i], INPUT);
      
    }
    
    OLD_RW = RW;
    
  }
  
  if ((RW == 'r') && (OLD_RW != 'r')) {
    
    for (int i = 0; i < 8; i++) {  
      
      pinMode(DATA_PIN[i], OUTPUT);
      
    }
    
    OLD_RW = RW;
    
  }
  
}

void decoding(byte gpbyte) { // OK
  
  boolean gpbit = 0;
  
  for (int i = 7; i >= 0; i--) { 
    
    gpbit = bitRead(gpbyte, i);
    
    if (gpbit == 0) {  
               
      digitalWrite(DATA_PIN[7-i],LOW);
      
    } else {  
      
      digitalWrite(DATA_PIN[7-i],HIGH);
      
    } 
       
  }

  hexData = gpbyte;
  
}

void RW_mem(unsigned int address, unsigned int reference, byte memblock[]) {

int offset = address - reference;
int bit = 0;
unsigned int data = 0;

  if( RW == 'W' ) {

    for(int i = 0; i < DATAN; i++) {
      
      bit = digitalRead(DATA_PIN[i]) ? 1 : 0;
      data = (data << 1) + bit;

    }

  memblock[offset] = data;

  } else if ( RW == 'r' ) {

    decoding(memblock[offset]);
    
  }
  
}

void IN_OUT(unsigned int address) { // OK

int bit = 0;

if( RW == 'W' ) {

  switch(address) {

    case 0x6000:
      for(int i = 0; i < DATAN; i++) {

        if ( bitRead(DATADRB, (7-i)) == 1 ) {
          
          OPB[7-i] = digitalRead(DATA_PIN[i]) ? 1 : 0;
        
        }
      
      }
      break;
    case 0x6001:
      for(int i = 0; i < DATAN; i++) {

        if ( bitRead(DATADRA, (7-i)) == 1 ) {
          
          OPA[7-i] = digitalRead(DATA_PIN[i]) ? 1 : 0;
        
        }
      
      }
      break;
    case 0x6002: // W
      for(int i = 0; i < DATAN; i++) {
      
        bit = digitalRead(DATA_PIN[i]) ? 1 : 0;
        DATADRB = (DATADRB << 1) + bit;
      
      }
      break;
    case 0x6003: // W
      for(int i = 0; i < DATAN; i++) {
      
        bit = digitalRead(DATA_PIN[i]) ? 1 : 0;
        DATADRA = (DATADRA << 1) + bit;
      
      }
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

    statusBit = 0;

    char output[15];
    unsigned int address = 0;
    int bit = 0;

    check_RW();
  
    for(int i = 0; i < ADDRN; i++) {

      bit = digitalRead(ADDR_PIN[i]) ? 1 : 0;
      address = (address << 1) + bit;
    
    }

    check_RESET(address);

    if ((address >= 0x0000) && (address <= 0x00FF)) {

      RW_mem(address, 0x0000, zeropage);
    
    } else if ((address >= 0x0100) && (address <= 0x01FF)) {

      RW_mem(address, 0x0100, stack);
    
    } else if ((address >= 0x0200) && (address <= 0x5FFF)) {
    
      RW_mem(address, 0x0200, freeram);
    
    } else if ((address >= 0x6000) && (address <= 0x7FFF)) {

      IN_OUT(address);
    
    } else if ((address >= 0x8000) && (address < 0x8000+DIMPROG)) {
    
      int offset = address - 0x8000;
      decoding(PROG[offset]);
    
    }

    if ( RW == 'W' ) {

      for(int i = 0; i < DATAN; i++) {
      
        bit = digitalRead(DATA_PIN[i]) ? 1 : 0;
        hexData = (hexData << 1) + bit;
      
      }
    
    }

    sprintf(output, "%04x %c %02x", address, RW, hexData);
    Serial.println(output);
    hexData = 0;

  }

}
