#ifndef FPLIB_H
#define FPLIB_H

class fixed
{
  public:
  fixed(void);
  fixed(long);
  fixed(long,bool);
  long value;
  bool isone; //not used currently
};

class lfixed
{
  public:
  lfixed(void);
  lfixed(long long);
  long long value;
};

fixed::fixed(void)
{
  fixed::value=0;
  bool isone=false;
}

fixed::fixed(long x)
{
  fixed::value=x;
  bool isone=false;
  //if(x==1) Serial.println("Did you mean 'one'?");
}

fixed::fixed(long x, bool is_one)
{
  fixed::value=x;
  bool isone=is_one;
}

lfixed::lfixed(void)
{}

lfixed::lfixed(long long x)
{lfixed::value=x;}

/*fixed newfixed(long x)
{
  fixed z;
  z.value=x;
  return z;
}

lfixed newlfixed(long long x)
{
  lfixed z;
  z.value=x;
  return z;
}*/

const fixed one = fixed(((signed long)((1UL<<31)-1))+1, true);

bool operator==(fixed x, fixed y)
{return x.value==y.value ? true : false;}

bool operator!=(fixed x, fixed y)
{return x.value!=y.value ? true : false;}

bool operator>(fixed x, fixed y)
{return (x.value>y.value)||((x==one)&&(y!=one)) ? true : false;}

bool operator>=(fixed x, fixed y)
{return (x.value>=y.value)||(x==one) ? true : false;}

bool operator<(fixed x, fixed y)
{return (x.value<y.value)||((y==one)&&(x!=one)) ? true : false;}

bool operator<=(fixed x, fixed y)
{return (x.value<=y.value)||(y==one) ? true : false;}


bool operator==(lfixed x, lfixed y)
{return x.value==y.value ? true : false;}

bool operator!=(lfixed x, lfixed y)
{return x.value!=y.value ? true : false;}

bool operator>(lfixed x, lfixed y)
{return x.value>y.value ? true : false;}

bool operator>=(lfixed x, lfixed y)
{return x.value>=y.value ? true : false;}

bool operator<(lfixed x, lfixed y)
{return x.value<y.value ? true : false;}

bool operator<=(lfixed x, lfixed y)
{return x.value<=y.value ? true : false;}

/*bool operator==(fixed x, long y)
{return x.value==y ? true : false;}

bool operator!=(fixed x, long y)
{return x.value!=y ? true : false;}

bool operator>(fixed x, long y)
{return x.value>y ? true : false;}

bool operator>=(fixed x, long y)
{return x.value>=y ? true : false;}

bool operator<(fixed x, long y)
{return x.value<y ? true : false;}

bool operator<=(fixed x, long y)
{return x.value<=y ? true : false;}
*/

fixed operator+(fixed x, fixed y)
{
  fixed z;
  z.value=x.value+y.value;
  return z;
}

/*fixed operator+(long x, fixed y)
{
  fixed z;
  z.value=x+y.value;
  return z;
}

fixed operator+(fixed x, long y)
{
  fixed z;
  z.value=x.value+y;
  return z;
}
*/
fixed operator-(fixed x, fixed y)
{
  fixed z;
  z.value=x.value-y.value;
  return z;
}

/*fixed operator-(long x, fixed y)
{
  fixed z;
  z.value=x-y.value;
  return z;
}

fixed operator-(fixed x, long y)
{
  fixed z;
  z.value=x.value-y;
  return z;
}
*/
fixed operator-(fixed x)
{
  fixed z;
  if (x==one)
    z.value=((signed long)((1L<<31)-1))+2;
  else
    z.value=-x.value;
  return z;
}

fixed operator<<(fixed x, char y)
{
  fixed z;
  z.value=x.value<<y;
  if(z<0 && x>0)
    z=one;
  return z;
}

fixed operator>>(fixed x, char y)
{
  fixed z;
  if(y==0)
    return x;
  if(x==one)
    z.value=1L<<(31-y);
  else
    z.value=x.value>>y;
  return z;
}


lfixed operator+(lfixed x, lfixed y)
{
  lfixed z;
  z.value=x.value+y.value;
  return z;
}

