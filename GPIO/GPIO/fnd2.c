/*
* fnd2.c
*
* Created: 2022-06-17 오후 4:17:31
* Author: KIM DONG HUN
*/
#define F_CPU 7432800UL
#include <avr/io.h>
#include <util/delay.h>

int FND[] =
{
	0x3f,	0x06,	0x5b,	0x4f,
	0x66,	0x6d,	0x7d,	0x27,
	0x7f,	0x6f,	0x77,	0x7c,
	0x39,	0x5e,	0x79,	0x71
};

int FND_SWITCH[4] =
{
	0b00000111,
	0b00001011,
	0b00001101,
	0b00001110
};

void Init()
{
	DDRA = 0xFF;	// PRORTA0 ~ PRORTA7을 출력으로 설정 0b 1111 1111
	DDRD = 0x0F;	// PRORTD0 ~ PRORTD3을 출력으로 설정 0b 0000 1111
}

int main()
{
	Init();
	
	int digit[4] = {0,0,0,0};
	
	while (1)
	{
		for (int num = 0 ; num < 10000 ; num++)
		{
			// num 자리수 단위로 쪼개기
			digit[0] = num%10;
			digit[1] = num%100/10;
			digit[2] = num%1000/100;
			digit[3] = num/10000;
			
			// 해당 num 값을 100ms(25*4*1ms) 만큼 출력
			for (int j = 0; j < 25 ; j++)
			{
				// i번째 FND만 켜고, i번째 자리수의 숫자를 FND에 출력
				for (int i = 0; i < 4 ; i++)
				{
					// i번째 FND만 켜기
					PORTD = FND_SWITCH[i];
					
					// i번째 자리수의 숫자를 FND에 출력
					PORTA = FND[digit[i]];
					
					//소수점 찍기
					if(i == 1)
					{
						PORTA |= 0x80;
					}
					_delay_ms(1);
				}
			}
		}
	}
	
	return 0;
}

