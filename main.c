
#include <p24fxxxx.h>
// Configuration Bits to make the part run from Internal FRCDIV
// Oscillator.
_FBS(     
    BWRP_OFF &      		// Table Write Protect Boot (Boot segment may be written)     
	  BSS_OFF         		// Boot segment Protect (No boot program Flash segment) 
)

_FGS (     
    GWRP_OFF &      		// General Segment Code Flash Write Protection bit (General segment may be written)     
    GCP_OFF         		// General Segment Code Flash Code Protection bit (No protection) 
) 

_FOSCSEL(     
    FNOSC_FRCDIV &  	// Oscillator Select (8 MHz FRC oscillator with divide-by-N (FRCDIV))
    IESO_ON         		// Internal External Switch Over bit (Internal External Switchover mode enabled (Two-Speed Start-up enabled)) 
)

_FOSC (     
    POSCMOD_NONE &  	// Primary Oscillator Configuration bits (Primary oscillator disabled)     
    OSCIOFNC_OFF &  	// CLKO Enable Configuration bit (CLKO output disabled)        	
    POSCFREQ_MS &	// Primary Oscillator Frequency Range Configuration bits (Primary oscillator/external clock input // frequency between 100 kHz and 8 MHz)
    SOSCSEL_SOSCHP &	// SOSC Power Selection Configuration bits (Secondary oscillator configured for high-power operation)     
    FCKSM_CSECME    	// Clock Switching and Monitor Selection (Both Clock Switching and Fail-safe Clock Monitor are enabled) 
)

_FWDT (     
    WDTPS_PS32768 & 	// Watchdog Timer Postscale Select bits (1:32,768)     
    FWPSA_PR128 &  	 // WDT Prescaler (WDT prescaler ratio of 1:128)     
    WINDIS_OFF &    		/ Windowed Watchdog Timer Disable bit (Standard WDT selected; windowed WDT disabled)    
    FWDTEN_OFF      		// Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
) 

_FPOR (     
    BOREN_BOR3 &    	// Brown-out Reset Enable bits (Brown-out Reset enabled in hardware; SBOREN bit disabled)     
    PWRTEN_ON &     	// Power-up Timer Enable bit (PWRT enabled)     
    I2C1SEL_PRI &   		// Alternate I2C1 Pin Mapping bit (Default location for SCL1/SDA1 pins)     
    BORV_V18 &      		// Brown-out Reset Voltage bits (Brown-out Reset set to lowest voltage (1.8V))     
    MCLRE_ON       		// MCLR Pin Enable bit (MCLR pin enabled; RA5 input pin disabled) 
    )
    
_FICD(
    ICS_PGx3          // ICD Pin Placement Select bits (PGC3/PGD3 are used for programming and debugging the device) 
) 

_FDS (     
    DSWDTPS_DSWDTPSF &	// Deep Sleep Watchdog Timer Postscale Select bits (1:2,147,483,648 (25.7 Days))     
    DSWDTOSC_LPRC & 	// DSWDT Reference Clock Select bit (DSWDT uses LPRC as reference clock)     
    RTCOSC_SOSC &   	// RTCC Reference Clock Select bit (RTCC uses SOSC as reference clock)     
    DSBOREN_OFF &   	// Deep Sleep Zero-Power BOR Enable bit (Deep Sleep BOR disabled in Deep Sleep)     
    DSWDTEN_OFF     	// Deep Sleep Watchdog Timer Enable bit (DSWDT disabled) 
) 


void adc_init() {
     AD1CON1 = 0x00E0;   	// auto convert after end of sampling
     AD1CSSL = 0;        	// no scanning required 
     AD1CON3 = 0x1F02;   	// max sample time = 31Tad, Tad = 2 x Tcy = 125ns >75ns
     AD1CON2 = 0;        	// use MUXA, AVss and AVdd are used as Vref+/-      
     AD1PCFGbits.PCFG0=0;	// AN0 analog pin
     AD1CON1bits.ADON = 1;   // Turn ON ADC 
} 

void uart_init() {     
     U2BRG = 51;   		//8MHz baud rate 9600     
     U2MODE = 0x8000;   	//UART enable 8bit no parity     
     U2STA = 0x0400;	 	// Enable transmission, clear all flags
} 


void delay() {       
    int i;       
    for(j = 0; j < 30; j++ )       
      for(i = 0; i < 65535; i++) {         
         Nop();             
         Nop();             
         Nop();             
         Nop();         
       }
} 

int main(void) {
    unsigned int i;    
    unsigned int ain;    
    unsigned char data;     
    float temp;   
    TRISAbits.TRISA0 = 1;     
    adc_init();     
    uart_init();     
    while(1)     {      
       AD1CHS = 0; 			// select analog input channel AN0
       AD1CON1bits.SAMP = 1;      	// start sampling, automatic conversion will follow;
       while (!AD1CON1bits.DONE);  	// wait to complete the conversion
       ain = ADC1BUF0;            	// read the conversion result
       temp = 0.3225*ain;
       data= temp;  			//assign 8 bit LSB
       U2TXREG = data;  		//send 8 bit LSB
       while(!U2STAbits.TRMT);  	// Wait tiil transmit buffer empty
       delay();  			//30 sec delay    
       }
}
