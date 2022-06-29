/*
* Project.c
*
* Created: 2022-06-24 오전 9:44:43
* Author : PKNU
*/
#define F_CPU 7372800UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "Lcd.h"
#define CHANGE_MODE 128

//PassWord
volatile unsigned char PASS_WORD[5] = "2345";
volatile unsigned char input_pw[5] = "";

//피아노 음계에 해당하는 PWM 주파수
unsigned int DoReMi[] = {523, 587, 659, 698, 783, 880, 987, 1046, 1174, 1318};

//스텝모터 제어
unsigned char mot_cnt = 0;
unsigned char Step[8] = {0x90,0x80,0xC0,0x40,0x60,0x20,0x30,0x10};

//Mode 결정 변수(Mode_flag, true : KeyPad/false : UART) 및 Mode 변환 여부 체크 변수(swap)
volatile bool Mode_flag = true, swap = false;

void UART_Init()
{
	DDRE |= (1 << DDE1);	//TXD0 : 1(출력), RXD0 : 0(입력)
	//UCSR0A = 0x00;
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);		// Receiver Enable, Transmitter Enable
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);	// Character Size : 8-bit
	//UBRR0H = 0x00;
	UBRR0L = 3;		//f : 7.3728MHz이고, Baud Rate : 115200일 때
}

void putch_USART0(unsigned char data)
{
	//UDR0이라는 송신 버퍼가 비어 있으면 UCSR0A에 존재하는 UDRE0 비트 값이 1로 Set 됨
	while((UCSR0A & (1 << UDRE0)) == 0); // 전송 준비가 될때까지 대기
	UDR0 = data; // 데이터를 UDR0(송신 버퍼)에 할당하면 전송된다.
	UCSR0A |= 1 << UDRE0;	//UDRE0 값 1로 초기화(전송이 끝났기 때문에 Initial Value 값으로 Set)
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
	//UDR0이라는 수신 버퍼에 수신할 문자가 존재하면 UCSR0A에 존재하는 RXC0 비트 값이 1로 Set 됨
	while ((UCSR0A & (1 << RXC0)) == 0)
	{
		// 인터럽트에 의해 Mode_flag 값이 true가 되면 return
		if(Mode_flag == true) return CHANGE_MODE;	
	}
	data = UDR0;	// UDR0(수신 버퍼)의 값을 데이터에 할당한다.
	UCSR0A |= 1 << RXC0;	//UCSR0A &= ~(1 << RXC0); <- 이 경우는 RXC0 비트를 0(Initial Value)으로 클리어 해준다는 의미
	return data;
}

void LCD_Init()
{
	DDRA = 0xFF;
	DDRG = 0xFF;
	
	Lcd_Init();
	Lcd_Clear();
	_delay_ms(1000);
}

void Keypad_Init()
{
	DDRF = 0xF8; //0b 1111 1000
	PORTF = 0x07; //0b 0000 0111
}

unsigned char keyscan()
{
	PORTF = 0x08;	//0b 0000 1000
	_delay_ms(1);
	if(((PINF >> 0) & 0x01) == 1) return '1';
	if(((PINF >> 1) & 0x01) == 1) return '2';
	if(((PINF >> 2) & 0x01) == 1) return '3';
	_delay_ms(10);
	
	PORTF = 0x10;	//0b 0001 0000
	_delay_ms(1);
	if(((PINF >> 0) & 0x01) == 1) return '4';
	if(((PINF >> 1) & 0x01) == 1) return '5';
	if(((PINF >> 2) & 0x01) == 1) return '6';
	_delay_ms(10);
	
	PORTF = 0x20;	//0b 0010 0000
	_delay_ms(1);
	if(((PINF >> 0) & 0x01) == 1) return '7';
	if(((PINF >> 1) & 0x01) == 1) return '8';
	if(((PINF >> 2) & 0x01) == 1) return '9';
	_delay_ms(10);
	
	PORTF = 0x40;	//0b 0100 0000
	_delay_ms(1);
	if(((PINF >> 0) & 0x01) == 1) return '*';
	if(((PINF >> 1) & 0x01) == 1) return '0';
	if(((PINF >> 2) & 0x01) == 1) return '#';
	_delay_ms(10);
	
	return 0;
}

void StepMotor_Init()
{
	DDRC = 0xF0;
}

void Mode_Interrupt_Init()
{
	DDRD = 0x00;		// INT0 핀을 사용하기 위해 D포트를 입력모드로 설정
	EIMSK = (1 << INT0);// INT0 interrupt enabled
	EICRA = (1 << ISC01) | (1 << ISC00);	//rising edge
	sei();
}

ISR(INT0_vect)
{
	cli();
	Mode_flag = !Mode_flag;
	sei();
}

void LCD_INFO()
{
	Lcd_Pos(0,0);
	Lcd_STR((unsigned char*)"Door lock system");
	Lcd_Pos(1,0);
	Lcd_STR((unsigned char*)"Insert PW: ");
}

void Piezo_Init()
{
	DDRB |= (1 << DDB7);					// 포트B PB7(OC1C)을 출력 나머지는 입력포트로 설정
	
	//TCCR1A = 0x00;						// WGM1(1:0) = "00" WGM(3:0) = "1100" CTC 모드
	TCCR1B |= (1 << WGM32) | (1 << WGM33);	// WGM1(3:2) = "11" WGM(3:0) = "1100" CTC 모드
	TCCR1B |= (1 << CS10);					// CS1(2:0) = "001" clkI/O/1 (No prescaling)
	//TCCR1C = 0x00;
	TCNT1 = 0;
}

