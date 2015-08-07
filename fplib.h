#ifndef FPLIB_H
#define FPLIB_H

class lfixed;

class fixed
{
  public:
  fixed(void);
  fixed(const long&);
  fixed(const long&, const bool);
  fixed(const lfixed&);
  long value;
  bool isone; //not used currently
  
  bool operator==(const fixed&) const;
  bool operator!=(const fixed&) const;
  bool operator>(const fixed&) const;
  bool operator>=(const fixed&) const;
  bool operator<(const fixed&) const;
  bool operator<=(const fixed&) const;

  fixed& operator+=(const fixed&);
  fixed& operator-=(const fixed&);
  //fixed& operator*=(const fixed&);
  //fixed& operator*=(const int);
  fixed& operator/=(const int);
  fixed& operator<<=(const byte);
  fixed& operator>>=(const byte);

  const fixed operator+(const fixed&) const;
  const fixed operator-(const fixed&) const;
  const fixed operator-(void) const;
  const fixed operator*(const fixed&) const;
  const lfixed operator%(const fixed&) const;  //multiplication with lfixed result
  const fixed operator/(const int) const;
  //const fixed operator*(const int) const;
  const fixed operator<<(const byte) const;
  const fixed operator>>(const byte) const;
};

class lfixed
{
  public:
  lfixed(void);
  lfixed(const long long&);
  lfixed(const fixed&);
  long long value;

  bool operator==(const lfixed&) const;
  bool operator!=(const lfixed&) const;
  bool operator>(const lfixed&) const;
  bool operator>=(const lfixed&) const;
  bool operator<(const lfixed&) const;
  bool operator<=(const lfixed&) const;

  lfixed& operator+=(const lfixed&);
  lfixed& operator-=(const lfixed&);
  lfixed& operator*=(const lfixed&);
  lfixed& operator<<=(const byte);
  lfixed& operator>>=(const byte);

  const lfixed operator+(const lfixed&) const;
  const lfixed operator-(const lfixed&) const;
  const lfixed operator-(void) const;
  const lfixed operator*(const lfixed&) const;
  const fixed operator/(const fixed&) const; //division with fixed result
  const fixed operator/(const lfixed&) const; //division with fixed result
  const lfixed operator<<(const byte) const;
  const lfixed operator>>(const byte) const;
};

const fixed one = fixed(((signed long)((1UL<<31)-1))+1, true);

fixed::fixed(void)
{
  fixed::value=0;
  fixed::isone=false;
}

fixed::fixed(const long &x)
{
  fixed::value=x;
  fixed::isone=false;
  //if(x==1) Serial.println("Did you mean 'one'?");
}

fixed::fixed(const long &x, const bool is_one)
{
  fixed::value=x;
  fixed::isone=is_one;
}

fixed::fixed(const lfixed &x)
{
  fixed::isone=false;
  if (x.value>=0x4000000000000000LL)
  {
    fixed::value=((signed long)((1UL<<31)-1))+1;
    fixed::isone=true;
  }
  else if (x.value<=-0x3FFFFFFF80000000LL)
    fixed::value=((signed long)((1UL<<31)-1))+2;
  else
    fixed::value=x.value>0?(x.value+0x40000000)>>31:-((-x.value+0x40000000)>>31);
}


bool fixed::operator==(const fixed &y) const
{return this->value==y.value ? true : false;}

bool fixed::operator!=(const fixed &y) const
{return this->value!=y.value ? true : false;}

bool fixed::operator>(const fixed &y) const
{return (((this->value>y.value)||(*this==one))&&(y!=one)) ? true : false;}

bool fixed::operator>=(const fixed &y) const
{return ((this->value>=y.value)&&(y!=one))||(*this==one) ? true : false;}

bool fixed::operator<(const fixed &y) const
{return (((this->value<y.value)||(y==one))&&(*this!=one)) ? true : false;}

bool fixed::operator<=(const fixed &y) const
{return ((this->value<=y.value)&&(*this!=one))||(y==one) ? true : false;}


fixed& fixed::operator+=(const fixed &y)
{
  this->value+=y.value;
  return *this;
}