/*lfixed operator+(long long x, lfixed y)
{
  lfixed z;
  z.value=x+y.value;
  return z;
}

lfixed operator+(lfixed x, long long y)
{
  lfixed z;
  z.value=x.value+y;
  return z;
}
*/
lfixed operator-(lfixed x, lfixed y)
{
  lfixed z;
  z.value=x.value-y.value;
  return z;
}

/*lfixed operator-(long long x, lfixed y)
{
  lfixed z;
  z.value=x-y.value;
  return z;
}

lfixed operator-(lfixed x, long long y)
{
  lfixed z;
  z.value=x.value-y;
  return z;
}
*/
lfixed operator-(lfixed x)
{
  lfixed z;
  z.value=-x.value;
  return z;
}

lfixed operator<<(lfixed x, char y)
{
  lfixed z;
  z.value=x.value<<y;
  return z;
}

lfixed operator>>(lfixed x, char y)
{
  lfixed z;
  z.value=x.value>>y;
  return z;
}

fixed operator*(fixed x, fixed y) //multiply and conquer!
{
  fixed z;
  uint32_t tmp;
  uint8_t zero;
  if(y==one)
    return x;
  else if(x==one)
    return y;
  else if(x==-one && y==-one)
    return one;
  else
    //z.value=(x.value>0) ? ((long long)(y.value)*((unsigned long)(x.value)<<1)+0x80000000)>>32 : -(((long long)(y.value)*((unsigned long)(-x.value)<<1)+0x80000000)>>32);
    asm (    // 160 cycles
    "clr %[Z] \n\t"
    "fmuls %D[X], %D[Y] \n\t"
    "movw %C[R], r0 \n\t"
    "fmulsu %D[Y], %B[X]  \n\t"
    "sbc %C[R], %[Z]  \n\t"
    "sbc %D[R], %[Z]  \n\t"
    "movw %A[R], r0  \n\t"
    "fmulsu %D[Y], %C[X]  \n\t"
    "sbc %D[R], %[Z]  \n\t"
    "add %B[R], r0  \n\t"
    "adc %C[R], r1  \n\t"
    "adc %D[R], %[Z]  \n\t"
    "fmulsu %D[Y], %A[X]  \n\t"
    "sbc %B[R], %[Z]  \n\t"
    "sbc %C[R], %[Z]  \n\t"
    "sbc %D[R], %[Z]  \n\t"
    "mov %D[T], r0  \n\t"
    "add %A[R], r1  \n\t"
    "adc %B[R], %[Z]  \n\t"
    "adc %C[R], %[Z]  \n\t"
    "adc %D[R], %[Z]  \n\t"
    "fmulsu %D[X], %C[Y]  \n\t"
    "sbc %D[R], %[Z]  \n\t"
    "add %B[R], r0  \n\t"
    "adc %C[R], r1  \n\t"
    "adc %D[R], %[Z]  \n\t"
    "fmul %C[X], %C[Y]  \n\t"
    "adc %C[R], %[Z]  \n\t"
    "adc %D[R], %[Z]  \n\t"
    "add %A[R], r0  \n\t"
    "adc %B[R], r1  \n\t"
    "adc %C[R], %[Z]  \n\t"
    "adc %D[R], %[Z]  \n\t"
    "fmul %B[X], %C[Y]  \n\t"
    "adc %B[R], %[Z]  \n\t"
    "adc %C[R], %[Z]  \n\t"
    "adc %D[R], %[Z]  \n\t"
    "add %D[T], r0  \n\t"
    "adc %A[R], r1  \n\t"
    "adc %B[R], %[Z]  \n\t"
    "adc %C[R], %[Z]  \n\t"
    "adc %D[R], %[Z]  \n\t"
    "fmul %A[X], %C[Y]   \n\t"
    "adc %A[R], %[Z]  \n\t"
    "adc %B[R], %[Z]  \n\t"
    "adc %C[R], %[Z]  \n\t"
    "adc %D[R], %[Z]  \n\t"
    "mov %C[T], r0  \n\t"
    "add %D[T], r1  \n\t"
    "adc %A[R], %[Z]  \n\t"
    "adc %B[R], %[Z]  \n\t"
    "adc %C[R], %[Z]  \n\t"
    "adc %D[R], %[Z]  \n\t"
    "fmulsu %D[X], %B[Y]  \n\t"
    "sbc %C[R], %[Z]  \n\t"
    "sbc %D[R], %[Z]  \n\t"
    "add %A[R], r0  \n\t"
    "adc %B[R], r1  \n\t"
    "adc %C[R], %[Z]  \n\t"
    "adc %D[R], %[Z]  \n\t"
    "fmul %C[X], %B[Y]  \n\t"
    "adc %B[R], %[Z]  \n\t"
    "adc %C[R], %[Z]  \n\t"
    "adc %D[R], %[Z]  \n\t"
    "add %D[T], r0  \n\t"
    "adc %A[R], r1  \n\t"
    "adc %B[R], %[Z]  \n\t"
    "adc %C[R], %[Z]  \n\t"
    "adc %D[R], %[Z]  \n\t"
    "fmul %B[X], %B[Y]  \n\t"
    "adc %A[R], %[Z]  \n\t"
    "adc %B[R], %[Z]  \n\t"
    "adc %C[R], %[Z]  \n\t"
    "adc %D[R], %[Z]  \n\t"
    "add %C[T], r0  \n\t"
    "adc %D[T], r1  \n\t"
    "adc %A[R], %[Z]  \n\t"
    "adc %B[R], %[Z]  \n\t"
    "adc %C[R], %[Z]  \n\t"
    "adc %D[R], %[Z]  \n\t"
    "fmul %A[X], %B[Y]  \n\t"
    "adc %D[T], %[Z]  \n\t"
    "adc %A[R], %[Z]  \n\t"
    "adc %B[R], %[Z]  \n\t"
    "adc %C[R], %[Z]  \n\t"
    "adc %D[R], %[Z]  \n\t"
    "mov %B[T], r0   \n\t"
    "add %C[T], r1  \n\t"
    "adc %D[T], %[Z]  \n\t"
    "adc %A[R], %[Z]  \n\t"
    "adc %B[R], %[Z]  \n\t"
    "adc %C[R], %[Z]  \n\t"
    "adc %D[R], %[Z]  \n\t"
    "fmulsu %D[X], %A[Y]  \n\t"
    "sbc %B[R], %[Z]  \n\t"
    "sbc %C[R], %[Z]  \n\t"
    "sbc %D[R], %[Z]  \n\t"
    "add %D[T], r0  \n\t"
    "adc %A[R], r1  \n\t"
    "adc %B[R], %[Z]  \n\t"
    "adc %C[R], %[Z]  \n\t"
    "adc %D[R], %[Z]  \n\t"
    "fmul %C[X], %A[Y]  \n\t"
    "adc %A[R], %[Z]  \n\t"
    "adc %B[R], %[Z]  \n\t"
    "adc %C[R], %[Z]  \n\t"
    "adc %D[R], %[Z]  \n\t"
    "add %C[T], r0  \n\t"
    "adc %D[T], r1  \n\t"
    "adc %A[R], %[Z]  \n\t"
    "adc %B[R], %[Z]  \n\t"
    "adc %C[R], %[Z]  \n\t"
    "adc %D[R], %[Z]  \n\t"
    "fmul %B[X], %A[Y]  \n\t"
    "adc %D[T], %[Z]  \n\t"
    "adc %A[R], %[Z]  \n\t"
    "adc %B[R], %[Z]  \n\t"
    "adc %C[R], %[Z]  \n\t"
    "adc %D[R], %[Z]  \n\t"
    "add %B[T], r0   \n\t"
    "adc %C[T], r1  \n\t"
    "adc %D[T], %[Z]  \n\t"
    "adc %A[R], %[Z]  \n\t"
    "adc %B[R], %[Z]  \n\t"
    "adc %C[R], %[Z]  \n\t"
    "adc %D[R], %[Z]  \n\t"
    "fmul %A[X], %A[Y]  \n\t"
    "adc %C[T], %[Z]  \n\t"
    "adc %D[T], %[Z]  \n\t"
    "adc %A[R], %[Z]  \n\t"
    "adc %B[R], %[Z]  \n\t"
    "adc %C[R], %[Z]  \n\t"
    "adc %D[R], %[Z]  \n\t"
//    "mov %A[T], r0  ; not needed for fixed multiplication \n\t"
    "add %B[T], r1  \n\t"
    "adc %C[T], %[Z]  \n\t"
    "adc %D[T], %[Z]  \n\t"
    "adc %A[R], %[Z]  \n\t"
    "adc %B[R], %[Z]  \n\t"
    "adc %C[R], %[Z]  \n\t"
    "adc %D[R], %[Z]  \n\t"
    "clr r1  \n\t"
    : [R]"=&r"(z.value), [T]"=&r"(tmp), [Z]"=&r"(zero)
    : [X]"a"(x.value), [Y]"a"(y.value)
    );
  return z;
}

