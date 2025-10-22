#include "device_registers.h"
#include "clocks_and_modes.h"
#include "LPUART.h"
#include "ADC.h"
#include "lcd1602A.h"
#include <stdio.h>


int lpit0_ch0_flag_counter = 0;
char buffer[20];
volatile int current_sw = 0;
unsigned int adcResultInMv=0;
int D=0;
unsigned int i = 0;                                                      


void LPIT0_init (void)
{
  PCC->PCCn[PCC_LPIT_INDEX] = PCC_PCCn_PCS(6);
  PCC->PCCn[PCC_LPIT_INDEX] |= PCC_PCCn_CGC_MASK;
  LPIT0->MCR |= LPIT_MCR_M_CEN_MASK;  
  LPIT0->TMR[0].TVAL = 20000000;
  LPIT0->TMR[0].TCTRL |= LPIT_TMR_TCTRL_T_EN_MASK;
  LPIT0->MIER |= LPIT_MIER_TIE0_MASK;
  S32_NVIC->ICPR[1] = 1 << (48 % 32);
  S32_NVIC->ISER[1] = 1 << (48 % 32);
}


void LPIT0_init1(uint32_t delay) {
    uint32_t timeout;
    PCC->PCCn[PCC_LPIT_INDEX] = PCC_PCCn_PCS(6);
    PCC->PCCn[PCC_LPIT_INDEX] |= PCC_PCCn_CGC_MASK;
    LPIT0->MCR |= LPIT_MCR_M_CEN_MASK;
    timeout = delay * 40;
    LPIT0->TMR[0].TVAL = timeout;
    LPIT0->TMR[0].TCTRL |= LPIT_TMR_TCTRL_T_EN_MASK;
}


void LPIT0_init2 (uint32_t delay){
   uint32_t timeout2;
  PCC->PCCn[PCC_LPIT_INDEX] = PCC_PCCn_PCS(6);
  PCC->PCCn[PCC_LPIT_INDEX] |= PCC_PCCn_CGC_MASK;
  LPIT0->MCR |= LPIT_MCR_M_CEN_MASK;
  timeout2=delay * 40000;
  LPIT0->TMR[0].TVAL = timeout2;
  LPIT0->TMR[0].TCTRL |= LPIT_TMR_TCTRL_T_EN_MASK;
}


void delay_us (volatile int us){
   LPIT0_init1(us);
   while (0 == (LPIT0->MSR & LPIT_MSR_TIF0_MASK)) {}
   lpit0_ch0_flag_counter++;
   LPIT0->MSR |= LPIT_MSR_TIF0_MASK;
}


void delay_ms(volatile int ms)
{
    while (ms > 0)
    {
        LPIT0->TMR[0].TVAL = 20000;
        LPIT0->TMR[0].TCTRL |= LPIT_TMR_TCTRL_T_EN_MASK;
        while (!(LPIT0->MSR & LPIT_MSR_TIF0_MASK)) {}
        LPIT0->MSR |= LPIT_MSR_TIF0_MASK;
        ms--;
    }
}


void delay_ms2 (uint32_t ms){
     LPIT0_init2(ms);
   while (0 == (LPIT0->MSR & LPIT_MSR_TIF0_MASK)) {}
   lpit0_ch0_flag_counter++;
   LPIT0->MSR |= LPIT_MSR_TIF0_MASK;
}


void NVIC_init_IRQs(void)
{
    S32_NVIC->ICPR[1] |= 1 << (61 % 32);
    S32_NVIC->ISER[1] |= 1 << (61 % 32);
}


void LPIT0_Ch0_IRQHandler(void)
{
    LPIT0->MSR |= LPIT_MSR_TIF0_MASK;


    if (current_sw == 2) {
        PTD->PSOR = (1 << 2) | (1 << 8);
        PTC->PCOR = (1 << 9);
        current_sw = 0;
    }
    else {
        switch (current_sw)
        {
        case 1: PTD->PTOR = (1 << 2); break;
        case 5: PTD->PTOR = (1 << 8); break;
        case 3: PTD->PTOR = (1 << 2) | (1 << 8); break;
                case 4: PTC->PTOR = (1<< 9); break;
        default: break;
        }
    }
}

void PORTC_IRQHandler(void)
{
    if (PORTC->ISFR & (1 << 11)) { current_sw = 1; PORTC->PCR[11] |= (1 << 24); }
    else if (PORTC->ISFR & (1 << 12)) { current_sw = 2; PORTC->PCR[12] |= (1 << 24); }
    else if (PORTC->ISFR & (1 << 13)) { current_sw = 3; PORTC->PCR[13] |= (1 << 24); }
    else if (PORTC->ISFR & (1 << 16)) { current_sw = 4; PORTC->PCR[16] |= (1 << 24); }
    else if (PORTC->ISFR & (1 << 17)) { current_sw = 5; PORTC->PCR[17] |= (1 << 24); }
}


