/*
* input1.c
*
* Created: 2022-06-17 오후 3:20:19
*  Author: PKNU
*/
#define F_CPU 7432800UL
#include <avr/io.h>
#include <util/delay.h>

int main()
{
	DDRA = 0xFF;	// PRORTA를 출력으로 설정
	DDRD = 0x00;	// PRORTB를 입력으로 설정
	
	while (1)
	{
		int pin_status = PIND;
		PORTA = pin_status;
	}
	
	return 0;
}