fixed& fixed::operator-=(const fixed &y)
{
  this->value-=y.value;
  return *this;
}

/*fixed& fixed::operator*=(const fixed &y)
{
  uint32_t z;
  uint32_t tmp;
  uint8_t zero;
  if(y==one)
    return *this;
  else if(*this==one)
  {
    *this=y;
    return *this;
  }
  else if(*this==-one && y==-one)
  {
    *this=one;
    return *this;
  }
  else
    //z.value=(this->value>0) ? ((long long)(y.value)*((unsigned long)(this->value)<<1)+0x80000000)>>32 : -(((long long)(y.value)*((unsigned long)(-this->value)<<1)+0x80000000)>>32);
    asm (    // 160 cycles (10 usec)
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
    : [R]"=&r"(z), [T]"=&r"(tmp), [Z]"=&r"(zero)
    : [X]"a"(this->value), [Y]"a"(y.value)
    );

  this->value=z;
  return *this;
}*/

fixed& fixed::operator/=(const int y)
{
  if(*this==one)
    this->value=(one.value-1)/y;
  else
    this->value/=y;
  return *this;
}

fixed& fixed::operator<<=(const byte y)
{
  this->value<<=y;
  return *this;
}

fixed& fixed::operator>>=(const byte y)
{
  if(y==0)
    return *this;
  if(*this==one)
    this->value=1L<<(31-y);
  else
    this->value>>=y;
  return *this;
}


const fixed fixed::operator+(const fixed &y) const
{
  //return fixed(*this)+=y;
  return fixed(this->value+y.value);
}

const fixed fixed::operator-(const fixed &y) const
{
  //return fixed(*this)-=y;
  return fixed(this->value-y.value);
}

const fixed fixed::operator-(void) const
{
  return fixed((*this==one)? ((signed long)((1UL<<31)-1))+2 : -this->value);
}

const fixed fixed::operator*(const fixed &y) const //multiply and conquer!
{
  //return fixed(*this)*=y;
  fixed z;
  uint32_t tmp;
  uint8_t zero;
  if(y==one)
    return *this;
  else if(*this==one)
    return y;
  else if(*this==-one && y==-one)
    return one;
  else
    //z.value=(this->value>0) ? ((long long)(y.value)*((unsigned long)(this->value)<<1)+0x80000000)>>32 : -(((long long)(y.value)*((unsigned long)(-this->value)<<1)+0x80000000)>>32);
    asm (    // 160 cycles (10 usec)
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
    : [X]"a"(this->value), [Y]"a"(y.value)
    );
  return z;
}

const lfixed fixed::operator%(const fixed &y) const  //NOTE that it is NOT a division
{
  lfixed z;
  if (y==one && *this==one)
    z.value=1LL<<62;
  else if (y==one)
    z.value=(long long)this->value<<31;
  else if (*this==one)
    z.value=(long long)y.value<<31;
  else
    z.value=(long long)this->value*(long long)y.value;
  return z;
}

const fixed fixed::operator/(const int y) const
{
  //return fixed(*this)/=y;
  return fixed((*this==one) ? (one.value-1)/y : this->value/y);
}

const fixed fixed::operator<<(const byte y) const
{
  //return fixed(*this)<<=y;
  fixed z;
  z.value=this->value<<y;
  if(z<0 && *this>0)
    z=one;
  return z;
}

const fixed fixed::operator>>(const byte y) const
{
  //return fixed(*this)>>=y;
  fixed z;
  if(y==0)
    return *this;
  if(*this==one)
    z.value=1L<<(31-y);
  else
    z.value=this->value>>y;
  return z;
}



lfixed::lfixed(void)
{}

lfixed::lfixed(const long long &x)
{lfixed::value=x;}

lfixed::lfixed(const fixed &x)
{
  if(x==one)
    lfixed::value=0x4000000000000000LL;
  else if(x==-one)
    lfixed::value=-0x4000000000000000LL;
  else
    lfixed::value= x>0? ((long long)x.value)<<31:-(((long long)-x.value)<<31);
}


