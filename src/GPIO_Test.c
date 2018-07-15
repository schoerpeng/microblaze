
#include "xparameters.h"
#include "xgpio.h"
#include "xil_printf.h"
#include "xtmrctr_l.h"
#include "xstatus.h"

XGpio Gpio;

#define LED_CHANNEL 1
#define WR 0
#define RD 1
#define TMRCTR_BASEADDR		XPAR_TMRCTR_0_BASEADDR
#define TIMER_COUNTER_0	 0

int Delay_using_timer(u32 TmrCtrBaseAddress, u8 TmrCtrNumber);

int main(void)
{
	int Status;
	int Status_1;
	uint32_t ZERO = 0x00;
	uint32_t LED = 0x0F;

	// initialize the GPIO ip core
	Status = XGpio_Initialize(&Gpio,XPAR_AXI_GPIO_0_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		xil_printf("Gpio Initialization Failed\r\n");
		return XST_FAILURE;
	}

	/* Set the direction for all signals as inputs except the LED output */
	XGpio_SetDataDirection(&Gpio, LED_CHANNEL, ZERO);



    while(1)
    {

	XGpio_DiscreteWrite(&Gpio,LED_CHANNEL,LED);

	Status_1 = Delay_using_timer(TMRCTR_BASEADDR,TIMER_COUNTER_0);
	if (Status_1 != XST_SUCCESS) {
		xil_printf("Tmrctr lowlevel Example Failed\r\n");
		return XST_FAILURE;
	}

	xil_printf("Successfully ran Tmrctr lowlevel Example\r\n");

    XGpio_DiscreteClear(&Gpio, LED_CHANNEL, LED);

	Delay_using_timer(TMRCTR_BASEADDR,TIMER_COUNTER_0);

    }



	xil_printf("Successfully ran Gpio Example\r\n");
	return XST_SUCCESS;
}

int Delay_using_timer(u32 TmrCtrBaseAddress, u8 TmrCtrNumber)
{

	u32 Value1;
	u32 Value2;
	u32 Value3;
	u32 ControlStatus;

	XTmrCtr_SetControlStatusReg(TmrCtrBaseAddress, TmrCtrNumber,0x0);

	XTmrCtr_SetLoadReg(TmrCtrBaseAddress, TmrCtrNumber, 0xDEADBEEF);
	XTmrCtr_LoadTimerCounterReg(TmrCtrBaseAddress, TmrCtrNumber);

	ControlStatus = XTmrCtr_GetControlStatusReg(TmrCtrBaseAddress,
						 TmrCtrNumber);
	XTmrCtr_SetControlStatusReg(TmrCtrBaseAddress, TmrCtrNumber,
				 ControlStatus & (~XTC_CSR_LOAD_MASK));

	Value1 = XTmrCtr_GetTimerCounterReg(TmrCtrBaseAddress, TmrCtrNumber);
	XTmrCtr_Enable(TmrCtrBaseAddress, TmrCtrNumber);
	xil_printf("start counting \r\n");
	Value3 = Value1 - 100000;
	while (1) {
		Value2 = XTmrCtr_GetTimerCounterReg(TmrCtrBaseAddress, TmrCtrNumber);
		xil_printf("conuter value is %d\r\n",Value2);
		if (Value2 == 0 ) {
			xil_printf("conuter value is \r\n",Value2);
			break;
		}

	}



	/*
	 * Disable the timer counter such that it stops incrementing
	 */
	XTmrCtr_Disable(TmrCtrBaseAddress, TmrCtrNumber);

	return XST_SUCCESS;

}
