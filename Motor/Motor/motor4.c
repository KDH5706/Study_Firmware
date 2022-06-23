/*
 * motor4.c
 *
 * Created: 2022-06-23 오후 3:34:39
 *  Author: PKNU
 */ 
#define F_CPU 7372800UL
#include <avr/io.h>
#include <util/delay.h> // delay 함수사용을 위한 헤더파일
#include <avr/interrupt.h>
#define DIR_L 0
#define DIR_R 1

unsigned char timer0Cnt = 0, mot_cnt = 0;
volatile unsigned char dir = DIR_R; 
unsigned char Step[8] = {0x90,0x80,0xC0,0x40,0x60,0x20,0x30,0x10};

ISR(TIMER0_OVF_vect)
{
	cli();
	TCNT0 = 184; // 256-72=184 -> 0.01초 마다 한번씩 인터럽트 발생
	timer0Cnt++; // timer0Cnt 변수를 1 증가
	if(timer0Cnt == 200) // 0.01s * 200 = 2s
	// 1초를 얻기 위한 카운트 횟수
	{
		dir^=1; // 방향 전환
		timer0Cnt=0; // timer0Cnt 카운터 초기화
	}
	sei();}
ISR(TIMER1_OVF_vect)
{
	cli();
	TCNT1 = 7936; // 65536-28800-28800 = 7936
	PORTD = Step[mot_cnt]; // 1-2상 여자 방식 한 스텝 진행
	if(dir==DIR_R) // 회전 방향이 우측 방향이면
	{
		if(mot_cnt++==7) mot_cnt=0; // 스텝 카운터 증가
	}
	else if(mot_cnt--==0) mot_cnt=7; // 스텝 카운터 감소
	sei();}

void Init()
{
	//DDRB = 0x20; // MOTOR1_EN 를 출력 포트로 설정
	//PORTB &= ~0x20; // M1 Disable, DC 모터 정지
	
	DDRD = 0xF0; // STEP0 ~ STEP3을 출력 포트로 설정
	
	TCCR0 = 0x07;	// 분주비 : 1024
	TCNT0 = 184; // 256-72=184 -> 0.01초 마다 한번씩 인터럽트 발생
	
	//TCCR1A = 0;
	TCCR1B = 0x04;	// 분주비 : 256
	TCNT1 = 7936; // 65536-28800-28800 = 7936
	TIMSK = 0x05; // TOIE1, TOIE0 모두 '1'
	TIFR = 0x05; // TOV1, TOV0 모두 '1'
	sei();}

int main(void) {
	
	Init();

	while (1){ };
	
	return 0;
}
