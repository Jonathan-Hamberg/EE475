#include <xc.h>        /* XC8 General Include File */
#include "mothership.h"
#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */
#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/eusart1.h"

//#define PARITY_BIT RCSTA1bits.RX9D
//
//uint8_t getOddParity(uint8_t byte) {
//      byte ^= (byte >> 4);  
//      byte ^= (byte >> 2); 
//      byte ^= (byte >> 1);  
//      return byte & 1; 
//}

//void RS232_WriteNData(uint8_t *data, uint8_t size) {
//    for(int byte = 0; byte < size; byte++) {
//        PARITY_BIT = getOddParity(*data);
//        EUSART1_Write(*data);
//        data++; 
//    }
//}
//
//uint8_t RS232_ReadData(void) {
//    while(EUSART1_DataReady);
//    return EUSART1_Read();
//}

 uint8_t curr = NULL;
 uint8_t prev = NULL; 
  uint8_t parity;
  uint8_t temp;
  

  
/**
 * 
 */
void main(void) {
    
    SYSTEM_Initialize();
    
//    uint8_t temp;
//    while(1) {
////        while(EUSART1_DataReady) {
////            temp = EUSART1_Read();
////        };
//        temp = getOddParity(0x01);
//        temp = (temp << 8) | 0x01;
//        EUSART1_Write(temp);
//    }
   

//    while(1) {
//        parity = getOddParity('a');
//        temp = 'a' | (parity << 7);
//        EUSART1_Write(temp);
//    }
//    RS232_WriteString("Welcome to the Space Explorer Earth Station");
//    RS232_WriteString("To launch the probe from the mothership, press 's'");
//    
//    while(1) {
//        
//        if(EUSART1_DataReady) {
//            curr = EUSART1_Read(); 
//        }
//        else {
//            EUSART1_Write(curr);
//        }
//    }
    
//    // Initiallze the mothership system
//    mothership_setup();
//    
//    // Run mothership system loop
//    while(1) {
//        mothership_loop();
//    }
    
    //Initialize the mothership system
    mothership_setup();
    
    while(1) {
        mothership_loop();
    }
}

