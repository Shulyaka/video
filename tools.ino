bool IsLinkedLine(byte x1, byte y1, byte x2, byte y2)
{
  byte i, j;
  i=median(x1, x2);
  j=median(y1, y2);
  if((!tv.get_pixel(i, j)) && !((i>0)&&tv.get_pixel(i-1, j)) && !((j>0) && tv.get_pixel(i, j-1)) && !((i<W-1) && tv.get_pixel(i+1, j)) && !((j<H-1) && tv.get_pixel(i, j+1)))  //check the nearest also
    return false;
  if(((i==x1)&&(j==y1))||((i==x2)&&(j==y2))) //stop condition
    return true;
  return IsLinkedLine(i, j, x2, y2) && IsLinkedLine(x1, y1, i, j);
}

byte length_dr(byte x, byte y, byte *a, byte *b)
{
  byte i=x, j=y, length=0;
  while ((i<W-1)&&(j<H-1))
  {
    if(tv.get_pixel(i, j+1) && IsLinkedLine(x, y, i, j+1))
    {
      j++;
      length++;
    }
    else if(tv.get_pixel(i+1, j) && IsLinkedLine(x, y, i+1, j))
    {
      i++;
      length++;
    }
    else
      break;
  }
  *a=i;
  *b=j;
  return length;
}

byte length_rd(byte x, byte y, byte *a, byte *b)
{
  byte i=x, j=y, length=0;
  while ((i<W-1)&&(j<H-1))
  {
    if(tv.get_pixel(i+1, j) && IsLinkedLine(x, y, i+1, j))
    {
      i++;
      length++;
    }
    else if(tv.get_pixel(i, j+1) && IsLinkedLine(x, y, i, j+1))
    {
      j++;
      length++;
    }
    else
      break;
  }
  *a=i;
  *b=j;
  return length;
}

byte length_dl(byte x, byte y, byte *a, byte *b)
{
  byte i=x, j=y, length=0;
  while ((i>0)&&(j<H-1))
  {
    if(tv.get_pixel(i, j+1) && IsLinkedLine(x, y, i, j+1))
    {
      j++;
      length++;
    }
    else if(tv.get_pixel(i-1, j) && IsLinkedLine(x, y, i-1, j))
    {
      i--;
      length++;
    }
    else
      break;
  }
  *a=i;
  *b=j;
  return length;
}

byte length_ld(byte x, byte y, byte *a, byte *b)
{
  byte i=x, j=y, length=0;
  while ((i>0)&&(j<H-1))
  {
    if(tv.get_pixel(i-1, j) && IsLinkedLine(x, y, i-1, j))
    {
      i--;
      length++;
    }
    else if(tv.get_pixel(i, j+1) && IsLinkedLine(x, y, i, j+1))
    {
      j++;
      length++;
    }
    else
      break;
  }
  *a=i;
  *b=j;
  return length;
}

byte length_ul(byte x, byte y, byte *a, byte *b)
{
  byte i=x, j=y, length=0;
  while ((i>0)&&(j>0))
  {
    if(tv.get_pixel(i, j-1) && IsLinkedLine(x, y, i, j-1))
    {
      j--;
      length++;
    }
    else if(tv.get_pixel(i-1, j) && IsLinkedLine(x, y, i-1, j))
    {
      i--;
      length++;
    }
    else
      break;
  }
  *a=i;
  *b=j;
  return length;
}

byte length_lu(byte x, byte y, byte *a, byte *b)
{
  byte i=x, j=y, length=0;
  while ((i>0)&&(j>0))
  {
    if(tv.get_pixel(i-1, j) && IsLinkedLine(x, y, i-1, j))
    {
      i--;
      length++;
    }
    else if(tv.get_pixel(i, j-1) && IsLinkedLine(x, y, i, j-1))
    {
      j--;
      length++;
    }
    else
      break;
  }
  *a=i;
  *b=j;
  return length;
}

byte length_ur(byte x, byte y, byte *a, byte *b)
{
  byte i=x, j=y, length=0;
  while ((i<W-1)&&(j>0))
  {
    if(tv.get_pixel(i, j-1) && IsLinkedLine(x, y, i, j-1))
    {
      j--;
      length++;
    }
    else if(tv.get_pixel(i+1, j) && IsLinkedLine(x, y, i+1, j))
    {
      i++;
      length++;
    }
    else
      break;
  }
  *a=i;
  *b=j;
  return length;
}

