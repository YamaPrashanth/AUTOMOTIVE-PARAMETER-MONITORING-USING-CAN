//indicator_node.c

#include "headers.h"
#include "can_defines.h"
#include "can.h"
#include "delay.h"

/*------------------------------------------------------------
Macro Definitions
------------------------------------------------------------*/

// P0.0 to P0.7 connected to ACTIVE LOW LEDs
#define LEDS (0xFF<<0)


/*------------------------------------------------------------
Global Variables
------------------------------------------------------------*/

// CAN frame structure for receiving data
struct CAN_Frame rxFrame;

// Variable to store indicator command received from Main Node
u8 indicator_state = 0;


/*------------------------------------------------------------
Indicator Direction Information
------------------------------------------------------------*/
// Left Indicator  : LEDs glow from Right ? Left
// Right Indicator : LEDs glow from Left ? Right


/*------------------------------------------------------------
Function : main
Purpose  : Receive indicator commands through CAN bus
           and control LED indicator sequence
------------------------------------------------------------*/
int main()
{
        int i,j;

        /*----------------------------------------------------
        LED Initialization
        ----------------------------------------------------*/

        // Configure LED pins as output
        IODIR0 |= LEDS;

        // Turn OFF all LEDs initially (Active LOW LEDs)
        IOSET0 = LEDS;


        /*----------------------------------------------------
        CAN Initialization
        ----------------------------------------------------*/
        Init_CAN1();


        /*----------------------------------------------------
        Infinite Loop
        ----------------------------------------------------*/
        while(1)
        {
                /*--------------------------------------------
                Receive Indicator Command from Main Node
                --------------------------------------------*/
                if(C1GSR & RBS_BIT_READ)
                {
                        CAN1_Rx(&rxFrame);

                        // Check if message belongs to indicator node
                        if(rxFrame.ID == 2)
                        {
                                indicator_state = rxFrame.Data1;
                        }
                }


                /*--------------------------------------------
                LEFT Indicator Operation
                LEDs glow from Right ? Left
                --------------------------------------------*/
                if(indicator_state == 1)
                {
                        //Left_Indicator();

                        for(i=7;i>=0;i--)
                        {
                                // Check for updated CAN message
                                if(C1GSR & RBS_BIT_READ)
                                {
                                        CAN1_Rx(&rxFrame);

                                        if(rxFrame.ID == 2)
                                        {
                                                indicator_state = rxFrame.Data1;
                                        }
                                }

                                // Stop sequence if state changes
                                if(indicator_state!=1)
                                        break;

                                // Turn ON LED (Active LOW)
                                IOCLR0=(1<<i);

                                delay_ms(120);

                                // Turn OFF LED
                                IOSET0=(1<<i);

                                /*else{
                                        IOCLR0 = (1<<i);
                                        delay_ms(120);
                                        IOSET0 = (1<<i);
                                }*/
                        }
                }


                /*--------------------------------------------
                RIGHT Indicator Operation
                LEDs glow from Left ? Right
                --------------------------------------------*/
                else if(indicator_state == 2)
                {
                        //Right_Indicator();

                        for(j=0;j<=7;j++)
                        {
                                // Check for updated CAN message
                                if(C1GSR & RBS_BIT_READ)
                                {
                                        CAN1_Rx(&rxFrame);

                                        if(rxFrame.ID == 2)
                                        {
                                                indicator_state = rxFrame.Data1;
                                        }
                                }

                                // Stop sequence if state changes
                                if(indicator_state!=2)
                                        break;

                                // Turn ON LED (Active LOW)
                                IOCLR0=(1<<j);

                                delay_ms(120);

                                // Turn OFF LED
                                IOSET0=(1<<j);

                                /*else{
                                        IOCLR0 = (1<<j);
                                        delay_ms(120);
                                        IOSET0 = (1<<j);
                                }*/
                        }
                }


                /*--------------------------------------------
                Indicator OFF State
                --------------------------------------------*/
                else // indicator_state == 3
                {
                        // Turn OFF all LEDs
                        IOSET0 = LEDS;
                }
        }
}
