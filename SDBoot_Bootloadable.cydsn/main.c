/* SD Card Based Bootloader for PSoC3
 Author : Kartik Mankad
 Date : 11-Jun-2012
 This code is licensed as CC-BY-SA 3.0
 Description : The PSoC programs a cyacd file from an SD Card to its flash memory.
 
This file contains the code for the test case Bootloadable.
*/


/*******************************************************************************
* File Name: main.c
*
* Version: 2.0
*
* Description:
*   This project is a test case bootloadable for the SD Card based bootloader.
*	
*  To test bootloader:
*	1. Add the emFile dependencies for "emf32noslfn" configuration as explained
*	   in the emFile datasheet.(See "Creating an emFile Project for a PSoC 3 Application" on Page 5)
*   2. Build the USBFS_Bootloader project. Add dependency of the bootloadable 
*      project to the latest bootloader hex file. Build USBFS_Bootloadable 
*      project and program the Bootloader hex file on to the target device.
	3. Power your PSoC3 board.
*   4.If there is an SD Card formatted correctly,with the specified CYACD file,it 
*	  will get loaded,and you will see it execute,and "Hello World!" will be displayed.  
*
* Related Document:
*   PSoC Creator System Reference Guide
*
*******************************************************************************/

#include <device.h>


/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
*  The main function will be called from Bootloder after 2.5 sec delay and 
*  displays "BootloadableTest" message on the LCD.
*  This delay is set in DWR file in USBFS_Bootloder project.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void main()
{
    LCD_Start();
    LCD_PrintString("Hello World!");
    
    for(;;)
    {
    }
}

/* [] END OF FILE */
