#ifndef FACE_RECOGNITION_H
#define FACE_RECOGNITION_H


#include <stdint.h>

void face_recognition_init(void);
void face_recognition_send_command(uint8_t *command, uint8_t command_len);
extern uint8_t cmd[8];
void rcu_config(void);
void gpio_config(void);
void nvic_config(void);
void usart_config(void);
void init_face_recognition(void);
void send_serial_cmd(uint8_t* command, uint8_t length);

#endif /* FACE_RECOGNITION_H */