bool lfixed::operator==(const lfixed &y) const
{return this->value==y.value ? true : false;}

bool lfixed::operator!=(const lfixed &y) const
{return this->value!=y.value ? true : false;}

bool lfixed::operator>(const lfixed &y) const
{return this->value>y.value ? true : false;}

bool lfixed::operator>=(const lfixed &y) const
{return this->value>=y.value ? true : false;}

bool lfixed::operator<(const lfixed &y) const
{return this->value<y.value ? true : false;}

bool lfixed::operator<=(const lfixed &y) const
{return this->value<=y.value ? true : false;}


lfixed& lfixed::operator+=(const lfixed &y)
{
  this->value+=y.value;
  return *this;
}

lfixed& lfixed::operator-=(const lfixed &y)
{
  this->value-=y.value;
  return *this;
}

struct ufmultparams
{
  unsigned long x;
  unsigned long y;
  unsigned long resA;
  unsigned long resB;
};

void ufmult_asm(struct ufmultparams *p)
{
  uint8_t zero;
  
  asm (
    "clr %[Z] \n\t"
    "fmul %D[X], %D[Y] \n\t"
    "movw %C[R], r0 \n\t"
    "fmulsu %B[X], %D[Y]  \n\t"
    "adc %C[R], %[Z]  \n\t"
    "adc %D[R], %[Z]  \n\t"
    "movw %A[R], r0  \n\t"
    "fmul %C[X], %D[Y]  \n\t"
    "adc %D[R], %[Z]  \n\t"
    "add %B[R], r0  \n\t"
    "adc %C[R], r1  \n\t"
    "adc %D[R], %[Z]  \n\t"
    "fmul %A[X], %D[Y]  \n\t"
    "adc %B[R], %[Z]  \n\t"
    "adc %C[R], %[Z]  \n\t"
    "adc %D[R], %[Z]  \n\t"
    "mov %D[T], r0  \n\t"
    "add %A[R], r1  \n\t"
    "adc %B[R], %[Z]  \n\t"
    "adc %C[R], %[Z]  \n\t"
    "adc %D[R], %[Z]  \n\t"
    "fmul %D[X], %C[Y]  \n\t"
    "adc %D[R], %[Z]  \n\t"
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
    "fmul %D[X], %B[Y]  \n\t"
    "adc %C[R], %[Z]  \n\t"
    "adc %D[R], %[Z]  \n\t"
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
    "fmul %D[X], %A[Y]  \n\t"
    "adc %B[R], %[Z]  \n\t"
    "adc %C[R], %[Z]  \n\t"
    "adc %D[R], %[Z]  \n\t"
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
    "mov %A[T], r0  \n\t"
    "add %B[T], r1  \n\t"
    "adc %C[T], %[Z]  \n\t"
    "adc %D[T], %[Z]  \n\t"
    "adc %A[R], %[Z]  \n\t"
    "adc %B[R], %[Z]  \n\t"
    "adc %C[R], %[Z]  \n\t"
    "adc %D[R], %[Z]  \n\t"
    "clr r1  \n\t"
    : [R]"=&r"(p->resA), [T]"=&r"(p->resB), [Z]"=&r"(zero)
    : [X]"a"(p->x), [Y]"a"(p->y)
    );
}

const unsigned long long ufmult(const unsigned long &x, const unsigned long &y)
{
  struct ufmultparams p;
  p.x=x;
  p.y=y;
  ufmult_asm(&p);       // this wrapper is a work around buggy gcc register allocator http://gcc.gnu.org/bugzilla/show_bug.cgi?id=56479
  return (((unsigned long long)p.resA)<<32) + p.resB;
}

/*void print(const char *, fixed);
void print(const char *, lfixed);

const unsigned long long ufmult(const unsigned long x, const unsigned long y)
{
  unsigned long long a=x;
  unsigned long long b=y;
  unsigned long long c=a*b;

//  print("ufmult.x",(fixed)x);
//  print("ufmult.y",(fixed)y);
//  print("ufmult.a",(lfixed)a);
//  print("ufmult.b",(lfixed)b);
//  print("ufmult.c",(lfixed)c);
//  print("ufmult",(lfixed)(c>>1));
  
  return c<<1;
}*/