byte length_ru(byte x, byte y, byte *a, byte *b)
{
  byte i=x, j=y, length=0;
  while ((i<W-1)&&(j>0))
  {
    if(tv.get_pixel(i+1, j) && IsLinkedLine(x, y, i+1, j))
    {
      i++;
      length++;
    }
    else if(tv.get_pixel(i, j-1) && IsLinkedLine(x, y, i, j-1))
    {
      j--;
      length++;
    }
    else
      break;
  }
  *a=i;
  *b=j;
  return length;
}

byte length_r(byte x, byte y, byte *a, byte *b)
{
  byte i=x, length=0;
  while (i<W-1)
  {
    if(tv.get_pixel(i+1, y) && IsLinkedLine(x, y, i+1, y))
    {
      i++;
      length++;
    }
    else
      break;
  }
  *a=i;
  *b=y;
  return length;
}

byte length_l(byte x, byte y, byte *a, byte *b)
{
  byte i=x, length=0;
  while (i>0)
  {
    if(tv.get_pixel(i-1, y) && IsLinkedLine(x, y, i-1, y))
    {
      i--;
      length++;
    }
    else
      break;
  }
  *a=i;
  *b=y;
  return length;
}

inline byte median(byte a, byte b)
{return (b>a)?((b-a)>>1)+a:((a-b)>>1)+b;}

inline byte dif(byte a, byte b)
{return (b>a)?b-a:a-b;}

byte move_dr(byte x, byte y, byte *a, byte *b)
{
  byte i=x, j=y, length=0;
  while ((i<W-1)&&(j<H-1))
  {
    if(tv.get_pixel(i, j+1))
    {
      j++;
      length++;
    }
    else if(tv.get_pixel(i+1, j))
    {
      i++;
      length++;
    }
    else
      break;
  }
  *a=i;
  *b=j;
  return length;
}

byte move_rd(byte x, byte y, byte *a, byte *b)
{
  byte i=x, j=y, length=0;
  while ((i<W-1)&&(j<H-1))
  {
    if(tv.get_pixel(i+1, j))
    {
      i++;
      length++;
    }
    else if(tv.get_pixel(i, j+1))
    {
      j++;
      length++;
    }
    else
      break;
  }
  *a=i;
  *b=j;
  return length;
}

byte move_dl(byte x, byte y, byte *a, byte *b)
{
  byte i=x, j=y, length=0;
  while ((i>0)&&(j<H-1))
  {
    if(tv.get_pixel(i, j+1))
    {
      j++;
      length++;
    }
    else if(tv.get_pixel(i-1, j))
    {
      i--;
      length++;
    }
    else
      break;
  }
  *a=i;
  *b=j;
  return length;
}

byte move_ld(byte x, byte y, byte *a, byte *b)
{
  byte i=x, j=y, length=0;
  while ((i>0)&&(j<H-1))
  {
    if(tv.get_pixel(i-1, j))
    {
      i--;
      length++;
    }
    else if(tv.get_pixel(i, j+1))
    {
      j++;
      length++;
    }
    else
      break;
  }
  *a=i;
  *b=j;
  return length;
}

byte move_ul(byte x, byte y, byte *a, byte *b)
{
  byte i=x, j=y, length=0;
  while ((i>0)&&(j>0))
  {
    if(tv.get_pixel(i, j-1))
    {
      j--;
      length++;
    }
    else if(tv.get_pixel(i-1, j))
    {
      i--;
      length++;
    }
    else
      break;
  }
  *a=i;
  *b=j;
  return length;
}

byte move_lu(byte x, byte y, byte *a, byte *b)
{
  byte i=x, j=y, length=0;
  while ((i>0)&&(j>0))
  {
    if(tv.get_pixel(i-1, j))
    {
      i--;
      length++;
    }
    else if(tv.get_pixel(i, j-1))
    {
      j--;
      length++;
    }
    else
      break;
  }
  *a=i;
  *b=j;
  return length;
}

byte move_ur(byte x, byte y, byte *a, byte *b)
{
  byte i=x, j=y, length=0;
  while ((i<W-1)&&(j>0))
  {
    if(tv.get_pixel(i, j-1))
    {
      j--;
      length++;
    }
    else if(tv.get_pixel(i+1, j))
    {
      i++;
      length++;
    }
    else
      break;
  }
  *a=i;
  *b=j;
  return length;
}

byte move_ru(byte x, byte y, byte *a, byte *b)
{
  byte i=x, j=y, length=0;
  while ((i<W-1)&&(j>0))
  {
    if(tv.get_pixel(i+1, j))
    {
      i++;
      length++;
    }
    else if(tv.get_pixel(i, j-1))
    {
      j--;
      length++;
    }
    else
      break;
  }
  *a=i;
  *b=j;
  return length;
}

