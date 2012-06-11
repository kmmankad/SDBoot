/* SD Card Based Bootloader for PSoC3
 Author : Kartik Mankad
 Date : 11-Jun-2012
 This code is licensed as CC-BY-SA 3.0
 Description : The PSoC programs a cyacd file from an SD Card to its flash memory.
 
This file contains the call to the main function that will execute the SD bootload
operation
*/

/*******************************************************************************
* File Name: main.c
*
* Version: 1.0
*
* Description:
*   This project tries to load a CYACD file of the specified name from an
*	attached SD card,and then also enumerates as a USB HID Device to allow 
*	USB Bootloading as well.
*  To test bootloader:
*	1. Add the emFile dependencies for "emf32noslfn" configuration as explained
*	   in the emFile datasheet.(See "Creating an emFile Project for a PSoC 3 Application" on Page 5)
*   2. Build the USBFS_Bootloader project. Add dependency of the bootloadable 
*      project to the latest bootloader hex file. Build USBFS_Bootloadable 
*      project and program the Bootloader hex file on to the target device.
	3. Power your PSoC3 board.
*   4.If there is an SD Card formatted correctly(FAT32),with the specified CYACD file,it 
*	  will get loaded,and you will see it execute.
*
*******************************************************************************/

#include <device.h>
#include "cybtldr_parse.h"


/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
*  The main code starts boot loader.
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
	/*Attempt SD Card Bootload using the file "A.cyacd",expected in the root of the SD Card.*/
	CyBtldr_SD_Bootload("A.cyacd");
    
	/*Call USB HID Bootloader*/
    CyBtldr_Start();

    /* Code will never be executed here */    
    for(;;)
    {
    }
}

/* [] END OF FILE */
