#ifndef QUATERNIONLIB_H
#define QUATERNIONLIB_H

//#include "fplib.h"

class quaternion
{
  public:
  quaternion(void);
  quaternion(const fixed&, const fixed&, const fixed&, const fixed&);
  quaternion(const fixed&, const fixed&, const fixed&);
  quaternion& normalize(void);
  fixed w, x, y, z;

  bool operator==(const quaternion&) const;
  bool operator!=(const quaternion&) const;
  
  quaternion& operator+=(const quaternion&);
  quaternion& operator-=(const quaternion&);
  quaternion& operator*=(const quaternion&);
  quaternion& operator*=(const fixed&);
  quaternion& operator/=(const fixed&);
  quaternion& operator<<=(const byte);
  quaternion& operator>>=(const byte);

  const quaternion operator+(const quaternion&) const;
  const quaternion operator-(const quaternion&) const;
  const quaternion operator-(void) const;
  const quaternion operator*(const quaternion&) const;
  const quaternion operator*(const fixed&) const;
  const quaternion operator/(const fixed&) const;
  const quaternion operator<<(const byte) const;
  const quaternion operator>>(const byte) const;
};

const quaternion ident=quaternion(one, 0, 0, 0);

quaternion::quaternion(void)
{
  quaternion::w=one;
  quaternion::x=0;
  quaternion::y=0;
  quaternion::z=0;
}

quaternion::quaternion(const fixed &w, const fixed &x, const fixed &y, const fixed &z)
{
  quaternion::w=w;
  quaternion::x=x;
  quaternion::y=y;
  quaternion::z=z;
}

quaternion::quaternion(const fixed &x, const fixed &y, const fixed &z)
{
  quaternion::w=0;
  quaternion::x=x;
  quaternion::y=y;
  quaternion::z=z;
}

bool quaternion::operator==(const quaternion &b) const
{return (this->w==b.w && this->x==b.x && this->y==b.y && this->z==b.z) ? true : false;}

bool quaternion::operator!=(const quaternion &b) const
{return (this->w!=b.w || this->x!=b.x || this->y!=b.y || this->z!=b.z) ? true : false;}

quaternion& quaternion::operator+=(const quaternion &b)
{
  this->w+=b.w;
  this->x+=b.x;
  this->y+=b.y;
  this->z+=b.z;
  return *this;
}

quaternion& quaternion::operator-=(const quaternion &b)
{
  this->w-=b.w;
  this->x-=b.x;
  this->y-=b.y;
  this->z-=b.z;
  return *this;
}

quaternion& quaternion::operator*=(const quaternion &b)
{
  *this=quaternion(this->w*b.w - this->x*b.x - this->y*b.y - this->z*b.z,
                   this->y*b.z - this->z*b.y  +this->w*b.x + this->x*b.w,
                   this->z*b.x - this->x*b.z + this->w*b.y + this->y*b.w,
                   this->x*b.y - this->y*b.x + this->w*b.z + this->z*b.w);
  return *this;
}

quaternion& quaternion::operator*=(const fixed &b)
{
/*  this->w*=b;
  this->x*=b;
  this->y*=b;
  this->z*=b;
*/
  this->w=this->w*b;
  this->x=this->x*b;
  this->y=this->y*b;
  this->z=this->z*b;
  return *this;
}

quaternion& quaternion::operator/=(const fixed &b)
{
  this->w=lfixed(this->w)/b;
  this->x=lfixed(this->x)/b;
  this->y=lfixed(this->y)/b;
  this->z=lfixed(this->z)/b;
  return *this;
}



quaternion& quaternion::operator>>=(const byte b)
{
  this->w>>=b;
  this->x>>=b;
  this->y>>=b;
  this->z>>=b;
  return *this;
}

quaternion& quaternion::operator<<=(const byte b)
{
  this->w<<=b;
  this->x<<=b;
  this->y<<=b;
  this->z<<=b;
  return *this;
}


const quaternion quaternion::operator+(const quaternion &b) const
{
  //return quaternion(*this)+=b;
  return quaternion(this->w+b.w, this->x+b.x, this->y+b.y, this->z+b.z);
}

const quaternion quaternion::operator-(const quaternion &b) const
{
  //return quaternion(*this)-=b;
  return quaternion(this->w-b.w, this->x-b.x, this->y-b.y, this->z-b.z);
}

const quaternion quaternion::operator-(void) const //see also conjugate()
{
  return quaternion(-this->w, -this->x, -this->y, -this->z);
}

const quaternion quaternion::operator*(const quaternion &b) const
{
  return quaternion(*this)*=b;
  //return quaternion(this->w*b.w - this->x*b.x - this->y*b.y - this->z*b.z,
  //                  this->y*b.z - this->z*b.y  +this->w*b.x + this->x*b.w,
  //                  this->z*b.x - this->x*b.z + this->w*b.y + this->y*b.w,
  //                  this->x*b.y - this->y*b.x + this->w*b.z + this->z*b.w);
}

const quaternion quaternion::operator*(const fixed &b) const
{
  //return quaternion(*this)*=b;
  return quaternion(this->w*b, this->x*b, this->y*b, this->z*b);
}

const quaternion quaternion::operator/(const fixed &b) const
{
  //return quaternion(*this)/=b;
  return quaternion(lfixed(this->w)/b, lfixed(this->x)/b, lfixed(this->y)/b, lfixed(this->z)/b);
}

const quaternion quaternion::operator>>(const byte b) const
{
  //return quaternion(*this)>>=b;
  return quaternion(this->w>>b, this->x>>b, this->y>>b, this->z>>b);
}

const quaternion quaternion::operator<<(const byte b) const
{
  //return quaternion(*this)<<=b;
  return quaternion(this->w<<b, this->x<<b, this->y<<b, this->z<<b);
}


const fixed norm(const quaternion &a)
{return sq(a.w)+sq(a.x)+sq(a.y)+sq(a.z);}

const lfixed lnorm(const quaternion &a)
{return lsq(a.w)+lsq(a.x)+lsq(a.y)+lsq(a.z);}

const lfixed lmagnitude(const quaternion &a)
{return lsqrt(lnorm(a));}

quaternion& quaternion::normalize(void)
{
  lfixed nm=lmagnitude(*this);
  this->w=lfixed(this->w)/nm;
  this->x=lfixed(this->x)/nm;
  this->y=lfixed(this->y)/nm;
  this->z=lfixed(this->z)/nm;
  
  if(this->w<0)
  {
    this->w=-this->w;
    this->x=-this->x;
    this->y=-this->y;
    this->z=-this->z;
  }
  return *this;
}

const fixed magnitude(const quaternion &a)
{return sqrt(lnorm(a));}

const quaternion conjugate(const quaternion &a)
{
  return quaternion(a.w, -a.x, -a.y, -a.z);
}

const quaternion inverse(const quaternion &a)
{return conjugate(a)/norm(a);}

const fixed inner(const quaternion &a, const quaternion &b)
{return a.w*b.w + a.x*b.x + a.y*b.y + a.z*b.z;}

const quaternion sqrt(const quaternion &a)
{
  //return (a>>1)+(ident>>1);
  return quaternion((a.w>>1)+(one>>1), a.x>>1, a.y>>1, a.z>>1).normalize();
}

#endif
