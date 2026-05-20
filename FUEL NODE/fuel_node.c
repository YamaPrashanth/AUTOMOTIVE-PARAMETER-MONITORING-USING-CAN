//fuel_node.c

#include "headers.h"
#include "can_defines.h"
#include "can.h"
#include "delay.h"

/*------------------------------------------------------------
Global Variable
------------------------------------------------------------*/

// CAN frame structure for data transmission
struct CAN_Frame txFrame;


/*------------------------------------------------------------
Function : main
Purpose  : Read fuel sensor value using ADC and transmit
           fuel percentage over CAN bus
------------------------------------------------------------*/
int main()
{
        f32 fuel,voltage;

        // Variables for ADC values corresponding to
        // empty and full fuel tank calibration
        f32 empty_tank,full_tank;

        // Example calibration values
        //f32 empty_tank=0.264,full_tank=2.380;

        Init_ADC();            // Initialize ADC module

        Init_CAN1();           // Initialize CAN1 peripheral

        while(1)
        {
                // Read analog voltage from fuel sensor
                voltage = Read_ADC(CH0);

                // Convert sensor voltages into ADC counts
                empty_tank=((0.238/3.3)*1023);

                full_tank=((2.364/3.3)*1023);

                // Calculate fuel percentage
                fuel=(((voltage-empty_tank)/(full_tank-empty_tank))*100);

                // Configure CAN frame
                txFrame.ID=1;              // CAN message ID

                txFrame.vbf.RTR=0;         // Data frame (not remote frame)

                txFrame.vbf.DLC=1;         // Data length = 1 byte

                txFrame.Data1=(u8)fuel;    // Store fuel percentage

                // Wait until transmit buffer becomes free
                while(!(C1GSR & TCS1_BIT_READ));

                // Transmit CAN frame
                CAN1_Tx(txFrame);

                delay_ms(500);             // Update every 500ms
        }
}
