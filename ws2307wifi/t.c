#include <stdio.h>

unsigned char BCD_to_HEX(char high, char low)
{
unsigned char res = 0;

  if(high >= '0' && high <= '9') res = high - '0';
  if(high >= 'A' && high <= 'F') res = high - 'A' + 0xa;
  res <<= 4;
  
  if(low >= '0' && low <= '9') res |= low - '0';
  if(low >= 'A' && low <= 'F') res |= low - 'A' + 0xa;
}

int main()
{
    char src[4] = {'6','5','6','A'};
    
    unsigned char z1 = BCD_to_HEX(src[0],src[1]);
    unsigned char z2 = BCD_to_HEX(src[2],src[3]);
    
    printf("%x %x\n",z1,z2);
}