inline fixed pow2(fixed x)
{
  return x*x;
}

inline fixed pow3(fixed x)
{
  return x*x*x;
}

inline fixed pow4(fixed x)
{
  return x*x*x*x;
}

lfixed operator%(fixed x, fixed y) //NOTE that it is NOT a division
{
  lfixed z;
  if (y==one && x==one)
    z.value=1LL<<62;
  else if (y==one)
    z.value=(long long)x.value<<31;
  else if (x==one)
    z.value=(long long)y.value<<31;
  else
    z.value=(long long)x.value*(long long)y.value;
  return z;
}

fixed operator/(lfixed x, fixed y)
{
  fixed z;
  if(y==one)
    z.value=x.value>>31;
  else
    {z.value=x.value/y.value;
    if (z.value==one && x.value<0)
      z.value=one.value+1;
    }
  return z;
}

fixed operator/(lfixed x, lfixed y)
{
  fixed z;
  unsigned long long a,b;
  unsigned char sign=0;
//  if(y.value==0)
//    return one;
  if(x.value<0)
  {
    a=-x.value;
    sign^=1;
  }
  else
    a=x.value;
  if(y.value<0)
  {
    b=-y.value;
    sign^=1;
  }
  else
    b=y.value;
  if(b<a)
    return one;

  z.value=0;
  for(unsigned char i=30; i!=255; i--)
  {
    a<<=1;
    if(b<a)
    {
      z.value|=1UL<<i;
      a-=b;
    }
  }
  if(b<a<<1)
    z.value++; //rounding
  return sign?-z:z;
//  if(sign==1)
//    z.value=-z.value;
//  return z;
}

