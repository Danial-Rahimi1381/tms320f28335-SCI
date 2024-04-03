#include "DSP28x_Project.h"
#include "DSP2833x_Sci.h"
#include "sci.h"
#include "protocole.h"



interrupt void timer0_isr(void);

int i;
int f;
char *str = "\t\t HelllllllllllllOOOOOOOOOOoWWWWWWW \t\t";
struct ProtocolStructure message;

int main(void)
{  // MemCopy(RamfuncsLoadStart,RamfuncsLoadEnd,RamfuncsRunStart);
    // InitFlash();

    // Initialize system
       InitSysCtrl();

       // Initialize PIE control registers to their default state
       InitPieCtrl();
    InitPieVectTable();
    // Disable CPU interrupts and clear all CPU interrupt flags
    DINT;
    IER = 0x0000;
    IFR = 0x0000;

    // Initialize PIE vector table
    InitPieVectTable();
    // Configure CPU timer 0
    InitCpuTimers();

    ConfigCpuTimer(&CpuTimer0, 150, 1000); // 150MHz CPU, 1ms period
    // Set up interrupt handling for CPU timer 0
    EALLOW;
    PieVectTable.TINT0 = &timer0_isr;
    EDIS;
    // Enable CPU timer 0 interrupt
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;

    // Enable global interrupts
    IER |= M_INT1;
    EINT;
    EALLOW;
       // Enable internal pull-up for the selected pins
       // Pull-ups can be enabled or disabled disabled by the user.
       // This will enable the pullups for the specified pins.
       //
       GpioCtrlRegs.GPAPUD.bit.GPIO28 = 0;  // Enable pull-up for GPIO28 (SCIRXDA)
       GpioCtrlRegs.GPAPUD.bit.GPIO29 = 0;  // Enable pull-up for GPIO29 (SCITXDA)
       // Set qualification for selected pins to asynch only
       // Inputs are synchronized to SYSCLKOUT by default.
       // This will select asynch (no qualification) for the selected pins.
       GpioCtrlRegs.GPAQSEL2.bit.GPIO28 = 3;  // Asynch input GPIO28 (SCIRXDA)
       // Configure SCI-A pins using GPIO regs
       // This specifies which of the possible GPIO pins will be SCI functional
       // pins.
       //
       GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 1;   // Configure GPIO28 to SCIRXDA
       GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 1;   // Configure GPIO29 to SCITXDA

       EDIS;
    Init_Sci();

           message.Header1 = 'A';
           message.Header2 = 'B';
           message.Header3 = 'C';
           message.Header4 = 'D';
           message.data = "SendProtocol";
           message.Checksum = calculateChecksum(&message);
           message.EndofFrame = '\n';
           // Start CPU tiemr 0
           StartCpuTimer0();

    while(1){


}

}


__interrupt void timer0_isr(void)
{
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
    SendProtocol(message);


}
