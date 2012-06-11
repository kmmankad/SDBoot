/* SD Card Based Bootloader for PSoC3
 Author : Kartik Mankad
 Date : 11-Jun-2012
 This code is licensed as CC-BY-SA 3.0
 Description : The PSoC programs a cyacd file from an SD Card to its flash memory.
 
 This file contains the necessary functions to parse data from the CYACD file,
 and write it to flash.
*/

#include <string.h>
#include "device.h"
#include "cybtldr_parse.h"
#include "FS.h"

/* CYRET_SUCCESS - Completed successfully - is already defined in other PSoC Creator generated files.
* So commentting out the redefenition */
/* #define CYRET_SUCCESS           0x00 */
/* File is not accessable */
#define CYRET_ERR_FILE          0x01
/* Reached the end of the file */
#define CYRET_ERR_EOF           0x02
/* The amount of data available is outside the expected range */
#define CYRET_ERR_LENGTH        0x03
/* The data is not of the proper form */
#define CYRET_ERR_DATA          0x04
/* The command is not recognized */
#define CYRET_ERR_CMD           0x05


#define CYBTLDR_FROW_SIZE           CYDEV_FLS_ROW_SIZE
uint8 flashBufferX[CYBTLDR_FROW_SIZE];
#define INIT_FLASH_WRITE  		(CYRET_SUCCESS == CySetTemp() && CYRET_SUCCESS == CySetFlashEEBuffer(flashBufferX))
#define CYBTLDR_SW_RESET          CY_SET_REG8(CYREG_RESET_CR2, 0x01)



/* Pointer to the *.cyacd file containing the data that is to be read */
FS_FILE * dataFile;

unsigned char CyBtldr_FromHex(char value)
{
    if ('0' <= value && value <= '9')
        return (unsigned char)(value - '0');
    if ('a' <= value && value <= 'f')
        return (unsigned char)(10 + value - 'a');
    if ('A' <= value && value <= 'F')
        return (unsigned char)(10 + value - 'A');
    return 0;
}

//expected that it is given the start of 
int CyBtldr_ReadLine(char* buffer)
{
    int err = CYRET_SUCCESS;
    unsigned int len = 0;

    if (NULL != dataFile && FS_FEof(dataFile)!=1){//End of File Not reached.
        if (FS_Read(dataFile,buffer,589)==589){ //Read the entire line. 
			//Send the file pointer ahead by 2,since /n /r are the 2 bytes after the last byte(the checksum).
			err=FS_FSeek(dataFile,2, FS_SEEK_CUR);
        } else {
            err = CYRET_ERR_EOF;
		}
    } else {
        err = CYRET_ERR_FILE;
	}

    return err;
}


int CyBtldr_ParseHeader(unsigned int bufSize, unsigned char* buffer, unsigned long* siliconId, unsigned char* siliconRev)
{
    const unsigned int LINE_LENGTH = 10; //8-silicon id, 2-silicon rev

    int err = CYRET_SUCCESS;

    if (bufSize == LINE_LENGTH)
    {
        *siliconId = (CyBtldr_FromHex(buffer[0]) << 28) | (CyBtldr_FromHex(buffer[1]) << 24) | 
            (CyBtldr_FromHex(buffer[2]) << 20) | (CyBtldr_FromHex(buffer[3]) << 16) | 
            (CyBtldr_FromHex(buffer[4]) << 12) | (CyBtldr_FromHex(buffer[5]) << 8) | 
            (CyBtldr_FromHex(buffer[6]) << 4) | CyBtldr_FromHex(buffer[7]);
        *siliconRev = (CyBtldr_FromHex(buffer[8]) << 4) | CyBtldr_FromHex(buffer[9]);
    }
    else
        err = CYRET_ERR_LENGTH;

    return err;
}