lfixed& lfixed::operator*=(const lfixed &y)
{
  byte sign=0;
  lfixed z;
  unsigned long a, b, c, d;
  unsigned long long r, m, xv, yv;
  
  if(this->value<0)
  {
    sign^=1;
    xv=-this->value;
  }
  else
    xv=this->value;
  if(y.value<0)
  {
    sign^=1;
    yv=-y.value;
  }
  else
    yv=y.value;
  
  a=xv>>32;
  b=yv>>32;
  z.value=ufmult(a,b);
  
//  print("z", z);
  
  a=xv-(((unsigned long long)a)<<32);
  b=yv-(((unsigned long long)b)<<32);
  
  r=ufmult(a,b);
  
  a=xv>>32;
    
  m=ufmult(a,b);
  c=m>>32;
  d=m-(((unsigned long long)c)<<32);
  
  z.value=z.value+c;
  if(d&0x80000000UL && r&0x8000000000000000ULL)
    z.value=z.value+1;
  
  r+=(unsigned long long)d<<32;
  
//  print("z", z);
  
  a=xv-(((unsigned long long)a)<<32);
  b=yv-(((unsigned long long)yv>>32)<<32);
  
  m=ufmult(a,b);
  c=m>>32;
  d=m-(((unsigned long long)c)<<32);
  
  z.value=z.value+c;
  if(d&0x80000000UL && r&0x8000000000000000ULL)
    z.value=z.value+1;
  
  r+=(unsigned long long)d<<32;

//  print("z", z);
  
  z.value=z.value<<1;
  if(r&0x8000000000000000ULL)
    z.value=z.value+1;
  
  *this=sign?-z:z;

  return *this;
}

lfixed& lfixed::operator<<=(const byte y)
{
  this->value<<=y;
  return *this;
}

lfixed& lfixed::operator>>=(const byte y)
{
  this->value>>=y;
  return *this;
}


const lfixed lfixed::operator+(const lfixed &y) const
{
  //return lfixed(*this)+=y;
  return lfixed(this->value+y.value);
}

const lfixed lfixed::operator-(const lfixed &y) const
{
  //return lfixed(*this)-=y;
  return lfixed(this->value-y.value);
}

const lfixed lfixed::operator-(void) const
{
  return lfixed(-this->value);
}

const lfixed lfixed::operator*(const lfixed &y) const
{
  //return lfixed(*this)*=y;
  byte sign=0;
  lfixed z;
  unsigned long a, b, c, d;
  unsigned long long r, m, xv, yv;
  
  if(this->value<0)
  {
    sign^=1;
    xv=-this->value;
  }
  else
    xv=this->value;
  if(y.value<0)
  {
    sign^=1;
    yv=-y.value;
  }
  else
    yv=y.value;
  
  a=xv>>32;
  b=yv>>32;
  z.value=ufmult(a,b);
  
//  print("z", z);
  
  a=xv-(((unsigned long long)a)<<32);
  b=yv-(((unsigned long long)b)<<32);
  
  r=ufmult(a,b);
  
  a=xv>>32;
    
  m=ufmult(a,b);
  c=m>>32;
  d=m-(((unsigned long long)c)<<32);
  
  z.value=z.value+c;
  if(d&0x80000000UL && r&0x8000000000000000ULL)
    z.value=z.value+1;
  
  r+=(unsigned long long)d<<32;
  
//  print("z", z);
  
  a=xv-(((unsigned long long)a)<<32);
  b=yv-(((unsigned long long)yv>>32)<<32);
  
  m=ufmult(a,b);
  c=m>>32;
  d=m-(((unsigned long long)c)<<32);
  
  z.value=z.value+c;
  if(d&0x80000000UL && r&0x8000000000000000ULL)
    z.value=z.value+1;
  
  r+=(unsigned long long)d<<32;

//  print("z", z);
  
  z.value=z.value<<1;
  if(r&0x8000000000000000ULL)
    z.value=z.value+1;
  
  return sign?-z:z;
}

