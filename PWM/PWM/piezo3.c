/*
 * piezo3.c
 *
 * Created: 2022-06-23 오후 4:14:51
 * Author : PKNU
 */ 
#define F_CPU 7372800UL
#include <stdio.h>
#include <avr/io.h> // AVR 입출력에 대한 헤더 파일
#include <avr/interrupt.h> // AVR 인터럽트에 대한 헤더파일
#include <util/delay.h> // delay 함수사용을 위한 헤더파일

//피아노 음계에 해당하는 PWM 주파수
unsigned int DoReMi[] = {523, 587, 659, 698, 783, 880, 987, 1046, 1174, 1318};

void putch_USART0(unsigned char data)
{
	while((UCSR0A & 0x20) == 0); // 전송준비가 될때까지 대기
	UDR0 = data; // 데이터를 UDR0에 쓰면 전송
	UCSR0A |= 0x20;
}

void puts_USART0(unsigned char *str)
{
	while (*str != 0)
	{
		putch_USART0(*str);
		str++;
	}
}

unsigned char getch()
{
	unsigned char data;
	while ((UCSR0A & 0x80) == 0);
	data = UDR0;
	UCSR0A |= 0x80;
	return data;
}

void Show_Input_Value(unsigned char input)
{
	unsigned char str[15] = "";
	sprintf((char*)str, "INPUT : %c\n\r", input);
	puts_USART0((unsigned char*)str);
}

void Make_Melody(unsigned char input)
{
	int num = input - '0';
	ICR3 = 7372800/DoReMi[num]/5;
	TCCR3A |= (1 << COM3A0); // COM3A(1:0) = "01" Toggle OCnA/OCnB/OCnC on compare match, OC3A(= PE3)
	_delay_ms(100); // 0.1초 지연
	TCCR3A = 0x00; // 부저 소리를 끈다.
}

void Init()
{
	DDRE |= (1 << DDE1);	//TXD0 : 1, RXD0 : 0
	//UCSR0A = 0x00;
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);		// Receiver Enable, Transmitter Enable
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);	// Character Size : 8-bit
	//UBRR0H = 0x00;
	UBRR0L = 3;		//f : 7.3728MHz이고, Baud Rate : 115200일 때
	
	DDRE |= (1 << DDE3); // 포트E PE3를 출력 나머지는 입력포트로 설정한다.
	
	//TCCR3A = 0x00; // WGM3(1:0) = “00”
	TCCR3B |= (1 << WGM33) | (1 << WGM32);// WGM3(3:2) = “11”
	TCCR3B |= (1 << CS30); // CS3(2:0) = “001” 1분주 사용
	TCCR3C = 0x00; // WGM3(3:0) = “1100”, CTC, 모드 12
	
	TCNT3 = 0; // 타이머3 카운터 초기화
}

int main(void)
{
	Init();
	
	unsigned char msg[] =
	"\r\nPlease enter a number button.(0 ~ 9)\r\n";

	puts_USART0(msg);
	
	while(1)
	{
		//출력할 주파수 값을 입력 받음
		unsigned char input = getch();
		Show_Input_Value(input);
		if((input >= '0') && (input <= '9'))
		{
			Make_Melody(input);
		}
		else
		{
			puts_USART0(msg);
		}
	}
}