int CyBtldr_ParseRowData(uint16 bufSize, unsigned char* buffer, unsigned char* arrayId, uint16* rowNum, unsigned char* rowData, uint16* size, unsigned char* checksum)
{
    const unsigned short MIN_SIZE = 13; //1-ident, 2-array, 4-addr, 4-size, 2-checksum
    const int DATA_OFFSET = 11;

    unsigned int i, j;
    uint16 size2;
    int err = CYRET_SUCCESS;

    if (bufSize <= MIN_SIZE)
        err = CYRET_ERR_LENGTH;
    else if (buffer[0] == ':')
    {
        *arrayId = (CyBtldr_FromHex(buffer[1]) << 4) | CyBtldr_FromHex(buffer[2]);
        *rowNum = (CyBtldr_FromHex(buffer[3]) << 12) | (CyBtldr_FromHex(buffer[4]) << 8) |(CyBtldr_FromHex(buffer[5]) << 4) | CyBtldr_FromHex(buffer[6]);
      size2 = ((uint16)CyBtldr_FromHex(buffer[7]) << 12) | ((uint16)CyBtldr_FromHex(buffer[8]) << 8) |((uint16)CyBtldr_FromHex(buffer[9]) << 4) | ((uint16)CyBtldr_FromHex(buffer[10]));
        *size = size2;
        *checksum = (CyBtldr_FromHex(buffer[bufSize - 2]) << 4) | CyBtldr_FromHex(buffer[bufSize - 1]);

        if ((size2 * 2) == (unsigned short)(bufSize - MIN_SIZE))
        {
            for (i = 0; i < size2; i++)
            {
                j = DATA_OFFSET + (i * 2);
                rowData[i] = (CyBtldr_FromHex(buffer[j]) << 4) | CyBtldr_FromHex(buffer[j + 1]);
            }
        }
        else
            err = CYRET_ERR_DATA;
    }
    else
        err = CYRET_ERR_CMD;
	buffer=NULL;
    return err;
}


int CyBtldr_SD_Bootload(char * file){
	char cyacd_header[10];
	char cyacd_line[589];
	unsigned char cyacd_arrayId;
	uint16 cyacd_rowAddress;
	char cyacd_rowData[288];
	uint16 cyacd_rowSize;
	unsigned char cyacd_checksum;
	unsigned long siliconId;
	unsigned char siliconRev;
	
	int err=CYRET_SUCCESS;
	int Flash_err=CYRET_SUCCESS;
	
	/*Initialize Flash Write Mechanism*/
	if(!INIT_FLASH_WRITE){
	/*If the Flash Write wasnt initialized successfully,
	fire a software reset and hope things will work out.*/
	CYBTLDR_SW_RESET;
	}
	
	/*Initialize the emFile FS*/
	FS_Init();
	
	/*Open the File for reading */
	dataFile = FS_FOpen(file, "r");
	
	/*File Opened*/
	if (NULL !=  dataFile && FS_FEof(dataFile)!=1){
		err=FS_Read(dataFile,cyacd_header,10);
		/* Read the Header line from the CYACD file.*/
        
		if (err!=0){ 
        /* Send the file pointer ahead by 4,setting it to the start of the first row */
		err=FS_FSeek( dataFile,4, FS_SEEK_CUR);
		
		/*Parse the header for SiliconID and SiliconRev*/
		err=CyBtldr_ParseHeader(10,cyacd_header,&siliconId,&siliconRev);
		
		/*Heres where you should check the SiliconID and Rev.*/
		/*
		if(CYSWAP_ENDIAN32(CYDEV_CHIP_JTAG_ID)==siliconID && CYDEV_CHIP_REV_EXPECT==siliconRev){
		}else{
		}*/
		
	}else{
		/*We have EOF or a NULL dataFile FS structure.*/
       	err = CYRET_ERR_EOF;
		return err;
	}
		
		/*Lets get to the real stuff.*/
		while(1){
		
		/*Read the First Line after the header.*/
		err=CyBtldr_ReadLine(cyacd_line);
		
		/*Check if the line was read successfully.*/
		if(err!=CYRET_SUCCESS){
			break;
		}
		
		/*Parse the line to get Row Address,Row Data,Row Size
		and the Checksum Byte*/
		err=CyBtldr_ParseRowData(589,cyacd_line,&cyacd_arrayId,&cyacd_rowAddress,&cyacd_rowData,&cyacd_rowSize,&cyacd_checksum);
		
		/*Check if the data was parsed successfully.*/
		if(err!=CYRET_SUCCESS){
			break;
		}
		
		/*Write the Row Data to flash*/
		CyWriteRowFull(cyacd_arrayId,cyacd_rowAddress,cyacd_rowData,cyacd_rowSize);
		}
		
		
		/*Close the File*/
		err=FS_FClose(dataFile);
		
		/*Fire a software reset.*/
		CYBTLDR_SW_RESET;
		
	return err;
	}
return err;
}

/* [] END OF FILE */