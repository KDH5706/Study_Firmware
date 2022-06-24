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
#define CHANGE_MODE 1

//PW
volatile unsigned char PASS_WORD[5] = "2345";
volatile unsigned char input_pw[5] = "";

//피아노 음계에 해당하는 PWM 주파수
unsigned int DoReMi[] = {523, 587, 659, 698, 783, 880, 987, 1046, 1174, 1318};

//스텝모터 제어
unsigned char mot_cnt = 0;
unsigned char Step[8] = {0x90,0x80,0xC0,0x40,0x60,0x20,0x30,0x10};

//Mode 결정 변수(Mode_flag) 및 Mode 변환 여부 체크 변수(swap)
volatile bool Mode_flag = true, swap = false;

void UART_Init()
{
	DDRE |= (1 << DDE1);	//TXD0 : 1, TXD0 : 0
	//UCSR0A = 0x00;
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);		// Receiver Enable, Transmitter Enable
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);	// Character Size : 8-bit
	//UBRR0H = 0x00;
	UBRR0L = 3;		//f : 7.3728MHz이고, Baud Rate : 115200일 때
}

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
	while ((UCSR0A & 0x80) == 0)
	{
		if(Mode_flag == true) return CHANGE_MODE;	
	}
	data = UDR0;
	UCSR0A |= 0x80;
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

void Piezo_Init()
{
	DDRB |= (1 << DDB7);		// 포트B PB7을 출력 나머지는 입력포트로 설정한다.
	
	TCCR1A |= (1 << WGM11);		// Fast PWM 모드 설정, TOP : ICR1
	TCCR1B |= (1 << WGM12);		// Fast PWM 모드 설정, TOP : ICR1
	TCCR1B |= (1 << WGM13);		// Fast PWM	모드 설정, TOP : ICR1
	TCCR1A |= (1 << COM1C1);	// Clear OCnA/OCnB/OCnC on compare match, set OCnA/OCnB/OCnC at BOTTOM, (non-inverting mode)
	TCCR1B |= (1 << CS10);		// clkI/O/1 (No prescaling)
	//TCCR1C = 0x00;
	TCNT1 = 0;
}

void StepMotor_Init()
{
	DDRC = 0xF0;
}

void Mode_Interrupt_Init()
{
	DDRD = 0x00;
	EIMSK = (1 << INT0);
	EICRA = (1 << ISC01) | (1 << ISC00);
	sei();
}

ISR(INT0_vect)
{
	Mode_flag = 1 - Mode_flag;
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

void LCD_INFO()
{
	Byte Info_str[16] = "";
	sprintf((char*)Info_str, "Door lock system");
	Lcd_Pos(0,0);
	Lcd_STR((unsigned char*) Info_str);
	sprintf((char*)Info_str, "Insert PW: ");
	Lcd_Pos(1,0);
	Lcd_STR((unsigned char*) Info_str);
	
	//_delay_ms(500);
	//Lcd_Clear();
}

void Beep_Melody()
{
	ICR1 = 7372800 / DoReMi[0];
	OCR1C = ICR1/50;
	_delay_ms(100); // 0.1초 지연
	OCR1C = 0;
}

void Melody_Init()
{
	ICR1 = 65535;
	OCR1C = ICR1/50;
	_delay_ms(100); // 0.1초 지연
	OCR1C = 0;
}

void Make_Melody(int input)
{
	ICR1 = 7372800 / DoReMi[input];
	OCR1C = ICR1/50;
	_delay_ms(100); // 0.1초 지연
	OCR1C = 0;		//이전 OVF에서 High 되었다가, 그 다음 클럭에서 OCR = 0이므로 바로 compare match가 발생해서 Low가 됨
}

void Success_Melody()
{
	for (int i = 0; i < 10 ; i++)
	{
		Make_Melody(i);
	}
}
				   
void Failure_Melody()
{
	for (int i = 0; i < 10 ; i++)
	{
		if(i%2 == 1) Make_Melody(10);
		else Make_Melody(5);
	}
}

void Open_Door()
{
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

int main(void)
{
	Init();
	Melody_Init();
	
	while (1)
	{
		// LCD 초기 세팅
		LCD_INFO();
		
		int input_pw_cnt = 0;
		unsigned char temp = 0;

		if(Mode_flag) // 키패드 모드
		{
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
		}
		else  //UART 모드
		{
			unsigned char info_msg[] = "\r\nDoor lock system\r\nInsert PW: ";
			puts_USART0(info_msg);
			
			// 비밀번호 4자리 입력 받음
			while (input_pw_cnt < 4)
			{
				// temp 정의
				unsigned char temp = getch();
				// 모드전환 체크
				if(temp == CHANGE_MODE)
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
		}
		
		//KeyPad <-> UART 전환해야 하는지 판단
		if(swap)
		{
			swap = false;
			continue;
		}
		
		_delay_ms(300);
		input_pw_cnt = 0;
		
		// 비밀번호가 같은지 판단하여 LCD창에 결과 출력
		if(strcmp((char*)input_pw, (char*)PASS_WORD) == 0)
		{
			Lcd_Clear();
			Lcd_Pos(0,0);
			Lcd_STR((unsigned char*)"OPEN DOOR");
			Success_Melody();
			Open_Door();
			_delay_ms(800);
		}
		else
		{
			Lcd_Clear();
			Lcd_Pos(0,0);
			Lcd_STR((unsigned char*)"Retry!");
			Failure_Melody();
			_delay_ms(800);
		}
	}
}