/*const lfixed lfixed::operator*(const lfixed &y) const
{
  byte sign=0;
  lfixed z=0;
  unsigned long long xv, yv;
  if(this->value<0)
  {
    sign^=1;
    xv=-this->value;
  }
  else
    xv=this->value;
  if(y.value<0)
  {
    sign^=1;
    yv=-y.value;
  }
  else
    yv=y.value;

//  for(byte i=62; i!=255; i--)
//  {
//    if(xv&(1ULL<<i))
  for(unsigned long long i=1ULL<<62; i!=0; i=i>>1)
  {
    if(xv&i)
      z.value+=yv;
    yv=yv>>1;
  }
  return sign?-z:z;
}*/

const fixed lfixed::operator/(const fixed &y) const
{
  fixed z;
  if(y==one)
    z.value=this->value>>31;
  else
    {z.value=this->value/y.value;
    if (fixed(z.value)==one && this->value<0)
      z.value=one.value+1;
    }
  return z;
}

const fixed lfixed::operator/(const lfixed &y) const
{
  fixed z;
  unsigned long long a,b;
  byte sign=0;
//  if(y.value==0)
//    return one;
  if(this->value<0)
  {
    a=-this->value;
    sign^=1;
  }
  else
    a=this->value;
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
  for(byte i=30; i!=255; i--)
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

const lfixed lfixed::operator<<(const byte y) const
{
  //return lfixed(*this)<<=y;
  return lfixed(this->value<<y);
}

const lfixed lfixed::operator>>(const byte y) const
{
  //return lfixed(*this)>>=y;
  return lfixed(this->value>>y);
}



const lfixed ldiv(const lfixed &x, const lfixed &y)
{
  lfixed z;
  unsigned long long a,b;
  byte sign=0;
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
  for(byte i=62; i!=255; i--)
  {
    if(b<a)
    {
      z.value|=1ULL<<i;
      a-=b;
    }
    a<<=1;
  }
  if (b<a)
    z.value++; //rounding
  return sign?-z:z;
}

inline const fixed pow(const fixed &a, const byte i)
{
  switch(i)
  {
    case 0:
      return one;
    case 1:
      return a;
    case 2:
      return a*a;
    case 3:
      return a*a*a;
    case 4:
      return sq(a*a);
    default:
      return sq(a*a)*pow(a, i-4);
  }
}

inline const lfixed lsq(const fixed &x)
{
  return x%x;
}

//   I used the following code to generate the below lookup table:
void printSqrtTable(void)
{
  Serial.print("const byte l_sqrt[192]={		// input: x (64-255), output: sqrt((x-64)<<8)\n");
  for(int i=64; i<256; i++)
  {
    Serial.print((unsigned byte)sqrt(i<<8));
    if(i!=255)
      Serial.print(",\n");
    else
      Serial.print("};\n");
  }
}

const byte l_sqrt[192]={		// input: x (64-255), output: sqrt((x-64)<<8)
128,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,144,145,146,147,148,149,150,150,151,152,153,154,155,155,
156,157,158,159,160,160,161,162,163,163,164,165,166,167,167,168,169,170,170,171,172,173,173,174,175,176,176,177,178,178,179,180,
181,181,182,183,183,184,185,185,186,187,187,188,189,189,190,191,192,192,193,193,194,195,195,196,197,197,198,199,199,200,201,201,
202,203,203,204,204,205,206,206,207,208,208,209,209,210,211,211,212,212,213,214,214,215,215,216,217,217,218,218,219,219,220,221,
221,222,222,223,224,224,225,225,226,226,227,227,228,229,229,230,230,231,231,232,232,233,234,234,235,235,236,236,237,237,238,238,
239,240,240,241,241,242,242,243,243,244,244,245,245,246,246,247,247,248,248,249,249,250,250,251,251,252,252,253,253,254,254,255
};

const unsigned long usqrt(const lfixed &x)
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

const fixed sqrt(const lfixed &x)
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

const lfixed lsqrt(const lfixed &x)
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
