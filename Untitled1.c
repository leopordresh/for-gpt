#include <stdio.h>
#include "stm32f10x.h"
#include "uart.h"

#define CMD_START       0xAA
#define CMD_STOP        0xAB
#define CMD_GET_IMG     0x01
#define CMD_RECOG       0x07
#define CMD_DEL         0x04
#define CMD_CLEAR       0x05
#define CMD_GET_ENROLL  0x0A

#define UART_RX_BUF_SIZE  512

volatile uint8_t uart_rx_buf[UART_RX_BUF_SIZE];
volatile uint16_t uart_rx_buf_head = 0;
volatile uint16_t uart_rx_buf_tail = 0;

void uart1_init(void)
{
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Enable USART1 and GPIOA clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
  
  /* Configure USART1 Tx (PA.09) and Rx (PA.10) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* USART1 configuration */
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART1, &USART_InitStructure);
  
  /* NVIC configuration */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  USART_Cmd(USART1, ENABLE);
  
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}

void uart1_send_byte(uint8_t byte)
{
  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
  USART_SendData(USART1, byte);
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
}

void uart1_send_cmd(uint8_t cmd, uint8_t *data, uint16_t len)
{
  uint8_t sum = cmd + len;
  uart1_send_byte(CMD_START);
  uart1_send_byte(cmd);
  uart1_send_byte(len & 0xFF);
  uart1_send_byte((len >> 8) & 0xFF);
  for (int i = 0; i < len; i++)
  {
    uart1_send_byte(data[i]);
    sum += data[i];
  }
  uart1_send_byte(sum);
}

void uart1_receive_isr(void)
{
  uint8_t byte;
  if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  {
    byte = USART_ReceiveData(USART1);
    uart_rx_buf[uart_rx
