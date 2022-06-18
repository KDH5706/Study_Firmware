/*
* fnd2.c
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

int FND_SW[4] =
{
	0b00000111,
	0b00001011,
	0b00001101,
	0b00001110
};


int main()
{
	DDRA = 0xFF;	// PRORTA를 출력으로 설정
	DDRD = 0x0F;	// PRORTA를 출력으로 설정	0b 0000 1111
	
	int display[4] = {0,0,0,0};
	
	while (1)
	{
		for (int num = 1000 ; num > 0 ; num--)
		{
			// num 자리수 단위로 쪼개기
			display[0] = num%10;
			display[1] = num%100/10;
			display[2] = num%1000/100;
			display[3] = num/10000;
			
			// 해당 num 100ms 만큼 출력
			for (int j = 0; j < 5 ; j++)
			{
				// i번째 FND만 켜고, i번째 자리수의 숫자를 FND에 출력
				for (int i = 0; i < 4 ; i++)
				{
					// i번째 FND만 켜기
					PORTD = FND_SW[i];
					// i번째 자리수의 숫자를 FND에 출력
					PORTA = FND[display[i]];
					_delay_ms(5);
				}
			}
		}
	}
	
	
	return 0;
}

