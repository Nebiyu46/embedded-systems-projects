/*----------------------------------------------------------------------------
    File name   : interrupts.c

    Description : implements interrupt initialization and installing routines

    Procesor    : Philips LPC2148 MCU with ARM7TDMI-s Core

    Note        : In every great computer system interrups play a major role and hence, 
                  are the most indespensable resources in the system

                [Please refer to LPC2148 MCU user manual, chapter 7 "VIC"]

           Acronyms:
                VIC = Vectored Interrupt Controller (chapter 7 on Manual)
                IRQ = Interrupt Request (Normal interrupt request to the ARM VIC)
                FIQ = Fast Interrupt Request
   
 ECEg-4501 - Microcomputers and interfacing, lab exercise II
 ----------------------------------------------------------------------------*/

#include "NXP/iolpc2124.h"
#include "inr.h"

//Some local function definitions

void DefVectISR(void);          //Default ISR for non-vectored IRQ
static void (* fiq_isr)(void);  //FIQ ISR (there can only be one FIQ)

/*-------------------------------------------------------------------------
   Function Name: VIC_init

   Parameters: None
 
   Return:  None
 
   Description: Initializes the Vectored Interrupt Controller
                Clears and Disables all interrupts. All interrupts are set
                to IRQ mode. 
 ---------------------------------------------------------------------------*/
void VIC_init(void)
{
  unsigned int *VectAddr = (unsigned int *)&VICVectAddr0; //define pointer to vector address
  unsigned int *VectCntl = (unsigned int *)&VICVectCntl0; //define pointer to int controller register address

  VICIntEnClear = 0xFFFFFFFF;    //Disable all interrupts
  
  VICSoftIntClear = 0xFFFFFFFF;  //Clear Software Interrupts
  
  VICVectAddr = 0;               //Write to VicVectAddr register
  
  VICIntSelect = 0;              //Set all interrupts to to IRQ
  
  VICDefVectAddr = 0;            //Set Default Vector Address to NULL
  
  for ( int i = 0; i < VIC_CHANNELS; i++ ) //Set all the vector addresses to NULL (disable all IRQ channels)
  {                                                    
    VectAddr[i] = 0x0;
    VectCntl[i] = 0x0;
  }
}
/*---------------------------------------------------------------------------------
   Function Name: Instal_IRQ

   Parameters   :Interrupt Number, pointer to the user ISR, and VIC channel
 
   Return       : None
 
   Description: Installs Interrupt Serice Routine(ISR) at selected VIC channel and
                enables the interrupt. This function can be used for enabling
                a default interrupt if it is called with channel >= VIC_CHANNELS
 ----------------------------------------------------------------------------------*/
void install_IRQ(unsigned int channel , void (*ISR)(void), unsigned int IntNumber )
{
   unsigned int * VectAddr = (unsigned int *)&VICVectAddr0;
   unsigned int * VectCntl = (unsigned int *)&VICVectCntl0;

   VICIntEnClear |=( 1<<channel);  //Disable Interrupt at the specified channel 
  
   if(VIC_CHANNELS > IntNumber)     //Vectored IRQ
   {
     VectAddr[IntNumber] = (unsigned int)ISR;  //Set interrupt Vector to ISR
     VectCntl[IntNumber] = channel | (1<<5); //Set Int Number and enable the channel
   }
   else   //Non-vectored IRQ
   {
     VICDefVectAddr = (unsigned int)DefVectISR;  //Install ISR for non vectored IRQ (default ISR)
   }

   VICIntEnable |= (1 << channel);  //Enable Interrupt
}

/*--------------------------------------------------------------------------
  Function Name: Install_FIQ

  Parameters   : Interrupt Number and pointer to the user ISR
 
  Return       :  None
 
  Description: Sets Interrupt in FIQ mode and enables it
 --------------------------------------------------------------------------*/
void install_FIQ(unsigned int channel,   void (*ISR)(void))
{
  VICIntEnClear |= 1<<channel;   //Disable Interrupt
  VICIntSelect |= 1<<channel;   //Set FIQ mode
  fiq_isr = ISR;                  //Set interrupt Vector
  VICIntEnable |= (1 << channel);  //Enable Interrupt
}
/*--------------------------------------------------------------------------
  Function Name: IRQ_Handler

  Parameters   :None
 
  Return       :None
 
  Description  :The IRQ Handler (when IRQ occurs CPU branches to here)
 -------------------------------------------------------------------------*/
__irq __arm void IRQ_Handler(void)
{
  void (* IntVector)(void);

  IntVector = (void (*)(void)) VICVectAddr;    //Read Interrupt Vector
  (* IntVector)();                             //Call ISR
  
  VICVectAddr = 0;                    //Dummy write to Vector address register (Errata)
}

/*--------------------------------------------------------------------------
  Function Name: FIQ_Handler
  Parameters   : None
 
  Return       : None
 
  Description  : The FIQ Handler
 ---------------------------------------------------------------------------*/
__fiq __arm void FIQ_Handler(void)
{
  (* fiq_isr)();                             //Call ISR
}
/*---------------------------------------------------------------------------
  Function Name: DefVectISR
  Parameters   : None
 
  Return       : None
 
  Description  : The Non-vectored IRQ ISR
 --------------------------------------------------------------------------*/

static void DefVectISR (void)   
{
   //Put Code of Non-vectored ISR here
  return;
}

