/* SD Card Based Bootloader for PSoC3
 Author : Kartik Mankad
 Date : 11-Jun-2012
 This code is licensed as CC-BY-SA 3.0
 Description : The PSoC programs a cyacd file from an SD Card to its flash memory.
 
 This file contains the necessary functions to parse data from the CYACD file,
 and write it to flash.
*/


/* Maximum number of bytes to allocate for a single row.  */
/* NB: Rows should have a max of 592 chars (2-arrayID, 4-rowNum, 4-len, 576-data, 2-checksum, 4-newline) */
#define MAX_BUFFER_SIZE 768

/*******************************************************************************
* Function Name: CyBtldr_FromHex
********************************************************************************
* Summary:
*   Converts the provided ASCII char into its hexadecimal numerical equivilant.
*
* Parameters:
*   value - the ASCII char to convert into a number
*
* Returns:
*   The hexadecimal numerical equivilant of the provided ASCII char.  If the
*   provided char is not a valid ASCII char, it will return 0.
*
*******************************************************************************/
unsigned char CyBtldr_FromHex(char value);

/*******************************************************************************
* Function Name: CyBtldr_ParseHeader
********************************************************************************
* Summary:
*   Parses the header information from the *.cyacd file.  The header information
*   is stored as the first line, so this method should only be called once, 
*   and only immediatly after calling OpenDataFile and reading the first line.
*
* Parameters:
*   bufSize    - The number of bytes contained within buffer
*   buffer     - The buffer containing the header data to parse
*   siliconId  - The silicon ID that the provided *.cyacd file is for
*   siliconRev - The silicon Revision that the provided *.cyacd file is for
*   chksum     - The type of checksum to use for packet integrety check
*
* Returns:
*   CYRET_SUCCESS    - The file was opened successfully.
*   CYRET_ERR_LENGTH - The line does not contain enough data
*
*******************************************************************************/

extern int CyBtldr_ParseHeader(unsigned int bufSize, unsigned char* buffer, unsigned long* siliconId, unsigned char* siliconRev);

/*******************************************************************************
* Function Name: CyBtldr_ParseRowData
********************************************************************************
* Summary:
*   Parses the contents of the provided buffer which is expected to contain
*   the row data from the *.cyacd file.  This is expected to be called multiple
*   times.  Once for each row of the *.cyacd file, excluding the header row.
*
* Parameters:
*   bufSize  - The number of bytes contained within buffer
*   buffer   - The buffer containing the row data to parse
*   arrayId  - The flash array that the row of data belongs in
*   rowNum   - The flash row number that the data corresponds to
*   rowData  - The preallocated buffer to store the flash row data
*   size     - The number of bytes of rowData
*   checksum - The checksum value for the entire row (rowNum, size, rowData)
*
* Returns:
*   CYRET_SUCCESS    - The file was opened successfully.
*   CYRET_ERR_LENGTH - The line does not contain enough data
*   CYRET_ERR_DATA   - The line does not contain a full row of data
*   CYRET_ERR_CMD    - The line does not start with the cmd identifier ':'
*
*******************************************************************************/
extern int CyBtldr_ParseRowData(uint16 bufSize, unsigned char* buffer, unsigned char* arrayId, uint16* rowNum, unsigned char* rowData, uint16* size, unsigned char* checksum);

/*******************************************************************************
* Function Name:  CyBtldr_ReadLine
********************************************************************************
* Summary:
*   Reads a line of data from the CYACD file.
*
* Parameters:
*   buffer   - The buffer into which the line will be written.
*
* Returns:
*   CYRET_SUCCESS	- The line was successfully read from the file into the buffer.
*   CYRET_ERR_EOF	- End of File Reached.
*   CYRET_ERR_FILE	- An error in reading data from the file,due to a null FS_FILE structure.
*
*******************************************************************************/
extern int CyBtldr_ReadLine(char *buffer);

/*******************************************************************************
* Function Name: CyBtldr_SD_Bootload
********************************************************************************
* Summary:
*   Does the actual operation of reading the data,parsing it through 
*	the above function and writes it to flash,after which it fires a
*	software reset.Call this to execute an SD Bootload.
*
* Parameters:
*   file - A buffer that contains the name of the CYACD file
*
* Returns:
*   The hexadecimal numerical equivilant of the provided ASCII char.  If the
*   provided char is not a valid ASCII char, it will return 0.
*
*******************************************************************************/
extern int CyBtldr_SD_Bootload(char *file);


/* [] END OF FILE */

