bool IsLinkedLine(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2)
{
  unsigned char i, j;
  i=median(x1, x2);
  j=median(y1, y2);
  if((!tv.get_pixel(i, j)) && !((i>0)&&tv.get_pixel(i-1, j)) && !((j>0) && tv.get_pixel(i, j-1)) && !((i<W-1) && tv.get_pixel(i+1, j)) && !((j<H-1) && tv.get_pixel(i, j+1)))  //check the nearest also
    return false;
  if(((i==x1)&&(j==y1))||((i==x2)&&(j==y2))) //stop condition
    return true;
  return IsLinkedLine(i, j, x2, y2) && IsLinkedLine(x1, y1, i, j);
}

unsigned char length_dr(unsigned char x, unsigned char y, unsigned char *a, unsigned char *b)
{
  unsigned char i=x, j=y, length=0;
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

unsigned char length_rd(unsigned char x, unsigned char y, unsigned char *a, unsigned char *b)
{
  unsigned char i=x, j=y, length=0;
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

unsigned char length_dl(unsigned char x, unsigned char y, unsigned char *a, unsigned char *b)
{
  unsigned char i=x, j=y, length=0;
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

unsigned char length_ld(unsigned char x, unsigned char y, unsigned char *a, unsigned char *b)
{
  unsigned char i=x, j=y, length=0;
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

unsigned char length_ul(unsigned char x, unsigned char y, unsigned char *a, unsigned char *b)
{
  unsigned char i=x, j=y, length=0;
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

unsigned char length_lu(unsigned char x, unsigned char y, unsigned char *a, unsigned char *b)
{
  unsigned char i=x, j=y, length=0;
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

unsigned char length_ur(unsigned char x, unsigned char y, unsigned char *a, unsigned char *b)
{
  unsigned char i=x, j=y, length=0;
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

unsigned char length_ru(unsigned char x, unsigned char y, unsigned char *a, unsigned char *b)
{
  unsigned char i=x, j=y, length=0;
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

unsigned char length_r(unsigned char x, unsigned char y, unsigned char *a, unsigned char *b)
{
  unsigned char i=x, length=0;
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

unsigned char length_l(unsigned char x, unsigned char y, unsigned char *a, unsigned char *b)
{
  unsigned char i=x, length=0;
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

inline unsigned char median(unsigned char a, unsigned char b)
{return (b>a)?((b-a)>>1)+a:((a-b)>>1)+b;}

inline unsigned char dif(unsigned char a, unsigned char b)
{return (b>a)?b-a:a-b;}

unsigned char move_dr(unsigned char x, unsigned char y, unsigned char *a, unsigned char *b)
{
  unsigned char i=x, j=y, length=0;
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

unsigned char move_rd(unsigned char x, unsigned char y, unsigned char *a, unsigned char *b)
{
  unsigned char i=x, j=y, length=0;
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

unsigned char move_dl(unsigned char x, unsigned char y, unsigned char *a, unsigned char *b)
{
  unsigned char i=x, j=y, length=0;
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

unsigned char move_ld(unsigned char x, unsigned char y, unsigned char *a, unsigned char *b)
{
  unsigned char i=x, j=y, length=0;
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

unsigned char move_ul(unsigned char x, unsigned char y, unsigned char *a, unsigned char *b)
{
  unsigned char i=x, j=y, length=0;
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

unsigned char move_lu(unsigned char x, unsigned char y, unsigned char *a, unsigned char *b)
{
  unsigned char i=x, j=y, length=0;
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

unsigned char move_ur(unsigned char x, unsigned char y, unsigned char *a, unsigned char *b)
{
  unsigned char i=x, j=y, length=0;
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

unsigned char move_ru(unsigned char x, unsigned char y, unsigned char *a, unsigned char *b)
{
  unsigned char i=x, j=y, length=0;
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

