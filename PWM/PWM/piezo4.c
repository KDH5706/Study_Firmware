/*
 * piezo4.c
 *
 * Created: 2022-06-23 오후 12:01:34
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
	ICR1 = 7372800 / DoReMi[num];
	OCR1C = ICR1/50;
	_delay_ms(100); // 0.1초 지연
	OCR1C = 0;		//이전 OVF에서 High 되었다가, 그 다음 클럭에서 OCR = 0이므로 바로 compare match가 발생해서 Low가 됨
}

void Init()
{
	DDRE |= (1 << DDE1);	//TXD0 : 1, RXD0 : 0
	//UCSR0A = 0x00;
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);		// Receiver Enable, Transmitter Enable
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);	// Character Size : 8-bit
	//UBRR0H = 0x00;
	UBRR0L = 3;		//f : 7.3728MHz이고, Baud Rate : 115200일 때
	
	DDRB |= (1 << DDB7); // 포트B PB7을 출력 나머지는 입력포트로 설정한다.
	
	TCCR1A |= (1 << WGM11);		// Fast PWM 모드 설정, TOP : ICR1
	TCCR1B |= (1 << WGM12);		// Fast PWM 모드 설정, TOP : ICR1
	TCCR1B |= (1 << WGM13);		// Fast PWM	모드 설정, TOP : ICR1
	TCCR1A |= (1 << COM1C1);	// Clear OCnA/OCnB/OCnC on compare match, set OCnA/OCnB/OCnC at BOTTOM, (non-inverting mode)
	TCCR1B |= (1 << CS10);		// clkI/O/1 (No prescaling)
	//TCCR1C = 0x00;
	TCNT1 = 0;
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

