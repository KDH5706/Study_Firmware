/*
* led1.c
*
* Created: 2022-06-17 오전 11:46:05
*  Author: PKNU
*/
#define F_CPU 7372800
#include <avr/io.h>
#include <util/delay.h>

int main()
{
	DDRA = 0xFF;	// PRORTA를 출력으로 설정
	
	while (1)
	{
		PORTA = 0xff;	//0b 1111 1111
		_delay_ms(10000);
		
		PORTA = 0x00;	//0b 0000 0000
		_delay_ms(10000);
	}
	
	return 0;
}