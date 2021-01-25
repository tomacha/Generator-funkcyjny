/*-----------------------------------------------------------------------------
					Technika Mikroprocesorowa 2 - projekt
					Generator funkcyjny
					autor: Michał Tomacha
					wersja: 1.0
------------------------------------------------------------------------------*/
					
#include "MKL05Z4.h"
#include "DAC.h"
#include "pit.h"
#include "frdm_bsp.h"
#include "i2c.h"
#include "lcd1602.h"
#include "tsi.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static uint8_t wave=0, slider=0, adj=0;
static uint16_t dac=0, ampl=2;
static uint32_t freq=14460, prev_freq=14460; 
static double vdac = 0.028;

static const uint16_t triangle[]={0x4,0x8,0xc,0x10,0x14,0x18,0x1c, // tablica próbek fali trójkątnej
																	0x20,0x24,0x28,0x24,0x20,0x1c,0x18,
																	0x14,0x10,0xc,0x8,0x4,0x0};

static const uint16_t sine[]={0x14,0x1a,0x20,0x24,0x27,0x28,0x27, // tablica próbek sinusa
															0x24,0x20,0x1a,0x14,0xe,0x8,0x4,0x1,
															0x0,0x1,0x4,0x8,0xe};

static const uint16_t square[]={20, 28, 35, 37, 35, 30, 23, 18, 16, 17, 19, 22, 23, 21, 16, 10,  4,  2,  4, 11};

static char display[]={0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20};

void PIT_IRQHandler(void);

int main (void)
{
	LCD1602_Init();						// Inicjalizacja wyświetlacza 	
	LCD1602_Backlight(TRUE);	// Włączenie podświetlenia wyświetlacza
	TSI_Init();								// Inicjalizacja pola dotykowego
	DAC_Init();								// Inicjalizacja przetwornika C/A
	DAC_Load_Trig(dac);				// Wyzwolenie prztwornika C/A wartością początkową
	PIT_Init();								// Inicjalizacja licznika PIT0
	
	PORTB->PCR[6] |= PORT_PCR_MUX(1);
	PORTB->PCR[6] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
	PORTB->PCR[7] |= PORT_PCR_MUX(1);
	PORTB->PCR[7] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
	
	LCD1602_SetCursor(0,0);
	LCD1602_Print("adj. freq");	
	LCD1602_SetCursor(13,0);
	LCD1602_Print("sqr");
	LCD1602_SetCursor(0,1);
	sprintf(display, "%dHz     ", (20+((97-2)*152)));
	LCD1602_Print(display);
	
	while(1)				
	{
		slider=TSI_ReadSlider();
		
		if(slider>1)
		{
			if(adj == 0)
			{
				prev_freq=freq;
				freq=20+((slider-2)*152);
				LCD1602_SetCursor(0,1);
				sprintf(display, "%dHz     ", freq);
				LCD1602_Print(display);
				
				
				if(freq!=prev_freq)
				{
					PIT->CHANNEL[0].TCTRL ^= PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK;		// Zatrzymanie przerwania i licznika
					PIT->CHANNEL[0].LDVAL = PIT_LDVAL_TSV(BUS_CLOCK/(freq*20));					// Zmiana częstotlowości przerwań
					PIT->CHANNEL[0].TCTRL = PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK;		// Odblokowanie przerwania i wystartowanie licznika
				}
			}
			else // wyświetlanie wartości napięcia międzyszczytowego
			{
				ampl=slider; 
				LCD1602_SetCursor(0,1);
				sprintf(display, "%.2fV   ", vdac*ampl);
				LCD1602_Print(display);
			}
		}
		
		if((PTB->PDIR & (1<<6)) == 0)
		{
			adj=~adj; 												// przy wciśnięciu przycisku zmiana trybu między zmianą napięcia a częstotliwości 
		
			LCD1602_SetCursor(0,0);           // wyswietlenie informacji o obecnym trybie
			if(adj == 0)											
				LCD1602_Print("adj. freq    ");	
			else															
				LCD1602_Print("adj. Vp-p    ");	
		
			LCD1602_SetCursor(0,1);						// zaktualizowanie wyświetlaniej wartości przy zmianie trybu
			if(adj == 0)
			{
				sprintf(display, "%dHz     ", freq);
				LCD1602_Print(display);
			}			
			else
			{
				sprintf(display, "%.2fV   ", vdac*ampl);
				LCD1602_Print(display);
			}
		
			while((PTB->PDIR & (1<<6)) == 0) // debouncing oraz ignorowanie przytrzymania przycisku
				DELAY(10)
		}

		if((PTB->PDIR & (1<<7)) == 0)
		{
			if(wave>1) 	// zmiana tablicy próbek na kolejną
				wave=0;
			else		
				wave++;
		
			LCD1602_SetCursor(13,0); // wyswietlenie informacji o obecnym kształcie przebiegu
			switch(wave)
			{
				case 1:
					LCD1602_Print("sin");	
					break;
				case 2:
					LCD1602_Print("tri");	
					break;
				default:
					LCD1602_Print("sqr");	
					break;
			}
		
			while((PTB->PDIR & (1<<7)) == 0) // debouncing oraz ignorowanie przytrzymania przycisku
				DELAY(10)
		}
	}
}

void PIT_IRQHandler()
{
	PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK;		// Skasuj flagę żądania przerwania
	
	if(dac>18) // zmiana indeksów próbek w tablicy
		dac=0; 
	else
		dac++;
	
	switch(wave) // Ustawienie nowej wartości prztwornika C/A	w zależności od wybranego kształtu przebiegu
	{
		case 1:
			DAC_Load_Trig((uint16_t)(sine[dac]*ampl));		
			break;
		case 2:
			DAC_Load_Trig((uint16_t)(triangle[dac]*ampl));			
			break;
		default:
			DAC_Load_Trig((uint16_t)(square[dac]*ampl));			
			break;
	}
}
