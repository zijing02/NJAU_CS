cfimager.exe tool is used to flash boot images and create FAT partition on SD/MMC cards
on the host PC for i.MX233 ,i.MX28 ,i.MX50 ,i.MX51 and i.MX53 SOCs. 

- When switching the card among i.MX233 ,i.MX28 ,i.MX50 ,i.MX51 and i.MX53 , it is recommended to use 
  the "-a" option to reformat the partitions.

- The tool is not case sensitive for the file name, and can also handle paths before
  the file name.

For Linux usage, typical usage mode is burn uboot to sd card.
i.MX233 and i.MX28 Instructions
=====================
- To flash the uboot, use the following command:
cfimager -f uboot.sb -d <card reader drive letter without colon>

i.MX50, i.MX51 and i.MX53 Instructions
=====================
- To flash the uboot, use the following command:
cfimager -raw -offset 0x400 -skip 0x400 -f u-boot.bin -d <card reader drive letter without colon>
Note if changing -offset 0xXXXX, other image can be flashed to SD card.
  
For Wince usage, please follow below commands.  
i.MX233 and i.MX28 Instructions
=====================

The fat partition for i.MX233&i.MX28 is created if one is not found on the card, 
or if the "-a" option is specified in the command line. After building a new bootloader
or NK image, the eboot.sb or nk.sb file will get copied to the
<%WINCEROOT%>\SUPPORT\TOOL\iMX233-EVK\SBIMAGE
or
<%WINCEROOT%>\SUPPORT\TOOL\iMX28-EVK\SBIMAGE folder. 

- To flash the bootloader, use the following command:
  cfimager -f eboot.sb -d <card reader drive letter without colon>

- To flash the OS image (without the bootloader), use the following command:
  cfimager -f nk.sb -d <card reader drive letter without colon>

- To flash the OS image (NK.nb0, found in the release directory of the OSDesign), use 
  the following command: 
  cfimager -f eboot.sb -d <card reader drive letter without colon> -e nk.nb0
  Note:to enable SD/MMC NK image flashing function in EBOOT, need reserve space for NK 
  using this option in the first time. 

- To add the fat partition, add "-a" option to any of the above command lines.

- To load the OS image from the SD/MMC card (with the bootloader), 
  copy the NK.bin to <card reader drive letter without colon>:\
  Also, in the bootloader menu, select "SDMMC Storage" as the Ether Device.

- To enable SD Card boot, the following fuses on the i.MX233 need to be blown
     HW_OCOTP_ROM0: SD_MBR_BOOT(3) Blown
     SD_POWER_GATE_GPIO(21,20) 10-PWM3

- Boot Mode switch on the board needs to be set to 1001


i.MX51 Instructions
=====================

The cfimager tool for i.MX51 can only flash *.nb0 files, found in the release directory
of the OSDesign. Please make sure to build the XLDR for SD, as XLDR for other flash device
may be built by default. Set IMGSDMMC=1 in the OSDesign before building the XLDR for SD.

- To flash the XLDR, use the following command:
  cfimager -f xldr.nb0 -d <card reader drive letter without colon> -imx51

- To flash the bootloader, use the following command:
  cfimager -f eboot.nb0 -d <card reader drive letter without colon> -imx51

- To flash the OS image, use the following command:
  cfimager -f nk.nb0 -d <card reader drive letter without colon> -imx51

  Note that flashing a debug nk.nb0 image will not work because it is too big.

- To add the fat partition, add "-a" option to any of the above command lines.

- To load the OS image from the SD/MMC card, after the nk.nb0 has been flashed using the
  above command line, in the bootloader menu, select "NK from SD/MMC" using option 5.

- For i.MX51-EVK, plug the card into the SD1 slot on the underside of the board, and boot
  switch S1 needs to be 7,8 ON, others OFF.

i.MX53&i.MX50 Instructions
=====================

The cfimager tool for i.MX53&i.MX50 can only flash *.nb0 files, found in the release directory
of the OSDesign. Please make sure to build the eboot for SD, as eboot for ram is built by default. Set IMGSDMMC=1 in the OSDesign before building the EBOOT for SD.

Note: There is no xldr anymore!!!
	  And i.MX50 and i.MX53 ROM use the same command. 	

- To flash the bootloader, use the following command:
  cfimager -f eboot.nb0 -d <card reader drive letter without colon> -imx53

- To flash the OS image, use the following command:
  cfimager -f nk.nb0 -d <card reader drive letter without colon> -imx53

  Note that flashing a debug nk.nb0 image will not work because it is too big.

- To add the fat partition, add "-a" option to any of the above command lines.

- To load the OS image from the SD/MMC card, after the nk.nb0 has been flashed using the
  above command line, in the bootloader menu, select "NK from SD/MMC" using option 5.

- For i.MX53-EVK, plug the card into the SD1 slot on the upside of the board, and boot
  switch S1 needs to be 7 ON, others OFF.

- For i.MX53-Armadillo2, plug the card into the SD1 slot on the downside of the CPU board, and boot
  switch S1 needs to be 7 ON, others OFF. 

- For i.MX53, other SD slots can also support eboot boot up. The corresponding boot switch need to be set.

- For i.MX50-RD1, plug the card into the SD1 slot on the upside of the board, and boot
  switch S1 needs to be 1,2 OFF and S5 needs to be 2,5 ON, others OFF.