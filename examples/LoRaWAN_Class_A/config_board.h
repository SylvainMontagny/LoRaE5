#ifndef CONFIG_BOARD_H
#define CONFIG_BOARD_H

//////////////////////////////////////////////
// If you use an ARDUINO or ESP32 board 
//////////////////////////////////////////////
  #if defined(ARDUINO_ARCH_AVR) || defined(ARDUINO_ARCH_SAM) || defined(ARDUINO_ARCH_SAMD) || defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_ARCH_RENESAS) || defined(ARDUINO_WIO_TERMINAL)
    #define Debug_Serial      Serial    // Select the right SerialX for Debug

    // Arduino's Serial Documentation : https://www.arduino.cc/reference/en/language/functions/communication/serial/
    // ESP32 Documentation : https://microcontrollerslab.com/esp32-uart-communication-pins-example/
    #define LoRa_Serial       Serial1   // Select the right SerialX for the LoRaE5 connection

//////////////////////////////////////////////
// If you use a STM32 NUCLEO board 
//////////////////////////////////////////////
  #elif defined(ARDUINO_ARCH_STM32)
    #define RX_PIN_DEBUG      PA3     // Select UART RX Pin for Debug
    #define TX_PIN_DEBUG      PA2     // Select UART TX Pin for Debug
    HardwareSerial Debug_Serial(RX_PIN_DEBUG, TX_PIN_DEBUG);


    #define RX_PIN_LORA       PA10  // Select UART RX Pin for the LoRaE5 connection
    #define TX_PIN_LORA       PA9   // Select UART TX Pin for the LoRaE5 connection
    HardwareSerial LoRa_Serial(RX_PIN_LORA, TX_PIN_LORA);


//////////////////////////////////////////////
// /!\ Your card has never been tested yet !
// Please let us know if you find the PIN configuration of your board
// So we can add it to the next version of this code
//////////////////////////////////////////////

  #else
  #warning "Your board has never been tested with this lib"
  #warning "Select the right Serial for Debug and LoRaE5 connection in config_board.h"

  #define Debug_Serial      Serial
  #define LoRa_Serial       Serial1

  #endif

#endif //CONFIG_BOARD_H

////////////////////////////////
/* Uart Pin list for known board
////////////////////////////////

NUCLEO-F446RE : 
USART1_RX = PA10 Alternate PB7
USART1_TX = PA9 Alternate PB6
 
USART2_RX = PA3 (ST-link)
USART2_TX = PA2 (ST-Link)

USART3_RX = PC5 Alternate PC11
USART3_TX = PB10 Alternate PC10

//

NUCLEO-L073RZ :
USART1_RX = PA10 Alternate PB7
USART1_TX = PA9 Alternate PB6

USART2_RX = PA3 (ST-link) Alternate PA15
USART2_TX = PA2 (ST-Link) Alternate PA14

//

NUCLEO-F411RE :
USART1_RX = PA10 Alternate PB7 Alternate PB3
USART1_TX = PA9 Alternate PB6 Alternate PA15
 
USART2_RX = PA3 (ST-link)
USART2_TX = PA2 (ST-Link)

//

NUCLEO-WL55JC1 :
USART1_RX = PA10 Alternate PB7
USART1_TX = PA9 Alternate PB6
 
USART2_RX = PA3 (ST-link)
USART2_TX = PA2 (ST-Link)

//

NUCLEO-G031K8 :
USART1_RX = PA10 Alternate PB7
USART1_TX = PA9 Alternate PB6
 
USART2_RX = PA3 (ST-link)
USART2_TX = PA2 (ST-Link)

//

NUCLEO-L433RC-P  :
USART1_RX = PA10 Alternate PB7
USART1_TX = PA9 Alternate PB6
 
USART2_RX = PA3 (ST-link) Alternate PA15
USART2_TX = PA2 (ST-Link)

//

NUCLEO-F746ZG :
USART1_RX = PA10 Alternate PB7
USART1_TX = PA9 Alternate PB6
 
USART2_RX = PA3 (ST-link) Alternate PD6
USART2_TX = PA2 (ST-Link) Alternate PD5

USART3_RX = PD9 Alternate PB11 Alternate PC11
USART3_TX = PD8 Alternate PB10 Alternate PC10

////////////////////////////////
Uart Pin list for known board */
////////////////////////////////