fixed tofixed(lfixed x)
{
//  fixed z;
  if (x>=0x4000000000000000LL)
    return one;
  if (x<=-0x3FFFFFFF80000000LL)
    return -one;
  return x>0?(x.value+0x40000000)>>31:-((-x.value+0x40000000)>>31);
//  return z;
}

lfixed tolfixed(fixed x)
{
  if(x==one)
    return 0x4000000000000000LL;
  if(x==-one)
    return -0x4000000000000000LL;
  return x>0? ((long long)x.value)<<31:-(((long long)x.value)<<31);
}

lfixed ldiv(lfixed x, lfixed y)
{
  lfixed z;
  unsigned long long a,b;
  unsigned char sign=0;
//  print("ldiv: a",a);
//  print("ldiv: b",b);
  if(x.value<0)
  {
    a=-x.value;
    sign^=1;
  }
  else
    a=x.value;
  if(y.value<0)
  {
    b=-y.value;
    sign^=1;
  }
  else
    b=y.value;
  if(b<a>>1)
    return 0;

  z.value=0;
  for(unsigned char i=62; i!=255; i--)
  {
    if(b<a)
    {
      z.value|=1ULL<<i;
      a-=b;
    }
    a<<=1;
  }
  if (b<a)
    x.value++; //rounding
  return sign?-z:z;
}

fixed operator/(fixed x, int y)
{
  fixed z;
  if(x==one)
    z.value=(one.value-1)/y;
  else
    z.value=x.value/y;
  return z;
}

lfixed operator*(lfixed x, lfixed y)
{
  unsigned char sign=0;
  lfixed z=0;
//  unsigned long long b=y.value;
//Serial.println((long)(b>>32),HEX);
  if(x.value<0)
    sign^=1;
  if(y.value<0)
    {sign^=1;
    y.value=-y.value;
    }

  for(unsigned char i=62; i!=255; i--)
  {
    if(x.value&(1ULL<<i))
      z=z+y;
      //Serial.println("bingo");
    //}
    y=y>>1;
//    Serial.println((long)((x.value&(1ULL<<i))>>32),HEX);
//    Serial.println((long)(b>>32),HEX);
  }
  return sign?-z:z;
}

