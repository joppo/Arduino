/*
 * This program will format an SD or SDHC card.
 * Warning all data will be deleted!
 *
 * For SD/SDHC cards larger than 64 MB this
 * program attempts to match the format
 * generated by SDFormatter available here:
 *
 * http://www.sdcard.org/consumers/formatter/
 *
 * For smaller cards this program uses FAT16
 * and SDFormatter uses FAT12.
 */

// Set USE_SDIO to zero for SPI card access. 
#define USE_SDIO 0
//
// Change the value of chipSelect if your hardware does
// not use the default value, SS.  Common values are:
// Arduino Ethernet shield: pin 4
// Sparkfun SD shield: pin 8
// Adafruit SD shields and modules: pin 10
const uint8_t chipSelect = 10;

// Initialize at highest supported speed not over 50 MHz.
// Reduce max speed if errors occur.
#define SPI_SPEED SD_SCK_MHZ(4)

// Print extra info for debug if DEBUG_PRINT is nonzero
#define DEBUG_PRINT 0
#include <SPI.h>
#include "SdFat.h"
#if DEBUG_PRINT
#include "FreeStack.h"
#endif  // DEBUG_PRINT

// Serial output stream
ArduinoOutStream cout(Serial);

#if USE_SDIO
// Use faster SdioCardEX
SdioCardEX card;
// SdioCard card;
#else  // USE_SDIO
Sd2Card card;
#endif  // USE_SDIO
 
uint32_t cardSizeBlocks;
uint32_t cardCapacityMB;

// cache for SD block
cache_t cache;

// MBR information
uint8_t partType;
uint32_t relSector;
uint32_t partSize;

// Fake disk geometry
uint8_t numberOfHeads;
uint8_t sectorsPerTrack;

// FAT parameters
uint16_t reservedSectors;
uint8_t sectorsPerCluster;
uint32_t fatStart;
uint32_t fatSize;
uint32_t dataStart;

// constants for file system structure
uint16_t const BU16 = 128;
uint16_t const BU32 = 8192;

