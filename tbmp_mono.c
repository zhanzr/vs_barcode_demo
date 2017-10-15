#include <stdint.h>
#include <stdio.h>

typedef struct strSimpleBMP16Header{
		uint32_t FileSize;//DataOff+Wid*Hgt*2
		uint16_t unused1;
		uint16_t unused2;
		uint32_t DataOff;//InfoSize + 0x16 always

		uint32_t InfoSize;//always 0x28
		uint32_t Wid;
		uint32_t Hgt;
		uint16_t PaneNum;//always 1
		uint16_t ColorDepth;//always 0x10

		uint32_t CompressMethod;//Always BI_BITFIELDS for simplification
		uint32_t BitSize;// Wid*Hgt*2, Including Padding 4Byte
		uint32_t XDPI;//Always 0x0B13 for simplification
		uint32_t YDPI;//Always 0x0B13 for simplification
		uint32_t ColorInPlatte;//0 for simplification
		uint32_t ImportantColor;//0 for simplification
		
		//uint32_t R_Mask;//0xF800
		//uint32_t G_Mask;//0x07E0
		//uint32_t B_Mask;//0x001F
		uint32_t Bit_Mask;//0
		uint32_t A_Mask;//0x00FFFFFF;		
}SimpleBMP16Header;


//For BMP screen shot output
#define BI_RGB	0
#define BI_BITFIELDS	3
#define CI_12_FORMAT    0x03
#define CI_16_FORMAT    0x05
#define CI_18_FORMAT    0x06

#define BLACK16 0x0000
#define WHITE16 0xffff
#define RED16   0xf800
#define CYNK16   0x4ffc
#define GREEN16 0x07e0
#define BLUE16  0x001f
#define LIGHT_BLUE16  0x067f
#define GREEN_YELLOW16  0xb7e6

#define LCD_WID ((uint16_t)320)
#define LCD_HGT ((uint16_t)240)
#define LCD_W LCD_WID
#define LCD_H LCD_HGT
#define LCD_PTS_TOTAL	((uint32_t)LCD_W*LCD_H)
#define TEST_DPT	1	//16

const uint16_t BMPFileType = 0x4D42;//Always "BM"

int main(int argc, char** argv)
{
	printf("%d\n", sizeof(SimpleBMP16Header));
	
	SimpleBMP16Header sbh;
    uint8_t tmpPix;
    uint32_t bw = 0;
	FILE* fOutputBin;
	uint32_t i;
	uint32_t j;
	
	fOutputBin = fopen("Test.bmp", "wb");
	sbh.unused1 = 0;
    sbh.InfoSize = 0x28;
    sbh.DataOff = sbh.InfoSize + 0x16;
    sbh.FileSize = sbh.DataOff + LCD_W * LCD_H * TEST_DPT/8;

    sbh.Wid = LCD_W;
    sbh.Hgt = LCD_H;

    sbh.PaneNum = 1;
    sbh.ColorDepth = TEST_DPT;

    sbh.CompressMethod = BI_RGB ;//BI_RGB
    sbh.BitSize = LCD_W * LCD_H * TEST_DPT / 8;
    sbh.XDPI = 0;//0x0B13;
    sbh.YDPI = 0;//0x0B13;
    sbh.ColorInPlatte = 0;
    sbh.ImportantColor = 0;

    //sbh.R_Mask = 0xF800;
    //sbh.G_Mask = 0x07E0;
    //sbh.B_Mask = 0x001F;
    sbh.Bit_Mask = 0;
    sbh.A_Mask = 0x00ffffff;
	
    fwrite(&BMPFileType, sizeof(BMPFileType), 1, fOutputBin);
	fwrite(&sbh, sizeof(sbh), 1, fOutputBin);
	//fwrite(raw_header, sizeof(raw_header), 1, fOutputBin);
    tmpPix = 0;
    for(i=0; i<LCD_H/3; ++i)
    {
        for(j=0; j<LCD_W*TEST_DPT/8; j++)
        {
            fwrite(&tmpPix, sizeof(tmpPix), 1, fOutputBin);
        }
    }

    tmpPix = 0xff;
    for(; i<LCD_H; i++)
    {
        for(j=0; j<LCD_W*TEST_DPT/8; j++)
        {
            fwrite(&tmpPix, sizeof(tmpPix), 1, fOutputBin);
        }
    }
	        
	fseek(fOutputBin, sbh.DataOff + LCD_W * LCD_H * TEST_DPT/8, SEEK_SET);
    fclose(fOutputBin);
	
	return 0;
}