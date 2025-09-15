/*
 * File: FATFS_SD.h
 * Driver Name: [[ FATFS_SD SPI ]]
 * SW Layer:   MIDWARE
 * Author:     Khaled Magdy
 * -------------------------------------------
 * For More Information, Tutorials, etc.
 * Visit Website: www.DeepBlueMbedded.com
 */
#ifndef FATFS_SD_H_
#define FATFS_SD_H_

//-----[ SD Card SPI Interface Cfgs ]-----

extern SPI_HandleTypeDef 	hspi1;
#define HSPI_SDCARD 		&hspi1
#define SD_CS_PORT 			GPIOA
#define SD_CS_PIN 			GPIO_PIN_4
#define SPI_TIMEOUT 		100

// SD Card Commands
#define CMD0    (0)         // GO_IDLE_STATE
#define CMD1    (1)         // SEND_OP_COND (MMC)
#define ACMD41  (0x80+41)   // SEND_OP_COND (SDC)
#define CMD8    (8)         // SEND_IF_COND
#define CMD9    (9)         // SEND_CSD
#define CMD10   (10)        // SEND_CID
#define CMD12   (12)        // STOP_TRANSMISSION
#define ACMD13  (0x80+13)   // SD_STATUS (SDC)
#define CMD16   (16)        // SET_BLOCKLEN
#define CMD17   (17)        // READ_SINGLE_BLOCK
#define CMD18   (18)        // READ_MULTIPLE_BLOCK
#define CMD23   (23)        // SET_BLOCK_COUNT (MMC)
#define ACMD23  (0x80+23)   // SET_WR_BLK_ERASE_COUNT (SDC)
#define CMD24   (24)        // WRITE_BLOCK
#define CMD25   (25)        // WRITE_MULTIPLE_BLOCK
#define CMD32   (32)        // ERASE_ER_BLK_START
#define CMD33   (33)        // ERASE_ER_BLK_END
#define CMD38   (38)        // ERASE
#define CMD55   (55)        // APP_CMD
#define CMD58   (58)        // READ_OCR

// Card type flags (CardType)
#define CT_MMC    0x01      // MMC ver 3
#define CT_SD1    0x02      // SD ver 1
#define CT_SD2    0x04      // SD ver 2
#define CT_SDC    (CT_SD1|CT_SD2) // SD
#define CT_BLOCK  0x08      // Block addressing

// SPI Definitions - Adjust these for your hardware
#define SD_SPI_HANDLE       hspi1
#define SD_CS_GPIO_Port     GPIOA
#define SD_CS_Pin           GPIO_PIN_4

// Function prototypes
DSTATUS SD_disk_initialize(BYTE pdrv);
DSTATUS SD_disk_status(BYTE pdrv);
DRESULT SD_disk_read(BYTE pdrv, BYTE* buff, DWORD sector, UINT count);
DRESULT SD_disk_write(BYTE pdrv, const BYTE* buff, DWORD sector, UINT count);
DRESULT SD_disk_ioctl(BYTE pdrv, BYTE cmd, void* buff);

#endif /* FATFS_SD_H_ */
