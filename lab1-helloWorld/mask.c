#include <stdio.h>

int main(void) {
    unsigned int bits = 0xAABBCCDD;
    unsigned char msb = bits >> 24;
    printf("MSB: 0x%x\n", msb);

    unsigned char center = bits >> 20;
    center = center & 0x3F;
    printf("Center: 0x%x\n", center);

    unsigned short msb2 = bits >> 16;
    printf("MSB2: 0x%x\n", msb2);

    unsigned char lsb = bits & 0x3;
    printf("LSB: 0x%x\n", lsb);

    unsigned char center2 = bits >> 7;
    center2 = center2 & 0x7;
    printf("Center2: 0x%x\n", center2);
}