/*
* led2.c
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
		int flag = 0x01;	//0b00000001
		for (int i = 0 ; i < 7 ; i++)
		{
			PORTA = ~(flag << i);
			_delay_ms(150);
		}
		flag = 0x80;	//0b10000000
		for (int i = 0 ; i < 7 ; i++)
		{
			PORTA = ~(flag >> i);
			_delay_ms(150);
		}
	}
	
	return 0;
}