#include "stm32f10x.h"
#include "system_stm32f10x.h"
#include "usart_header.h"

void GPIOA_PIN_USART1()
{
	//GPIOA->CRH = 0;
	RCC->APB2ENR |=RCC_APB2ENR_AFIOEN | RCC_APB2ENR_IOPAEN;
	GPIOA->CRH &= ~GPIO_CRH_CNF9;// line can be ingored 
	GPIOA->CRH |= GPIO_CRH_CNF9_1 |GPIO_CRH_MODE9_0;// pin 9 set in AF output push pull
	GPIOA->CRH |=	GPIO_CRH_CNF10_0;
}
void UsartInit()
{
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN ;
	USART1->BRR = getBRRvalue(72000000,115200);
	USART1->CR1 |= USART_CR1_UE | USART_CR1_TE |USART_CR1_RE |USART_CR1_RXNEIE;
}
uint16_t getBRRvalue(int sysfeq,int BaudRate)
{
	double UsartDiv = (sysfeq/(16.0*BaudRate));
	double UsartFraction = UsartDiv-(int)UsartDiv;
	//	int carry=0;
	UsartFraction*= 16;
	//if (UsartFraction>=15.5)
	//	{
	//	}
	//USART1->BRR = ((USART1->BRR=(int)UsartDiv)<<4 )|(USART1->BRR= (int)UsartFraction);
	 return(unsigned short)( (int)UsartDiv<<4 | (0xF & (int)UsartFraction));
}

void myUsartPuts(char *data)
{	
	while(*data)
	{		
		UsartTxe_Read(*data++);
	}
	
	UsartTxe_Read('\n');
	UsartTxe_Read('\r');
}

void UsartTxe_Read(char chat)
{
		while((USART1->SR & USART_SR_TXE ) == 0x00);
		USART1->DR = chat;
}

void UsartStringPass(char *string)
{
	int kdx=0;
	while (1)
	{
		if ( string[kdx-1] != 0x0D || string[kdx-1] != 0x0A)
		{
			// recieve from usart terminal ;
			while((USART1->SR & USART_SR_RXNE) == 0x00);
			string[kdx] = (char)USART1->DR;
			// transmite on Usart terminal 
			while((USART1->SR & USART_SR_TXE ) == 0x00);
			USART1->DR = string[kdx];
		
			kdx++;
		}
		else
		{
			break;
		}
	}
}

