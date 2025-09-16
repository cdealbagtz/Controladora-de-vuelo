/*
 * File: FATFS_SD.c
 * Driver Name: [[ FATFS_SD SPI ]]
 * SW Layer:   MIDWARE
 * Author:     Khaled Magdy
 * -------------------------------------------
 * For More Information, Tutorials, etc.
 * Visit Website: www.DeepBlueMbedded.com
 */
#include "main.h"
#include "diskio.h"
#include "FATFS_SD.h"
#include <string.h>

#define TRUE  1
#define FALSE 0
#define bool BYTE

static volatile DSTATUS Stat = STA_NOINIT;  // Physical drive status
static volatile UINT CardType;              // Card type flags

//-----[ SPI Functions ]-----

// Private functions
static void SELECT(void) {
    HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_RESET);
}

static void DESELECT(void) {
    HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_SET);
}

static void SPI_TxByte(BYTE data) {
    while(HAL_SPI_GetState(&SD_SPI_HANDLE) != HAL_SPI_STATE_READY);
    HAL_SPI_Transmit(&SD_SPI_HANDLE, &data, 1, HAL_MAX_DELAY);
}

static BYTE SPI_RxByte(void) {
    BYTE dummy = 0xFF, data = 0;
    while(HAL_SPI_GetState(&SD_SPI_HANDLE) != HAL_SPI_STATE_READY);
    HAL_SPI_TransmitReceive(&SD_SPI_HANDLE, &dummy, &data, 1, HAL_MAX_DELAY);
    return data;
}

static void SPI_RxBytePtr(BYTE *buff) {
    *buff = SPI_RxByte();
}

static BYTE SD_ReadyWait(void) {
    BYTE res;
    // Wait for ready in timeout of 500ms
    UINT timeout = 500;
    SPI_RxByte();
    do {
        res = SPI_RxByte();
    } while ((res != 0xFF) && timeout--);

    return res;
}

static void SD_PowerOn(void) {
    BYTE args[10];
    SELECT();
    for(int i = 0; i < 10; i++) {
        SPI_TxByte(0xFF);
    }
    DESELECT();
    SPI_TxByte(0xFF);
}

static void SD_PowerOff(void) {
    SELECT();
    SD_ReadyWait();
    DESELECT();
    SPI_TxByte(0xFF);
}

static BYTE SD_CheckPower(void) {
    return 0; // Dummy implementation
}

static bool SD_RxDataBlock(BYTE *buff, UINT btr) {
    BYTE token;
    UINT timeout = 200; // 200ms timeout

    do {
        token = SPI_RxByte();
    } while((token == 0xFF) && timeout--);

    if(token != 0xFE) return FALSE; // Invalid data token

    do {
        SPI_RxBytePtr(buff++);
        SPI_RxBytePtr(buff++);
    } while(btr -= 2);

    SPI_RxByte(); // Discard CRC
    SPI_RxByte();
    return TRUE;
}

static bool SD_TxDataBlock(const BYTE *buff, BYTE token) {
    BYTE resp;
    BYTE wc;

    if(SD_ReadyWait() != 0xFF) return FALSE;

    SPI_TxByte(token); // Send token
    if(token != 0xFD) { // Send data if token is other than StopTran
        wc = 0;
        do {
            SPI_TxByte(*buff++);
            SPI_TxByte(*buff++);
        } while(--wc);

        SPI_TxByte(0xFF); // CRC
        SPI_TxByte(0xFF);

        resp = SPI_RxByte(); // Receive data resp
        if((resp & 0x1F) != 0x05) // Function fails if the data packet was not accepted
            return FALSE;
    }
    return TRUE;
}

static BYTE SD_SendCmd(BYTE cmd, DWORD arg) {
    BYTE n, res;

    if(cmd & 0x80) { // Send a CMD55 prior to ACMD<n>
        cmd &= 0x7F;
        res = SD_SendCmd(CMD55, 0);
        if(res > 1) return res;
    }

    // Select the card and wait for ready except to stop multiple block read
    if(cmd != CMD12) {
        DESELECT();
        SELECT();
        if(SD_ReadyWait() != 0xFF) return 0xFF;
    }

    // Send command packet
    SPI_TxByte(0x40 | cmd);         // Start + command index
    SPI_TxByte((BYTE)(arg >> 24));  // Argument[31..24]
    SPI_TxByte((BYTE)(arg >> 16));  // Argument[23..16]
    SPI_TxByte((BYTE)(arg >> 8));   // Argument[15..8]
    SPI_TxByte((BYTE)arg);          // Argument[7..0]

    n = 0x01; // Dummy CRC + Stop
    if(cmd == CMD0) n = 0x95; // Valid CRC for CMD0(0)
    if(cmd == CMD8) n = 0x87; // Valid CRC for CMD8(0x1AA)
    SPI_TxByte(n);

    // Receive command resp
    if(cmd == CMD12) SPI_RxByte(); // Discard following one byte when CMD12

    n = 10; // Wait for response (10 bytes max)
    do {
        res = SPI_RxByte();
    } while((res & 0x80) && --n);

    return res;
}

/*--------------------------------------------------------------------------

   Public Functions

---------------------------------------------------------------------------*/