void Beep_Melody()
{
	ICR1 = 7372800 / DoReMi[0];
	//TCCR1A : Timer/Counter1 Control Register A
	TCCR1A = (1 << COM1C0);	// Toggle OC1C(PB7) on compare match.(단, non-PWM 모드일 경우)
	_delay_ms(100); // 0.1초 지연
	TCCR1A = 0x00;	// 출력 종료
}

void Make_Melody(int input)
{
	ICR1 = 7372800 / DoReMi[input];
	TCCR1A = (1 << COM1C0);	// Toggle OC1C(PB7) on compare match.(단, non-PWM 모드일 경우)
	_delay_ms(100); // 0.1초 지연
	TCCR1A = 0x00;	// 출력 종료
}

void Success_Melody()
{
	for (int i = 3; i < 10 ; i++)
	{
		Make_Melody(i);
		_delay_ms(20);
	}
}
				   
void Failure_Melody()
{
	for (int i = 0; i < 8 ; i++)
	{
		if(i % 2 == 1) Make_Melody(4);
		else Make_Melody(7);
	}
}

void Open_Door()
{
	// 2 * 500 = 1s 동안 스텝모터 작동
	for (int i = 0 ; i < 500 ; i++)
	{
		PORTC = Step[mot_cnt];
		if(mot_cnt++ == 7) mot_cnt = 0;
		_delay_ms(2);
	}
}

void Save_Input_num(unsigned char temp, int input_pw_cnt)
{
	Byte pw_state[16] = "Insert PW: ";

	//입력된 비밀번호 배열에 저장
	input_pw[input_pw_cnt] = temp;
	
	//비밀번호 마스킹 문구 설정
	switch (input_pw_cnt)
	{
		case 0:
		sprintf((char*)pw_state, "Insert PW: *");
		break;
		case 1:
		sprintf((char*)pw_state, "Insert PW: **");
		break;
		case 2:
		sprintf((char*)pw_state, "Insert PW: ***");
		break;
		case 3:
		sprintf((char*)pw_state, "Insert PW: ****");
		break;
	}
	
	Lcd_Pos(0,0);
	Lcd_STR((unsigned char*)"Door lock system");
	Lcd_Pos(1, 0);
	Lcd_STR((unsigned char*)pw_state);
}

void Init()
{
	UART_Init();
	Keypad_Init();
	Piezo_Init();
	StepMotor_Init();
	Mode_Interrupt_Init();
	LCD_Init();
}

void KeyPad_Mode()
{
	int input_pw_cnt = 0;
	unsigned char temp = 0;
	// 비밀번호 4자리 입력 받음
	while (input_pw_cnt < 4)
	{
		// 모드전환 체크
		if(Mode_flag == false)
		{
			// input_pw 초기화
			for(int i = 0; i < 4; i++)
			input_pw[i] = 0;
			// swap 활성화
			swap = true;
			break;
		}
		
		// 채터링 방지 코드
		unsigned char pre_temp = 0;
		unsigned char cur_temp = 0;
		cur_temp = keyscan();
		if(cur_temp != 0 && pre_temp == 0)
		{
			// temp 정의
			temp = cur_temp;
		}
		pre_temp = cur_temp;

		// 입력값 저장
		if( temp != 0)
		{
			//부저 비프음 출력
			Beep_Melody();
			Save_Input_num(temp, input_pw_cnt);
			temp = 0;
			input_pw_cnt++;
		}
	}
	_delay_ms(300);
}

void UART_Mode()
{
	int input_pw_cnt = 0;
	unsigned char temp = 0;
	
	unsigned char info_msg[] = "\r\nDoor lock system\r\nInsert PW: ";
	puts_USART0(info_msg);
	
	// 비밀번호 4자리 입력 받음
	while (input_pw_cnt < 4)
	{
		// temp 정의
		temp = getch();
		// 모드전환 체크
		if(Mode_flag == true)
		{
			// input_pw 초기화
			for(int i = 0; i < 4; i++)
			input_pw[i] = 0;
			// swap 활성화
			swap = true;
			break;
		}
		
		//부저 비프음 출력
		Beep_Melody();
		putch_USART0('*');
		
		// 입력값 저장
		if( temp != 0)
		{
			Save_Input_num(temp, input_pw_cnt);
			temp = 0;
			input_pw_cnt++;
		}
	}
	_delay_ms(300);
}

void Input_Result()
{
	Lcd_Clear();
	Lcd_Pos(0,0);
	// 비밀번호가 같은지 판단하여 LCD창에 결과 출력
	if(strcmp((char*)input_pw, (char*)PASS_WORD) == 0)
	{
		Lcd_STR((unsigned char*)"OPEN DOOR");
		Success_Melody();
		Open_Door();
	}
	else
	{
		Lcd_STR((unsigned char*)"Retry!");
		Failure_Melody();
	}
	_delay_ms(800);
}

int main(void)
{
	Init();
	
	while (1)
	{
		// LCD 초기 세팅
		LCD_INFO();

		// 모드 설정
		if(Mode_flag) // 키패드 모드
			KeyPad_Mode();
		else  //UART 모드
			UART_Mode();
		
		//KeyPad <-> UART 전환해야 하는지 판단
		if(swap)
		{
			swap = false;
			continue;
		}

		// 입력한 비밀번호를 판단하여 결과 결정
		Input_Result();
	}
}