lfixed operator*(lfixed x, fixed y)
{
  return x*tolfixed(y);
}

/*   I used the following code to generate the below lookup table:
void printSqrtTable(void)
{
  Serial.print("const unsigned char l_sqrt[192]={		// input: x (64-255), output: sqrt((x-64)<<8)\n");
  for(i=64; i<256; i++)
  {
    Serial.print((unsigned byte)sqrt(i<<8));
    if(i!=255)
      Serial.print(",\n");
    else
      Serial.print("};\n");
  }
}
*/

const unsigned char l_sqrt[192]={		// input: x (64-255), output: sqrt((x-64)<<8)
128,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,144,145,146,147,148,149,150,150,151,152,153,154,155,155,
156,157,158,159,160,160,161,162,163,163,164,165,166,167,167,168,169,170,170,171,172,173,173,174,175,176,176,177,178,178,179,180,
181,181,182,183,183,184,185,185,186,187,187,188,189,189,190,191,192,192,193,193,194,195,195,196,197,197,198,199,199,200,201,201,
202,203,203,204,204,205,206,206,207,208,208,209,209,210,211,211,212,212,213,214,214,215,215,216,217,217,218,218,219,219,220,221,
221,222,222,223,224,224,225,225,226,226,227,227,228,229,229,230,230,231,231,232,232,233,234,234,235,235,236,236,237,237,238,238,
239,240,240,241,241,242,242,243,243,244,244,245,245,246,246,247,247,248,248,249,249,250,250,251,251,252,252,253,253,254,254,255
};

unsigned long usqrt(lfixed x)
{
  char i;
  unsigned long a=0;
  long long t=x.value;
  if(t<=0) return 0;
  for(i=2; (t=t>>2)!=0; i+=2); i-=8;
  if(i<0)
    a=((unsigned long)l_sqrt[(x.value<<(-i))-64])>>(4+((-i)>>1));
  else if (i<8)
    a=((unsigned long)l_sqrt[(x.value>>i)-64])>>(4-(i>>1));
  else
    a=((unsigned long)l_sqrt[(x.value>>i)-64])<<((i>>1)-4);
  a=(x.value/a+a)>>1;
  return (x.value/a+a)>>1;
}

fixed sqrt(lfixed x)
{
  char i;
  unsigned long a=0;
  long long t=x.value;
  if(t<=0) return 0;
  for(i=2; (t=t>>2)!=0; i+=2); i-=8;
  if(i<0)
    a=((unsigned long)l_sqrt[(x.value<<(-i))-64])>>(4+((-i)>>1));
  else if (i<8)
    a=((unsigned long)l_sqrt[(x.value>>i)-64])>>(4-(i>>1));
  else
    a=((unsigned long)l_sqrt[(x.value>>i)-64])<<((i>>1)-4);
  a=(x.value/a+a+1)>>1;
//  a=(x.value/a+a+1)>>1;
  return (x.value/a+a+1)>>1;
}

//void print(const char *name, lfixed val);

lfixed lsqrt(lfixed x)
{
  char i;
  lfixed a=0;
  long long t=x.value;
//  print("sq",x);
  if(t<=0) return 0;
  for(i=2; (t=t>>2)!=0; i+=2); i-=8;
//  print("i",i);
  if(i<0)
    a=((unsigned long long)l_sqrt[(x.value<<(-i))-64])<<(27-((-i)>>1));
//  else if (i<8)
//    a=((unsigned long long)l_sqrt[(x.value>>i)-64])>>(4-(i>>1));
  else
    a=((unsigned long long)l_sqrt[(x.value>>i)-64])<<((i>>1)+27);
//  print("da", a);
//  print("dd",ldiv(x,a));
  a=(ldiv(x,a)>>1)+(a>>1);
//  print("da", a);
//  print("dd",ldiv(x,a));
  a=(ldiv(x,a)>>1)+(a>>1);
//  print("da", a);
//  print("dd",ldiv(x,a));
  return (ldiv(x,a)>>1)+(a>>1);
//  return a;
}

#endif