void FTM_init (void){
    PCC->PCCn[PCC_FTM0_INDEX] &= ~PCC_PCCn_CGC_MASK;
    PCC->PCCn[PCC_FTM0_INDEX] |= PCC_PCCn_PCS(0b010)| PCC_PCCn_CGC_MASK;
    FTM0->SC = FTM_SC_PWMEN2_MASK |FTM_SC_PS(0.5)|FTM_SC_PWMEN3_MASK;
    FTM0->MOD = 8000-1;
    FTM0->CNTIN = FTM_CNTIN_INIT(0);
    FTM0->CONTROLS[2].CnSC |=FTM_CnSC_MSB_MASK;
    FTM0->CONTROLS[2].CnSC |=FTM_CnSC_ELSA_MASK;
    FTM0->CONTROLS[3].CnSC |=FTM_CnSC_MSB_MASK;
    FTM0->CONTROLS[3].CnSC |=FTM_CnSC_ELSA_MASK;    
    FTM0->COMBINE |= FTM_COMBINE_SYNCEN1_MASK | FTM_COMBINE_COMP1_MASK | FTM_COMBINE_DTEN1_MASK;
}


void FTM0_CH_PWM(uint32_t i) {
  FTM0->CONTROLS[2].CnV = i;
  FTM0->SC |= FTM_SC_CLKS(3);
    FTM0->CONTROLS[3].CnV = 8000-i;
  FTM0->SC |= FTM_SC_CLKS(3);
}


void PORT_init (void)
{
    PCC->PCCn[PCC_PORTD_INDEX] |= PCC_PCCn_CGC_MASK;
    PORTD->PCR[0] |= PORT_PCR_MUX(2);
    PORTD->PCR[1] |= PORT_PCR_MUX(2);
      PORTD->PCR[8] = PORT_PCR_MUX(1);
    PORTD->PCR[2] = PORT_PCR_MUX(1);
    PTD->PDDR |= (1 << 8) | (1 << 2);
    PTD->PSOR = (1 << 8) | (1 << 2);
        PTD->PDDR |= 1<<9 | 1<<10 | 1<<11 | 1<<12 | 1<<13 | 1<<14 | 1<<15;
    PCC->PCCn[PCC_PORTC_INDEX] |= PCC_PCCn_CGC_MASK;
    PORTC->PCR[6] |= PORT_PCR_MUX(2);
    PORTC->PCR[7] |= PORT_PCR_MUX(2);
        PORTC->PCR[9] |= PORT_PCR_MUX(1);
    PTC->PDDR |= (1 << 9);
        PTC->PCOR = (1 << 9);
        PTC->PDDR &= ~(1 << 11) & ~(1 << 12) & ~(1 << 13) & ~(1 << 16) & ~(1 << 17);
        PORTC->PCR[11] |= PORT_PCR_MUX(1) | (10 << 16);
    PORTC->PCR[12] |= PORT_PCR_MUX(1) | (10 << 16);
    PORTC->PCR[13] |= PORT_PCR_MUX(1) | (10 << 16);
    PORTC->PCR[16] |= PORT_PCR_MUX(1) | (10 << 16);
    PORTC->PCR[17] |= PORT_PCR_MUX(1) | (10 << 16);
}


void buzzer_toggle(void) {
    PTC->PSOR = (1 << 9);
    delay_us(300000);
    PTC->PCOR = (1 << 9);
    delay_us(300000);
}


void WDOG_disable(void) {
    WDOG->CNT = 0xD928C520;
    WDOG->TOVAL = 0x0000FFFF;
    WDOG->CS = 0x00002100;
}


void display_text_lcd(char *msg) {
    lcdinput(0x01);
    delay_ms2(20);
    for (int i = 0; msg[i] != '\0'; i++) {
        lcdcharinput(msg[i]);
        delay_ms2(100);
    }
    delay_ms2(2000);
    lcdinput(0x08);  
    delay_ms2(400);
    lcdinput(0x01);
    delay_ms2(200);
}


int main(void) {
    uint32_t D;
    WDOG_disable();
    SOSC_init_8MHz();
    SPLL_init_160MHz();
    NormalRUNmode_80MHz();
    SystemCoreClockUpdate();
    PORT_init();
    FTM_init();
    ADC_init();
    LPUART1_init();
    lcdinit();
    char msg_array1[16] = {0x53, 0x4C, 0x4F, 0x57, 0x20, 0x44, 0x4F, 0x57, 0x4E, 0x20, 0x50, 0x4C, 0x45, 0x41, 0x53, 0x00};


    for (;;) {
        convertAdcChan(13);                  
        while (adc_complete() == 0) {}
        adcResultInMv = read_adc_chx();
        D = adcResultInMv * 1.6;
        FTM0_CH_PWM(D);
        if (adcResultInMv > 4250) {
            display_text_lcd(msg_array1);
            buzzer_toggle();
            adcResultInMv = 4250;
        }
        else if (adcResultInMv < 2470) {
            buzzer_toggle();  
        }
        else {
            PTC->PCOR = (1 << 9);
        }
        sprintf(buffer, "ADC Value: %umV\n\r", adcResultInMv);
        LPUART1_transmit_string(buffer);
        delay_ms2(500);
    }
}
