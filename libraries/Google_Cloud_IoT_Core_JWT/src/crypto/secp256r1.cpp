// Also know as prime256v1 aka NIST P-256
#include <String.h>
#include "ecc.h"

#define TRUE 1
#define FALSE 0

void get_curve_param(curve_params_t *para)
{
#ifdef EIGHT_BIT_PROCESSOR
  memset(para->p, 0, NUMWORDS * NN_DIGIT_LEN);
  para->p[31] = 0xFF;
  para->p[30] = 0xFF;
  para->p[29] = 0xFF;
  para->p[28] = 0xFF;
  para->p[27] = 0x00;
  para->p[26] = 0x00;
  para->p[25] = 0x00;
  para->p[24] = 0x01;
  para->p[23] = 0x00;
  para->p[22] = 0x00;
  para->p[21] = 0x00;
  para->p[20] = 0x00;
  para->p[19] = 0x00;
  para->p[18] = 0x00;
  para->p[17] = 0x00;
  para->p[16] = 0x00;
  para->p[15] = 0x00;
  para->p[14] = 0x00;
  para->p[13] = 0x00;
  para->p[12] = 0x00;
  para->p[11] = 0xFF;
  para->p[10] = 0xFF;
  para->p[9] = 0xFF;
  para->p[8] = 0xFF;
  para->p[7] = 0xFF;
  para->p[6] = 0xFF;
  para->p[5] = 0xFF;
  para->p[4] = 0xFF;
  para->p[3] = 0xFF;
  para->p[2] = 0xFF;
  para->p[1] = 0xFF;
  para->p[0] = 0xFF;

  memset(para->omega, 0, NUMWORDS * NN_DIGIT_LEN);
  para->omega[27] = 0xFF;
  para->omega[26] = 0xFF;
  para->omega[25] = 0xFF;
  para->omega[24] = 0xFE;
  para->omega[23] = 0xFF;
  para->omega[22] = 0xFF;
  para->omega[21] = 0xFF;
  para->omega[20] = 0xFF;
  para->omega[19] = 0xFF;
  para->omega[18] = 0xFF;
  para->omega[17] = 0xFF;
  para->omega[16] = 0xFF;
  para->omega[15] = 0xFF;
  para->omega[14] = 0xFF;
  para->omega[13] = 0xFF;
  para->omega[12] = 0xFF;
  para->omega[11] = 0x00;
  para->omega[10] = 0x00;
  para->omega[9] = 0x00;
  para->omega[8] = 0x00;
  para->omega[7] = 0x00;
  para->omega[6] = 0x00;
  para->omega[5] = 0x00;
  para->omega[4] = 0x00;
  para->omega[3] = 0x00;
  para->omega[2] = 0x00;
  para->omega[1] = 0x00;
  para->omega[0] = 0x01;
  // curve that will be used
  // a = -3
  memset(para->E.a, 0, NUMWORDS * NN_DIGIT_LEN);
  para->E.a[31] = 0xFF;
  para->E.a[30] = 0xFF;
  para->E.a[29] = 0xFF;
  para->E.a[28] = 0xFF;
  para->E.a[27] = 0x00;
  para->E.a[26] = 0x00;
  para->E.a[25] = 0x00;
  para->E.a[24] = 0x01;
  para->E.a[23] = 0x00;
  para->E.a[22] = 0x00;
  para->E.a[21] = 0x00;
  para->E.a[20] = 0x00;
  para->E.a[19] = 0x00;
  para->E.a[18] = 0x00;
  para->E.a[17] = 0x00;
  para->E.a[16] = 0x00;
  para->E.a[15] = 0x00;
  para->E.a[14] = 0x00;
  para->E.a[13] = 0x00;
  para->E.a[12] = 0x00;
  para->E.a[11] = 0xFF;
  para->E.a[10] = 0xFF;
  para->E.a[9] = 0xFF;
  para->E.a[8] = 0xFF;
  para->E.a[7] = 0xFF;
  para->E.a[6] = 0xFF;
  para->E.a[5] = 0xFF;
  para->E.a[4] = 0xFF;
  para->E.a[3] = 0xFF;
  para->E.a[2] = 0xFF;
  para->E.a[1] = 0xFF;
  para->E.a[0] = 0xFC;

  para->E.a_minus3 = TRUE;
  para->E.a_zero = FALSE;

  // b = 7
  memset(para->E.b, 0, NUMWORDS * NN_DIGIT_LEN);
  para->G.x[31] = 0x5A;
  para->G.x[30] = 0xC6;
  para->G.x[29] = 0x35;
  para->G.x[28] = 0xD8;
  para->G.x[27] = 0xAA;
  para->G.x[26] = 0x3A;
  para->G.x[25] = 0x93;
  para->G.x[24] = 0xE7;
  para->G.x[23] = 0xB3;
  para->G.x[22] = 0xEB;
  para->G.x[21] = 0xBD;
  para->G.x[20] = 0x55;
  para->G.x[19] = 0x76;
  para->G.x[18] = 0x98;
  para->G.x[17] = 0x86;
  para->G.x[16] = 0xBC;
  para->G.x[15] = 0x65;
  para->G.x[14] = 0x1D;
  para->G.x[13] = 0x06;
  para->G.x[12] = 0xB0;
  para->G.x[11] = 0xCC;
  para->G.x[10] = 0x53;
  para->G.x[9] = 0xB0;
  para->G.x[8] = 0xF6;
  para->G.x[7] = 0x3B;
  para->G.x[6] = 0xCE;
  para->G.x[5] = 0x3C;
  para->G.x[4] = 0x3E;
  para->G.x[3] = 0x27;
  para->G.x[2] = 0xD2;
  para->G.x[1] = 0x60;
  para->G.x[0] = 0x4B;

  // base point
  memset(para->G.x, 0, NUMWORDS * NN_DIGIT_LEN);
  para->G.x[31] = 0x6B;
  para->G.x[30] = 0x17;
  para->G.x[29] = 0xD1;
  para->G.x[28] = 0xF2;
  para->G.x[27] = 0xE1;
  para->G.x[26] = 0x2C;
  para->G.x[25] = 0x42;
  para->G.x[24] = 0x47;
  para->G.x[23] = 0xF8;
  para->G.x[22] = 0xBC;
  para->G.x[21] = 0xE6;
  para->G.x[20] = 0xE5;
  para->G.x[19] = 0x63;
  para->G.x[18] = 0xA4;
  para->G.x[17] = 0x40;
  para->G.x[16] = 0xF2;
  para->G.x[15] = 0x77;
  para->G.x[14] = 0x03;
  para->G.x[13] = 0x7D;
  para->G.x[12] = 0x81;
  para->G.x[11] = 0x2D;
  para->G.x[10] = 0xEB;
  para->G.x[9] = 0x33;
  para->G.x[8] = 0xA0;
  para->G.x[7] = 0xF4;
  para->G.x[6] = 0xA1;
  para->G.x[5] = 0x39;
  para->G.x[4] = 0x45;
  para->G.x[3] = 0xD8;
  para->G.x[2] = 0x98;
  para->G.x[1] = 0xC2;
  para->G.x[0] = 0x96;

  memset(para->G.y, 0, NUMWORDS * NN_DIGIT_LEN);
  para->G.y[31] = 0x4F;
  para->G.y[30] = 0xE3;
  para->G.y[29] = 0x42;
  para->G.y[28] = 0xE2;
  para->G.y[27] = 0xFE;
  para->G.y[26] = 0x1A;
  para->G.y[25] = 0x7F;
  para->G.y[24] = 0x9B;
  para->G.y[23] = 0x8E;
  para->G.y[22] = 0xE7;
  para->G.y[21] = 0xEB;
  para->G.y[20] = 0x4A;
  para->G.y[19] = 0x7C;
  para->G.y[18] = 0x0F;
  para->G.y[17] = 0x9E;
  para->G.y[16] = 0x16;
  para->G.y[15] = 0x2B;
  para->G.y[14] = 0xCE;
  para->G.y[13] = 0x33;
  para->G.y[12] = 0x57;
  para->G.y[11] = 0x6B;
  para->G.y[10] = 0x31;
  para->G.y[9] = 0x5E;
  para->G.y[8] = 0xCE;
  para->G.y[7] = 0xCB;
  para->G.y[6] = 0xB6;
  para->G.y[5] = 0x40;
  para->G.y[4] = 0x68;
  para->G.y[3] = 0x37;
  para->G.y[2] = 0xBF;
  para->G.y[1] = 0x51;
  para->G.y[0] = 0xF5;

  // prime divide the number of points
  memset(para->r, 0, NUMWORDS * NN_DIGIT_LEN);
  para->r[31] = 0xFF;
  para->r[30] = 0xFF;
  para->r[29] = 0xFF;
  para->r[28] = 0xFF;
  para->r[27] = 0x00;
  para->r[26] = 0x00;
  para->r[25] = 0x00;
  para->r[24] = 0x00;
  para->r[23] = 0xFF;
  para->r[22] = 0xFF;
  para->r[21] = 0xFF;
  para->r[20] = 0xFF;
  para->r[19] = 0xFF;
  para->r[18] = 0xFF;
  para->r[17] = 0xFF;
  para->r[16] = 0xFF;
  para->r[15] = 0xBC;
  para->r[14] = 0xE6;
  para->r[13] = 0xFA;
  para->r[12] = 0xAD;
  para->r[11] = 0xA7;
  para->r[10] = 0x17;
  para->r[9] = 0x9E;
  para->r[8] = 0x84;
  para->r[7] = 0xF3;
  para->r[6] = 0xB9;
  para->r[5] = 0xCA;
  para->r[4] = 0xC2;
  para->r[3] = 0xFC;
  para->r[2] = 0x63;
  para->r[1] = 0x25;
  para->r[0] = 0x51;
  /* EIGHT_BIT_PROCESSOR */
#elif defined(SIXTEEN_BIT_PROCESSOR)
  memset(para->p, 0, NUMWORDS * NN_DIGIT_LEN);
  para->p[15] = 0xFFFF;
  para->p[14] = 0xFFFF;
  para->p[13] = 0x0000;
  para->p[12] = 0x0001;
  para->p[11] = 0x0000;
  para->p[10] = 0x0000;
  para->p[9] = 0x0000;
  para->p[8] = 0x0000;
  para->p[7] = 0x0000;
  para->p[6] = 0x0000;
  para->p[5] = 0xFFFF;
  para->p[4] = 0xFFFF;
  para->p[3] = 0xFFFF;
  para->p[2] = 0xFFFF;
  para->p[1] = 0xFFFF;
  para->p[0] = 0xFFFF;

  memset(para->omega, 0, NUMWORDS * NN_DIGIT_LEN);
  para->omega[13] = 0xFFFF;
  para->omega[12] = 0xFFFE;
  para->omega[11] = 0xFFFF;
  para->omega[10] = 0xFFFF;
  para->omega[9] = 0xFFFF;
  para->omega[8] = 0xFFFF;
  para->omega[7] = 0xFFFF;
  para->omega[6] = 0xFFFF;
  para->omega[5] = 0x0000;
  para->omega[4] = 0x0000;
  para->omega[3] = 0x0000;
  para->omega[2] = 0x0000;
  para->omega[1] = 0x0000;
  para->omega[0] = 0x0001;
  // curve that will be used
  // a = -3
  memset(para->E.a, 0, NUMWORDS * NN_DIGIT_LEN);
  para->E.a[15] = 0xFFFF;
  para->E.a[14] = 0xFFFF;
  para->E.a[13] = 0x0000;
  para->E.a[12] = 0x0001;
  para->E.a[11] = 0x0000;
  para->E.a[10] = 0x0000;
  para->E.a[9] = 0x0000;
  para->E.a[8] = 0x0000;
  para->E.a[7] = 0x0000;
  para->E.a[6] = 0x0000;
  para->E.a[5] = 0xFFFF;
  para->E.a[4] = 0xFFFF;
  para->E.a[3] = 0xFFFF;
  para->E.a[2] = 0xFFFF;
  para->E.a[1] = 0xFFFF;
  para->E.a[0] = 0xFFFC;

  para->E.a_minus3 = TRUE;
  para->E.a_zero = FALSE;

  // b = 7
  memset(para->E.b, 0, NUMWORDS * NN_DIGIT_LEN);
  para->G.x[15] = 0x5AC6;
  para->G.x[14] = 0x35D8;
  para->G.x[13] = 0xAA3A;
  para->G.x[12] = 0x93E7;
  para->G.x[11] = 0xB3EB;
  para->G.x[10] = 0xBD55;
  para->G.x[9] = 0x7698;
  para->G.x[8] = 0x86BC;
  para->G.x[7] = 0x651D;
  para->G.x[6] = 0x06B0;
  para->G.x[5] = 0xCC53;
  para->G.x[4] = 0xB0F6;
  para->G.x[3] = 0x3BCE;
  para->G.x[2] = 0x3C3E;
  para->G.x[1] = 0x27D2;
  para->G.x[0] = 0x604B;

  // base point
  memset(para->G.x, 0, NUMWORDS * NN_DIGIT_LEN);
  para->G.x[15] = 0x6B17;
  para->G.x[14] = 0xD1F2;
  para->G.x[13] = 0xE12C;
  para->G.x[12] = 0x4247;
  para->G.x[11] = 0xF8BC;
  para->G.x[10] = 0xE6E5;
  para->G.x[9] = 0x63A4;
  para->G.x[8] = 0x40F2;
  para->G.x[7] = 0x7703;
  para->G.x[6] = 0x7D81;
  para->G.x[5] = 0x2DEB;
  para->G.x[4] = 0x33A0;
  para->G.x[3] = 0xF4A1;
  para->G.x[2] = 0x3945;
  para->G.x[1] = 0xD898;
  para->G.x[0] = 0xC296;

  memset(para->G.y, 0, NUMWORDS * NN_DIGIT_LEN);
  para->G.y[15] = 0x4FE3;
  para->G.y[14] = 0x42E2;
  para->G.y[13] = 0xFE1A;
  para->G.y[12] = 0x7F9B;
  para->G.y[11] = 0x8EE7;
  para->G.y[10] = 0xEB4A;
  para->G.y[9] = 0x7C0F;
  para->G.y[8] = 0x9E16;
  para->G.y[7] = 0x2BCE;
  para->G.y[6] = 0x3357;
  para->G.y[5] = 0x6B31;
  para->G.y[4] = 0x5ECE;
  para->G.y[3] = 0xCBB6;
  para->G.y[2] = 0x4068;
  para->G.y[1] = 0x37BF;
  para->G.y[0] = 0x51F5;

  // prime divide the number of points
  memset(para->r, 0, NUMWORDS * NN_DIGIT_LEN);
  para->r[15] = 0xFFFF;
  para->r[14] = 0xFFFF;
  para->r[13] = 0x0000;
  para->r[12] = 0x0000;
  para->r[11] = 0xFFFF;
  para->r[10] = 0xFFFF;
  para->r[9] = 0xFFFF;
  para->r[8] = 0xFFFF;
  para->r[7] = 0xBCE6;
  para->r[6] = 0xFAAD;
  para->r[5] = 0xA717;
  para->r[4] = 0x9E84;
  para->r[3] = 0xF3B9;
  para->r[2] = 0xCAC2;
  para->r[1] = 0xFC63;
  para->r[0] = 0x2551;

  /* SIXTEEN_BIT_PROCESSOR */
#elif defined(THIRTYTWO_BIT_PROCESSOR)
  // init parameters

  memset(para->p, 0, NUMWORDS * NN_DIGIT_LEN);
  para->p[7] = 0xFFFFFFFF;
  para->p[6] = 0x00000001;
  para->p[5] = 0x00000000;
  para->p[4] = 0x00000000;
  para->p[3] = 0x00000000;
  para->p[2] = 0xFFFFFFFF;
  para->p[1] = 0xFFFFFFFF;
  para->p[0] = 0xFFFFFFFF;

  memset(para->omega, 0, NUMWORDS * NN_DIGIT_LEN);
  para->omega[6] = 0xFFFFFFFE;
  para->omega[5] = 0xFFFFFFFF;
  para->omega[4] = 0xFFFFFFFF;
  para->omega[3] = 0xFFFFFFFF;
  para->omega[2] = 0x00000000;
  para->omega[1] = 0x00000000;
  para->omega[0] = 0x00000001;
  // curve that will be used
  // a = -3
  memset(para->E.a, 0, NUMWORDS * NN_DIGIT_LEN);
  para->E.a[7] = 0xFFFFFFFF;
  para->E.a[6] = 0x00000001;
  para->E.a[5] = 0x00000000;
  para->E.a[4] = 0x00000000;
  para->E.a[3] = 0x00000000;
  para->E.a[2] = 0xFFFFFFFF;
  para->E.a[1] = 0xFFFFFFFF;
  para->E.a[0] = 0xFFFFFFFC;

  para->E.a_minus3 = TRUE;
  para->E.a_zero = FALSE;

  // b = 7
  memset(para->E.b, 0, NUMWORDS * NN_DIGIT_LEN);
  para->G.x[7] = 0x5AC635D8;
  para->G.x[6] = 0xAA3A93E7;
  para->G.x[5] = 0xB3EBBD55;
  para->G.x[4] = 0x769886BC;
  para->G.x[3] = 0x651D06B0;
  para->G.x[2] = 0xCC53B0F6;
  para->G.x[1] = 0x3BCE3C3E;
  para->G.x[0] = 0x27D2604B;

  // base point
  memset(para->G.x, 0, NUMWORDS * NN_DIGIT_LEN);
  para->G.x[7] = 0x6B17D1F2;
  para->G.x[6] = 0xE12C4247;
  para->G.x[5] = 0xF8BCE6E5;
  para->G.x[4] = 0x63A440F2;
  para->G.x[3] = 0x77037D81;
  para->G.x[2] = 0x2DEB33A0;
  para->G.x[1] = 0xF4A13945;
  para->G.x[0] = 0xD898C296;

  memset(para->G.y, 0, NUMWORDS * NN_DIGIT_LEN);
  para->G.y[7] = 0x4FE342E2;
  para->G.y[6] = 0xFE1A7F9B;
  para->G.y[5] = 0x8EE7EB4A;
  para->G.y[4] = 0x7C0F9E16;
  para->G.y[3] = 0x2BCE3357;
  para->G.y[2] = 0x6B315ECE;
  para->G.y[1] = 0xCBB64068;
  para->G.y[0] = 0x37BF51F5;

  // prime divide the number of points
  memset(para->r, 0, NUMWORDS * NN_DIGIT_LEN);
  para->r[7] = 0xFFFFFFFF;
  para->r[6] = 0x00000000;
  para->r[5] = 0xFFFFFFFF;
  para->r[4] = 0xFFFFFFFF;
  para->r[3] = 0xBCE6FAAD;
  para->r[2] = 0xA7179E84;
  para->r[1] = 0xF3B9CAC2;
  para->r[0] = 0xFC632551;

#endif /* THIRTYTWO_BIT_PROCESSOR */
}

NN_UINT omega_mul(NN_DIGIT *a, NN_DIGIT *b, NN_DIGIT *omega, NN_UINT digits)
{
#ifdef EIGHT_BIT_PROCESSOR
  int omega_digit_length = 28;
#elif defined(SIXTEEN_BIT_PROCESSOR)
  int omega_digit_length = 14;
#elif defined(THIRTYTWO_BIT_PROCESSOR)
  int omega_digit_length = 7;
#endif

  NN_Mult(a, b, omega,
          digits > omega_digit_length ? digits : omega_digit_length);
  return digits + omega_digit_length;
}