DSTATUS SD_disk_initialize(BYTE pdrv) {
    BYTE n, cmd, ty, ocr[4];

    if(pdrv) return STA_NOINIT; // Supports only drive 0

    SD_PowerOn(); // Initialize SPI

    SELECT();
    ty = 0;
    if(SD_SendCmd(CMD0, 0) == 1) { // Put the card SPI/Idle state
        UINT timeout = 1000; // Initialization timeout = 1 sec
        if(SD_SendCmd(CMD8, 0x1AA) == 1) { // SDv2?
            // Get 32 bit return value of R7 resp
            for(n = 0; n < 4; n++) ocr[n] = SPI_RxByte();
            if(ocr[2] == 0x01 && ocr[3] == 0xAA) { // Does the card support vcc of 2.7-3.6V?
                // Wait for end of initialization with ACMD41(HCS)
                while(timeout-- && SD_SendCmd(ACMD41, 1UL << 30));
                if(timeout && SD_SendCmd(CMD58, 0) == 0) { // Check CCS bit in the OCR
                    for(n = 0; n < 4; n++) ocr[n] = SPI_RxByte();
                    ty = (ocr[0] & 0x40) ? CT_SD2 | CT_BLOCK : CT_SD2; // Card id SDv2
                }
            }
        } else { // Not SDv2 card
            if(SD_SendCmd(ACMD41, 0) <= 1) { // SDv1 or MMC?
                ty = CT_SD1; cmd = ACMD41; // SDv1 (ACMD41(0))
            } else {
                ty = CT_MMC; cmd = CMD1; // MMCv3 (CMD1(0))
            }
            // Wait for end of initialization
            while(timeout-- && SD_SendCmd(cmd, 0));
            if(!timeout || SD_SendCmd(CMD16, 512) != 0) // Set block length: 512
                ty = 0;
        }
    }
    CardType = ty; // Card type
    DESELECT();
    SPI_RxByte(); // Idle (Release DO)

    if(ty) { // OK
        Stat &= ~STA_NOINIT; // Clear STA_NOINIT flag
    } else { // Failed
        SD_PowerOff();
    }

    return Stat;
}

DSTATUS SD_disk_status(BYTE pdrv) {
    if(pdrv) return STA_NOINIT; // Supports only drive 0
    return Stat; // Return disk status
}

DRESULT SD_disk_read(BYTE pdrv, BYTE* buff, DWORD sector, UINT count) {
    if(pdrv || !count) return RES_PARERR; // Check parameter
    if(Stat & STA_NOINIT) return RES_NOTRDY; // Check if drive is ready

    if(!(CardType & CT_BLOCK)) sector *= 512; // LBA ot BA conversion (byte addressing cards)

    SELECT();

    if(count == 1) { // Single sector read
        if((SD_SendCmd(CMD17, sector) == 0) && SD_RxDataBlock(buff, 512)) {
            count = 0;
        }
    } else { // Multiple sector read
        if(SD_SendCmd(CMD18, sector) == 0) {
            do {
                if(!SD_RxDataBlock(buff, 512)) break;
                buff += 512;
            } while(--count);
            SD_SendCmd(CMD12, 0); // STOP_TRANSMISSION
        }
    }

    DESELECT();
    SPI_RxByte(); // Idle (Release DO)

    return count ? RES_ERROR : RES_OK; // Return result
}

DRESULT SD_disk_write(BYTE pdrv, const BYTE* buff, DWORD sector, UINT count) {
    if(pdrv || !count) return RES_PARERR; // Check parameter
    if(Stat & STA_NOINIT) return RES_NOTRDY; // Check drive status
    if(Stat & STA_PROTECT) return RES_WRPRT; // Check write protect

    if(!(CardType & CT_BLOCK)) sector *= 512; // LBA ==> BA conversion (byte addressing cards)

    SELECT();

    if(count == 1) { // Single sector write
        if((SD_SendCmd(CMD24, sector) == 0) && SD_TxDataBlock(buff, 0xFE)) {
            count = 0;
        }
    } else { // Multiple sector write
        if(CardType & CT_SDC) SD_SendCmd(ACMD23, count); // Predefine number of sectors
        if(SD_SendCmd(CMD25, sector) == 0) {
            do {
                if(!SD_TxDataBlock(buff, 0xFC)) break;
                buff += 512;
            } while(--count);
            if(!SD_TxDataBlock(0, 0xFD)) count = 1; // STOP_TRAN token
        }
    }

    DESELECT();
    SPI_RxByte(); // Idle (Release DO)

    return count ? RES_ERROR : RES_OK; // Return result
}

DRESULT SD_disk_ioctl(BYTE pdrv, BYTE cmd, void* buff) {
    DRESULT res;
    BYTE n, csd[16];
    DWORD *dp, st, ed, csize;

    if(pdrv) return RES_PARERR; // Check parameter
    if(Stat & STA_NOINIT) return RES_NOTRDY; // Check if drive is ready

    SELECT();

    res = RES_ERROR;
    switch(cmd) {
        case CTRL_SYNC: // Wait for end of internal write process of the drive
            if(SD_ReadyWait() == 0xFF) res = RES_OK;
            break;

        case GET_SECTOR_COUNT: // Get drive capacity in unit of sector (DWORD)
            if((SD_SendCmd(CMD9, 0) == 0) && SD_RxDataBlock(csd, 16)) {
                if((csd[0] >> 6) == 1) { // SDC ver 2.00
                    csize = csd[9] + ((WORD)csd[8] << 8) + ((DWORD)(csd[7] & 63) << 16) + 1;
                    *(DWORD*)buff = csize << 10;
                } else { // SDC ver 1.XX or MMC ver 3
                    n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
                    csize = (csd[8] >> 6) + ((WORD)csd[7] << 2) + ((WORD)(csd[6] & 3) << 10) + 1;
                    *(DWORD*)buff = csize << (n - 9);
                }
                res = RES_OK;
            }
            break;

        case GET_BLOCK_SIZE: // Get erase block size in unit of sector (DWORD)
            *(DWORD*)buff = 128; // 128 sectors
            res = RES_OK;
            break;

        default:
            res = RES_PARERR;
    }

    DESELECT();
    SPI_RxByte(); // Idle (Release DO)

    return res;
}
