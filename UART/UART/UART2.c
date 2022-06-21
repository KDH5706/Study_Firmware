/*
 * UART2.c
 *
 * Created: 2022-06-21 오전 11:17:55
 *  Author: PKNU
 */ 

#include <avr/io.h> // AVR 입출력에 대한 헤더 파일

void putch(unsigned char data)
{
	while((UCSR0A & 0x20) == 0); // 전송준비가 될때까지 대기
	UDR0 = data; // 데이터를 UDR0에 쓰면 전송
	UCSR0A |= 0x20;
}

unsigned char getch()
{
	unsigned char data;
	while ((UCSR0A & 0x80) == 0);
	data = UDR0;
	UCSR0A |= 0x80;
	return data;
}

void Init()
{
	DDRE = 0xfe;
	UCSR0A = 0x00;
	UCSR0B = 0x18;
	UCSR0C = 0x06;
	UBRR0H = 0x00;
	UBRR0L = 3;
	DDRA = 0xff;
}
int main()
{
	Init();

	unsigned char text[] =
	"\r\nWelcome! edgeiLAB\r\n USART 0 Test Program.\r\n";

	unsigned char echo[] = "ECHO >> ";
	unsigned char i = 0;
	
	while (text[i] != '\0')
	{
		putch(text[i++]);
	}
	i = 0;
	while (echo[i] != '\0')
	{
		putch(echo[i++]);
	}
	while (1)
	{
		putch(getch());
	}
}