//  strings needed in file system structures
char noName[] = "NO NAME    ";
char fat16str[] = "FAT16   ";
char fat32str[] = "FAT32   ";
//------------------------------------------------------------------------------
#define sdError(msg) {cout << F("error: ") << F(msg) << endl; sdErrorHalt();}
//------------------------------------------------------------------------------
void sdErrorHalt() {
  if (card.errorCode()) {
    cout << F("SD error: ") << hex << int(card.errorCode());
    cout << ',' << int(card.errorData()) << dec << endl;
  }
  SysCall::halt();
}
//------------------------------------------------------------------------------
#if DEBUG_PRINT
void debugPrint() {
  cout << F("FreeStack: ") << FreeStack() << endl;
  cout << F("partStart: ") << relSector << endl;
  cout << F("partSize: ") << partSize << endl;
  cout << F("reserved: ") << reservedSectors << endl;
  cout << F("fatStart: ") << fatStart << endl;
  cout << F("fatSize: ") << fatSize << endl;
  cout << F("dataStart: ") << dataStart << endl;
  cout << F("clusterCount: ");
  cout << ((relSector + partSize - dataStart)/sectorsPerCluster) << endl;
  cout << endl;
  cout << F("Heads: ") << int(numberOfHeads) << endl;
  cout << F("Sectors: ") << int(sectorsPerTrack) << endl;
  cout << F("Cylinders: ");
  cout << cardSizeBlocks/(numberOfHeads*sectorsPerTrack) << endl;
}
#endif  // DEBUG_PRINT
//------------------------------------------------------------------------------
// write cached block to the card
uint8_t writeCache(uint32_t lbn) {
  return card.writeBlock(lbn, cache.data);
}
//------------------------------------------------------------------------------
// initialize appropriate sizes for SD capacity
void initSizes() {
  if (cardCapacityMB <= 6) {
    sdError("Card is too small.");
  } else if (cardCapacityMB <= 16) {
    sectorsPerCluster = 2;
  } else if (cardCapacityMB <= 32) {
    sectorsPerCluster = 4;
  } else if (cardCapacityMB <= 64) {
    sectorsPerCluster = 8;
  } else if (cardCapacityMB <= 128) {
    sectorsPerCluster = 16;
  } else if (cardCapacityMB <= 1024) {
    sectorsPerCluster = 32;
  } else if (cardCapacityMB <= 32768) {
    sectorsPerCluster = 64;
  } else {
    // SDXC cards
    sectorsPerCluster = 128;
  }

  cout << F("Blocks/Cluster: ") << int(sectorsPerCluster) << endl;
  // set fake disk geometry
  sectorsPerTrack = cardCapacityMB <= 256 ? 32 : 63;

  if (cardCapacityMB <= 16) {
    numberOfHeads = 2;
  } else if (cardCapacityMB <= 32) {
    numberOfHeads = 4;
  } else if (cardCapacityMB <= 128) {
    numberOfHeads = 8;
  } else if (cardCapacityMB <= 504) {
    numberOfHeads = 16;
  } else if (cardCapacityMB <= 1008) {
    numberOfHeads = 32;
  } else if (cardCapacityMB <= 2016) {
    numberOfHeads = 64;
  } else if (cardCapacityMB <= 4032) {
    numberOfHeads = 128;
  } else {
    numberOfHeads = 255;
  }
}
//------------------------------------------------------------------------------
// zero cache and optionally set the sector signature
void clearCache(uint8_t addSig) {
  memset(&cache, 0, sizeof(cache));
  if (addSig) {
    cache.mbr.mbrSig0 = BOOTSIG0;
    cache.mbr.mbrSig1 = BOOTSIG1;
  }
}
//------------------------------------------------------------------------------
// zero FAT and root dir area on SD
void clearFatDir(uint32_t bgn, uint32_t count) {
  clearCache(false);
#if USE_SDIO
  for (uint32_t i = 0; i < count; i++) {
    if (!card.writeBlock(bgn + i, cache.data)) {
       sdError("Clear FAT/DIR writeBlock failed");
    }     
    if ((i & 0XFF) == 0) {
      cout << '.';
    }    
  }
#else  // USE_SDIO
  if (!card.writeStart(bgn, count)) {
    sdError("Clear FAT/DIR writeStart failed");
  }
  for (uint32_t i = 0; i < count; i++) {
    if ((i & 0XFF) == 0) {
      cout << '.';
    }
    if (!card.writeData(cache.data)) {
      sdError("Clear FAT/DIR writeData failed");
    }
  }
  if (!card.writeStop()) {
    sdError("Clear FAT/DIR writeStop failed");
  }
#endif  // USE_SDIO
  cout << endl;
}
//------------------------------------------------------------------------------
// return cylinder number for a logical block number
uint16_t lbnToCylinder(uint32_t lbn) {
  return lbn / (numberOfHeads * sectorsPerTrack);
}
//------------------------------------------------------------------------------
// return head number for a logical block number
uint8_t lbnToHead(uint32_t lbn) {
  return (lbn % (numberOfHeads * sectorsPerTrack)) / sectorsPerTrack;
}
//------------------------------------------------------------------------------
// return sector number for a logical block number
uint8_t lbnToSector(uint32_t lbn) {
  return (lbn % sectorsPerTrack) + 1;
}
//------------------------------------------------------------------------------
// format and write the Master Boot Record
void writeMbr() {
  clearCache(true);
  part_t* p = cache.mbr.part;
  p->boot = 0;
  uint16_t c = lbnToCylinder(relSector);
  if (c > 1023) {
    sdError("MBR CHS");
  }
  p->beginCylinderHigh = c >> 8;
  p->beginCylinderLow = c & 0XFF;
  p->beginHead = lbnToHead(relSector);
  p->beginSector = lbnToSector(relSector);
  p->type = partType;
  uint32_t endLbn = relSector + partSize - 1;
  c = lbnToCylinder(endLbn);
  if (c <= 1023) {
    p->endCylinderHigh = c >> 8;
    p->endCylinderLow = c & 0XFF;
    p->endHead = lbnToHead(endLbn);
    p->endSector = lbnToSector(endLbn);
  } else {
    // Too big flag, c = 1023, h = 254, s = 63
    p->endCylinderHigh = 3;
    p->endCylinderLow = 255;
    p->endHead = 254;
    p->endSector = 63;
  }
  p->firstSector = relSector;
  p->totalSectors = partSize;
  if (!writeCache(0)) {
    sdError("write MBR");
  }
}
//------------------------------------------------------------------------------
// generate serial number from card size and micros since boot
uint32_t volSerialNumber() {
  return (cardSizeBlocks << 8) + micros();
}
//------------------------------------------------------------------------------
// format the SD as FAT16
void makeFat16() {
  uint32_t nc;
  for (dataStart = 2 * BU16;; dataStart += BU16) {
    nc = (cardSizeBlocks - dataStart)/sectorsPerCluster;
    fatSize = (nc + 2 + 255)/256;
    uint32_t r = BU16 + 1 + 2 * fatSize + 32;
    if (dataStart < r) {
      continue;
    }
    relSector = dataStart - r + BU16;
    break;
  }
  // check valid cluster count for FAT16 volume
  if (nc < 4085 || nc >= 65525) {
    sdError("Bad cluster count");
  }
  reservedSectors = 1;
  fatStart = relSector + reservedSectors;
  partSize = nc * sectorsPerCluster + 2 * fatSize + reservedSectors + 32;
  if (partSize < 32680) {
    partType = 0X01;
  } else if (partSize < 65536) {
    partType = 0X04;
  } else {
    partType = 0X06;
  }
  // write MBR
  writeMbr();
  clearCache(true);
  fat_boot_t* pb = &cache.fbs;
  pb->jump[0] = 0XEB;
  pb->jump[1] = 0X00;
  pb->jump[2] = 0X90;
  for (uint8_t i = 0; i < sizeof(pb->oemId); i++) {
    pb->oemId[i] = ' ';
  }
  pb->bytesPerSector = 512;
  pb->sectorsPerCluster = sectorsPerCluster;
  pb->reservedSectorCount = reservedSectors;
  pb->fatCount = 2;
  pb->rootDirEntryCount = 512;
  pb->mediaType = 0XF8;
  pb->sectorsPerFat16 = fatSize;
  pb->sectorsPerTrack = sectorsPerTrack;
  pb->headCount = numberOfHeads;
  pb->hidddenSectors = relSector;
  pb->totalSectors32 = partSize;
  pb->driveNumber = 0X80;
  pb->bootSignature = EXTENDED_BOOT_SIG;
  pb->volumeSerialNumber = volSerialNumber();
  memcpy(pb->volumeLabel, noName, sizeof(pb->volumeLabel));
  memcpy(pb->fileSystemType, fat16str, sizeof(pb->fileSystemType));
  // write partition boot sector
  if (!writeCache(relSector)) {
    sdError("FAT16 write PBS failed");
  }
  // clear FAT and root directory
  clearFatDir(fatStart, dataStart - fatStart);
  clearCache(false);
  cache.fat16[0] = 0XFFF8;
  cache.fat16[1] = 0XFFFF;
  // write first block of FAT and backup for reserved clusters
  if (!writeCache(fatStart)
      || !writeCache(fatStart + fatSize)) {
    sdError("FAT16 reserve failed");
  }
}
//------------------------------------------------------------------------------
// format the SD as FAT32
void makeFat32() {
  uint32_t nc;
  relSector = BU32;
  for (dataStart = 2 * BU32;; dataStart += BU32) {
    nc = (cardSizeBlocks - dataStart)/sectorsPerCluster;
    fatSize = (nc + 2 + 127)/128;
    uint32_t r = relSector + 9 + 2 * fatSize;
    if (dataStart >= r) {
      break;
    }
  }
  // error if too few clusters in FAT32 volume
  if (nc < 65525) {
    sdError("Bad cluster count");
  }
  reservedSectors = dataStart - relSector - 2 * fatSize;
  fatStart = relSector + reservedSectors;
  partSize = nc * sectorsPerCluster + dataStart - relSector;
  // type depends on address of end sector
  // max CHS has lbn = 16450560 = 1024*255*63
  if ((relSector + partSize) <= 16450560) {
    // FAT32
    partType = 0X0B;
  } else {
    // FAT32 with INT 13
    partType = 0X0C;
  }
  writeMbr();
  clearCache(true);

  fat32_boot_t* pb = &cache.fbs32;
  pb->jump[0] = 0XEB;
  pb->jump[1] = 0X00;
  pb->jump[2] = 0X90;
  for (uint8_t i = 0; i < sizeof(pb->oemId); i++) {
    pb->oemId[i] = ' ';
  }
  pb->bytesPerSector = 512;
  pb->sectorsPerCluster = sectorsPerCluster;
  pb->reservedSectorCount = reservedSectors;
  pb->fatCount = 2;
  pb->mediaType = 0XF8;
  pb->sectorsPerTrack = sectorsPerTrack;
  pb->headCount = numberOfHeads;
  pb->hidddenSectors = relSector;
  pb->totalSectors32 = partSize;
  pb->sectorsPerFat32 = fatSize;
  pb->fat32RootCluster = 2;
  pb->fat32FSInfo = 1;
  pb->fat32BackBootBlock = 6;
  pb->driveNumber = 0X80;
  pb->bootSignature = EXTENDED_BOOT_SIG;
  pb->volumeSerialNumber = volSerialNumber();
  memcpy(pb->volumeLabel, noName, sizeof(pb->volumeLabel));
  memcpy(pb->fileSystemType, fat32str, sizeof(pb->fileSystemType));
  // write partition boot sector and backup
  if (!writeCache(relSector)
      || !writeCache(relSector + 6)) {
    sdError("FAT32 write PBS failed");
  }
  clearCache(true);
  // write extra boot area and backup
  if (!writeCache(relSector + 2)
      || !writeCache(relSector + 8)) {
    sdError("FAT32 PBS ext failed");
  }
  fat32_fsinfo_t* pf = &cache.fsinfo;
  pf->leadSignature = FSINFO_LEAD_SIG;
  pf->structSignature = FSINFO_STRUCT_SIG;
  pf->freeCount = 0XFFFFFFFF;
  pf->nextFree = 0XFFFFFFFF;
  // write FSINFO sector and backup
  if (!writeCache(relSector + 1)
      || !writeCache(relSector + 7)) {
    sdError("FAT32 FSINFO failed");
  }
  clearFatDir(fatStart, 2 * fatSize + sectorsPerCluster);
  clearCache(false);
  cache.fat32[0] = 0x0FFFFFF8;
  cache.fat32[1] = 0x0FFFFFFF;
  cache.fat32[2] = 0x0FFFFFFF;
  // write first block of FAT and backup for reserved clusters
  if (!writeCache(fatStart)
      || !writeCache(fatStart + fatSize)) {
    sdError("FAT32 reserve failed");
  }
}
//------------------------------------------------------------------------------
// flash erase all data
uint32_t const ERASE_SIZE = 262144L;
void eraseCard() {
  cout << endl << F("Erasing\n");
  uint32_t firstBlock = 0;
  uint32_t lastBlock;
  uint16_t n = 0;

  do {
    lastBlock = firstBlock + ERASE_SIZE - 1;
    if (lastBlock >= cardSizeBlocks) {
      lastBlock = cardSizeBlocks - 1;
    }
    if (!card.erase(firstBlock, lastBlock)) {
      sdError("erase failed");
    }
    cout << '.';
    if ((n++)%32 == 31) {
      cout << endl;
    }
    firstBlock += ERASE_SIZE;
  } while (firstBlock < cardSizeBlocks);
  cout << endl;

  if (!card.readBlock(0, cache.data)) {
    sdError("readBlock");
  }
  cout << hex << showbase << setfill('0') << internal;
  cout << F("All data set to ") << setw(4) << int(cache.data[0]) << endl;
  cout << dec << noshowbase << setfill(' ') << right;
  cout << F("Erase done\n");
}
//------------------------------------------------------------------------------
void formatCard() {
  cout << endl;
  cout << F("Formatting\n");
  initSizes();
  if (card.type() != SD_CARD_TYPE_SDHC) {
    cout << F("FAT16\n");
    makeFat16();
  } else {
    cout << F("FAT32\n");
    makeFat32();
  }
#if DEBUG_PRINT
  debugPrint();
#endif  // DEBUG_PRINT
  cout << F("Format done\n");
}
//------------------------------------------------------------------------------
void setup() {
  char c;
  Serial.begin(9600);
  // Wait for USB Serial 
  while (!Serial) {
    SysCall::yield();
  }
  cout << F("Type any character to start\n");
  while (!Serial.available()) {
    SysCall::yield();
  }
  // Discard any extra characters.
  do {
    delay(10);
  } while (Serial.available() && Serial.read() >= 0);
  cout << F(
         "\n"
         "This program can erase and/or format SD/SDHC cards.\n"
         "\n"
         "Erase uses the card's fast flash erase command.\n"
         "Flash erase sets all data to 0X00 for most cards\n"
         "and 0XFF for a few vendor's cards.\n"
         "\n"
         "Cards larger than 2 GB will be formatted FAT32 and\n"
         "smaller cards will be formatted FAT16.\n"
         "\n"
         "Warning, all data on the card will be erased.\n"
         "Enter 'Y' to continue: ");
  while (!Serial.available()) {
    SysCall::yield();
  }

  c = Serial.read();
  cout << c << endl;
  if (c != 'Y') {
    cout << F("Quiting, you did not enter 'Y'.\n");
    return;
  }
  // Read any existing Serial data.
  do {
    delay(10);
  } while (Serial.available() && Serial.read() >= 0);

  cout << F(
         "\n"
         "Options are:\n"
         "E - erase the card and skip formatting.\n"
         "F - erase and then format the card. (recommended)\n"
         "Q - quick format the card without erase.\n"
         "\n"
         "Enter option: ");

  while (!Serial.available()) {
    SysCall::yield();
  }
  c = Serial.read();
  cout << c << endl;
  if (!strchr("EFQ", c)) {
    cout << F("Quiting, invalid option entered.") << endl;
    return;
  }
#if USE_SDIO
  if (!card.begin()) {
    sdError("card.begin failed");  
  }
#else  // USE_SDIO
  if (!card.begin(chipSelect, SPI_SPEED)) {
    cout << F(
           "\nSD initialization failure!\n"
           "Is the SD card inserted correctly?\n"
           "Is chip select correct at the top of this program?\n");
    sdError("card.begin failed");
  }
#endif  
  cardSizeBlocks = card.cardSize();
  if (cardSizeBlocks == 0) {
    sdError("cardSize");
  }
  cardCapacityMB = (cardSizeBlocks + 2047)/2048;

  cout << F("Card Size: ") << setprecision(0) << 1.048576*cardCapacityMB;
  cout << F(" MB, (MB = 1,000,000 bytes)") << endl;

  if (c == 'E' || c == 'F') {
    eraseCard();
  }
  if (c == 'F' || c == 'Q') {
    formatCard();
  }
}
//------------------------------------------------------------------------------
void loop() {}