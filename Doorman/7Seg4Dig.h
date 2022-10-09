#include <Arduino.h>
#include <TM1637Display.h> //7Seg4Dig Display

//Display
#define DSPCLK 11
#define DSPDIO 12
TM1637Display tm = TM1637Display(DSPCLK, DSPDIO);
const uint8_t dsp_full[] = {0xff, 0xff, 0xff, 0xff}; // All Segments ON
const uint8_t dsp_blnk[] = {0x00, 0x00, 0x00, 0x00}; // All Segments OFF

const uint8_t ltr_a = SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G;
const uint8_t ltr_b = SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G;
const uint8_t ltr_c = SEG_A | SEG_D | SEG_E | SEG_F;
const uint8_t ltr_d = SEG_B | SEG_C | SEG_D | SEG_E | SEG_G;
const uint8_t ltr_e = SEG_A | SEG_D | SEG_E | SEG_F | SEG_G;
const uint8_t ltr_g = SEG_A | SEG_B | SEG_C | SEG_D | SEG_F | SEG_G;
const uint8_t ltr_i = SEG_E | SEG_F;
const uint8_t ltr_l = SEG_D | SEG_E | SEG_F;
const uint8_t ltr_n = SEG_A | SEG_B | SEG_C | SEG_E | SEG_F;
const uint8_t ltr_o = SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F;
const uint8_t ltr_p = SEG_A | SEG_B | SEG_E | SEG_F | SEG_G;
const uint8_t ltr_r = SEG_E | SEG_G;
const uint8_t ltr_t = SEG_D | SEG_E | SEG_F | SEG_G;
const uint8_t ltr_u = SEG_B | SEG_C | SEG_D | SEG_E | SEG_F;

//Display words:
const uint8_t dsp_done[] = { ltr_d, ltr_o, ltr_n, ltr_e }; //DONE
const uint8_t dsp_init[] = { ltr_i, ltr_n, ltr_i, ltr_t }; //INIT
const uint8_t dsp_open[] = { ltr_o, ltr_p, ltr_e, ltr_n }; //0PEN
const uint8_t dsp_debu[] = { ltr_d, ltr_e, ltr_b, ltr_u }; //DEBU

const uint8_t dsp_red[]  = { ltr_r, ltr_e, ltr_d }; //RED
const uint8_t dsp_gree[] = { ltr_g, ltr_r, ltr_e, ltr_e }; //GREE
const uint8_t dsp_blue[] = { ltr_b, ltr_l, ltr_u, ltr_e }; //BLUE

void displayBrightMax()
{
  tm.setBrightness(7);
}

void displayNumber(int num) {
  tm.showNumberDec(num, false);
}

void displayNumberLeading(int num)
{
  tm.showNumberDec(num, true);
}

void displaySegments(uint8_t data[])
{
  tm.setSegments(data);
}
