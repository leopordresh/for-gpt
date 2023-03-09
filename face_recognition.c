#include "face_recognition.h"
#include "gd32f10x_usart.h"
#include "gd32f10x_rcu.h"
uint8_t cmd[8]= {0xEF, 0xAA, 0x12, 0x00, 0x00, 0x00, 0x00, 0x12};
void rcu_config(void) {
    // ...
}

void gpio_config(void) {
    // ...
}

void nvic_config(void) {
    // ...
}

void usart_config(void) {
    // ...
}
void face_recognition_init() {
    // ?????????
    // ...
	  rcu_config();
    gpio_config();
    nvic_config();
    usart_config();
    
}

void face_recognition_send_command(uint8_t *cmd, uint8_t cmd_len) {
    // ???????????
    // ...
	 uint8_t i;

    for(i = 0; i < cmd_len; i++) {
        while(usart_flag_get(USART2, USART_FLAG_TC) == RESET);
        usart_data_transmit(USART2, cmd[i]);
    }
}

// ...

//int main(void) {
//    // ?????????
//    face_recognition_init();

//    // ???????????
// 
//    face_recognition_send_command(cmd, sizeof(cmd));

//    // ???
//    while(1);

//    return 0;
//}
