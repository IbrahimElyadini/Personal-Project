/*
 * Keypad_Operation.c
 *
 * Created: 1/30/2023 11:39:20 AM
 * Author : 2038471
 */ 

#include <avr/io.h>
#define F_CPU 1000000UL
#include <util/delay.h>
#define readc1 0x01 ;		//read column 1 input
#define readc2 0x02 ;		//read column 2 input
#define readc3 0x04 ;		//read column 3 input
#define readc4 0x08 ;		//read column 4 input
void getkey();
void checkkey();
void checkcode();
void outputkey();
void outputkeyc1();
void outputkeyc2();
void outputkeyc3();
void deletecode();
int code[5]={1,2,3,4};	//the numbers on the keypad activate other keypad numbers after they have been pressed ex: 6=4, 9=7, 3=?
int samplecode[5];	//maybe use char
int previouskeypress;
int keypressvar;

	int insc[4]; //maybe just 3
	int x;
	int y;	//used for the array of the sample code
	int led;
int main(void)
{
	//DDRD = 0x00;	//Keypad input
	DDRC = 0xFF;	//output to see number
	DDRA = 0xFF;
	DDRE = 0xFF;
	PORTE=0x00;
	x=0;
	y=0; //initialized
	previouskeypress = 0;
	keypressvar = 0;	
    while (1) 
    {	
		getkey();
		checkkey();	
			
		
    }
}


void getkey()
{
	DDRD = readc1; //first column
	PORTD = 0xF0;	//set as low to read
	insc[0] = PIND;
	insc[0]=insc[0]>>4;
	PORTD = 0x00; //reset output
	DDRD = 0x00; //reset input
	DDRD = readc2; //Second column
	PORTD = 0xF0;	//set as low to read
	insc[1] = PIND;
	insc[1]=insc[1]>>4;
	PORTD = 0x00; //reset output
	DDRD = 0x00; //reset input	
	DDRD = readc3; //third column
	PORTD = 0xF0;	//set as low to read
	insc[2] = PIND;		
	insc[2]=insc[2]>>4; //For testing purposes they are kept here
	PORTD = 0x00; //reset output
	DDRD = 0x00; //reset input
	DDRD = readc4; //first column
	PORTD = 0xF0;	//set as low to read
	insc[3] = PIND;
	insc[3]=insc[3]>>4;
	PORTD = 0x00; //reset output
	DDRD = 0x00; //reset input

	return;
}

void checkkey()
{
		
		
	for(x=0;x<3;x++)
	{
	
		if(insc[x]==0x0F)
		{	
			
			PORTA = 0x01;
			
		}
		else
		{
			outputkey();
			return;	
		}
	}
	return;
}
void outputkey()
{	
	PORTA = 0x02;
	switch(x)
	{
		case 0:outputkeyc1();		_delay_ms(1000.0);	 break;
		case 1:outputkeyc2();		_delay_ms(1000.0);	 break;
		case 2:outputkeyc3();		_delay_ms(1000.0);	 break;
		
	}
	if(y>3)
	{
	checkcode();
	}
	return;
}	
void outputkeyc1()
{
	switch(insc[0])
	{
		case 14:		samplecode[y]=1;	y++;	 break;
		case 13:		samplecode[y]=4;	y++; 	 break;
		case 11:		samplecode[y]=7;	y++; 	 break;
		case 7:			PORTA = 0x04;		 break; //not number
		default:		PORTA = 0x04;		 break;
	}
	
	return;
}

void outputkeyc2()
{
	switch(insc[1])
	{
		case 14:		samplecode[y]=2;	y++;	 break;
		case 13:		samplecode[y]=5;	y++;	 break;
		case 11:		samplecode[y]=8;	y++;	 break;
		case 7:			samplecode[y]=0;	y++;	 break; 
		default:	PORTA = 0x04;		 break;
	}
	return;
}

void outputkeyc3()
{
	switch(insc[2])
	{
		case 14:		samplecode[y]=3;	y++; 	 break;
		case 13:		samplecode[y]=6;	y++;	 break;
		case 11:		samplecode[y]=9;	y++;	 break;
		case 7:			PORTA = 0x04;		 break;
		default:		PORTA = 0x04;		 break;
	}
	
	return;
}

void deletecode()
{

	samplecode[0]=0;
	samplecode[1]=0;
	samplecode[2]=0; 
	samplecode[3]=0;
	y=0;
	x=0;
	insc[0]=0x0F;
	insc[1]=0x0F;
	insc[2]=0x0F;
	return;
}
void checkcode()
{
	int error=0;
	if(samplecode[0]!=code[0])
	error++;
	if(samplecode[1]!=code[1])
	error++;
	if(samplecode[2]!=code[2])
	error++;
	if(samplecode[3]!=code[3])
	error++;		
		
	if(error==0)
	{
		PORTA = 0x08;
		PORTE = 0xFF;
		deletecode();
		_delay_ms(5000.0);
		PORTA = 0x00;
		PORTE =0x00;
		y=0;
		return;
	}
	else	
	{ 
	deletecode();
	PORTA = 0x08;
	_delay_ms(200.0);
	PORTA = 0x04;
	_delay_ms(200.0);
	PORTA = 0x02;
	_delay_ms(200.0);
	PORTA = 0x01;
	_delay_ms(200.0);
	y=0;	
	return;
	}
}
