/*
* UART.c
*
* Created: 2022-06-21 오전 9:35:37
* Author : PKNU
*/

#include <avr/io.h> // AVR 입출력에 대한 헤더 파일
void putch(unsigned char data)
{
	while((UCSR0A & 0x20) == 0); // 전송준비가 될때까지 대기
	UDR0 = data; // 데이터를 UDR0에 쓰면 전송
	UCSR0A |= 0x20;
}

int main(void)
{
	unsigned char text[] = "Hello! World!! \r\n";
	unsigned char i=0;
	DDRE = 0xfe; // Rx(입력 0), Tx(출력, 1)

	UCSR0A = 0x00;
	UCSR0B = 0x18; // Rx, Tx enable
	UCSR0C = 0x06; // 비동기 방식, No Parity bit, 1 Stop bit
	UBRR0H = 0x00;
	UBRR0L = 3; // 115200 bps

	while(text[i]!='\0') // 문자열 데이터가 '\0'인 경우, 문자열 끝임
	{
		putch(text[i++]); // 저장된 문자열 출력
	}
}
