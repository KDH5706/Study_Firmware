/*
* fnd1.c
*
* Created: 2022-06-17 오후 4:17:31
*  Author: PKNU
*/
#define F_CPU 7432800UL
#include <avr/io.h>
#include <util/delay.h>

int FND[16] =
{
	0x3f,	0x06,	0x5b,	0x4f,
	0x66,	0x6d,	0x7d,	0x27,
	0x7f,	0x6f,	0x77,	0x7c,
	0x39,	0x5e,	0x79,	0x71
};


int main()
{
	DDRA = 0xFF;	// PRORTA를 출력으로 설정
	DDRD = 0x00;
	
	while (1)
	{
		for (int i = 0; i<16; i++)
		{
			PORTA = FND[i];
			_delay_ms(300);
		}
	}
	
	return 0;
}

