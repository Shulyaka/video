void FilterNoise(void)  //filters the noise in the frame
{
  unsigned char c, n, cp, cn;
  for (unsigned char x=1; x<W-1; x++)
    for (unsigned char y=1; y<H-1; y++)
    {
      if(y!=1)
        cp=c;
      else
        cp=tv.get_pixel(x, 0);
      c=tv.get_pixel(x, y);
      n=tv.get_pixel(x-1, y)+tv.get_pixel(x+1, y)+cp+tv.get_pixel(x, y+1);
      if((c==0)&&(n>2))
        tv.set_pixel(x,y,1);
      else if((c==1)&&(n<2))
        tv.set_pixel(x,y,0);
    }
}

void ClearArea(unsigned char x, unsigned char y)  //Clears the linked area recursively
{                                                 //the function may be optimized to exclude bound search in certain cases
  unsigned char r, i;                             //stack usage may also be optimized
  for(r=x; (r<W)&&(tv.get_pixel(r, y)); r++)  //find the right bound
    continue;                                     //the bound search may also be optimized by probing by a batch of 8 points, might require an enhancement of tvout library

  if(r==x)
    return; //pixel already 0

  for(x--; (x!=255)&&(tv.get_pixel(x, y)); x--)  //find the left bound
    continue;
  
  tv.draw_row(y, ++x, r--, 0);                      //was: "--r", might be a bug in TVout.cpp:400, may need to change to "rbit = ~(0xff >> ((x1+1)&7));", to check later.

  if(++y!=H)
  {
    for(i=r-1; (i>=x)&&(i<W); i--)
      if(!tv.get_pixel(i, y))
      {
        if(tv.get_pixel(i+1, y))
          ClearArea(i+1, y);
        else
          i--;
      }
  
    if(tv.get_pixel(x, y))
      ClearArea(x, y);
  }

  if((y-=2)!=255)
  {
    for(i=r-1; (i>=x)&&(i<W); i--)
      if(!tv.get_pixel(i, y))
      {
        if(tv.get_pixel(i+1, y))
          ClearArea(i+1, y);
        else
          i--;
      }
  
    if(tv.get_pixel(x, y))
      ClearArea(x, y);
  }

  return;
}

bool MatchX(unsigned char x, unsigned char y)  //returns true if the area is a cross (X)
{
  unsigned char lengtha, lengthb;
  unsigned char i, j, ax, ay, bx, by;
  
  lengtha=length_rd(x, y, &ax, &ay);

  lengthb=length_ld(x, y, &bx, &by);

  Serial.print("(");
  Serial.print(lengtha);
  Serial.print(", ");
  Serial.print(lengthb);
  Serial.println(")");
  
  if(lengtha>>2 > lengthb)
  {
    if((length_ld(median(x,median(x,ax)), median(y,median(y,ay)), &i, &j) + length_ru(median(x,ax), median(y,ay), &i, &j))>lengtha>>1)
      return false;
    if((length_ld(median(median(x,ax),ax), median(median(y,ay),ay), &i, &j) + length_ru(median(x,ax), median(y,ay), &i, &j))>lengtha>>1)
      return false;    
    return true;
  }
  
  if(lengthb>>2 > lengtha)
  {
    if((length_rd(median(x,median(x,bx)), median(y,median(y,by)), &i, &j) + length_lu(median(x,bx), median(y,by), &i, &j))>(lengthb>>1))
      return false;
    if((length_rd(median(median(x,bx),bx), median(median(y,by),by), &i, &j) + length_lu(median(x,bx), median(y,by), &i, &j))>(lengthb>>1))
      return false;
    return true;
  }
  return false;
}

/*
bool IsLinkedLine(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2)
{
  unsigned char i;
  if(abs(x2-x1)>abs(y2-y1))
  {
    if(x1>x2)
      return IsLinkedLine(x2, y2, x1, y1);
    
    
    
  }
  else
  {
    if(y1>y2)
      return IsLinkedLine(x2, y2, x1, y1);
    
  }
}
*/

bool IsLinkedLine(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2)
{
  unsigned char i, j;
//  return true;
  i=median(x1, x2);
  j=median(y1, y2);
/*
  Serial.print("(");
  Serial.print(x1);
  Serial.print(", ");
  Serial.print(y1);
  Serial.print(") - (");
  Serial.print(i);
  Serial.print(", ");
  Serial.print(j);
  Serial.print(") - (");
  Serial.print(x2);
  Serial.print(", ");
  Serial.print(y2);
  Serial.println(")");
*/
  if((!tv.get_pixel(i, j)) && !((i>0)&&tv.get_pixel(i-1, j)) && !((j>0) && tv.get_pixel(i, j-1)) && !((i+1<W) && tv.get_pixel(i+1, j)) && !((j+1<H) && tv.get_pixel(i, j+1)))  //check the nearest also
    return false;
  if(((i==x1)&&(j==y1))||((i==x2)&&(j==y2))) //stop condition
    return true;
  return IsLinkedLine(i, j, x2, y2) && IsLinkedLine(x1, y1, i, j);
}

unsigned char length_rd(unsigned char x, unsigned char y, unsigned char *a, unsigned char *b)
{
  unsigned char i=x, j=y, length=0;
  while ((i<W-1)&&(j<H-1))
  {
/*    Serial.print("(");
    Serial.print(i);
    Serial.print(", ");
    Serial.print(j);
    Serial.println(")");
*/
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

unsigned char length_ld(unsigned char x, unsigned char y, unsigned char *a, unsigned char *b)
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

unsigned char length_lu(unsigned char x, unsigned char y, unsigned char *a, unsigned char *b)
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

unsigned char length_ru(unsigned char x, unsigned char y, unsigned char *a, unsigned char *b)
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

inline unsigned char median(unsigned char a, unsigned char b)
{return (b>a)?((b-a)>>1)+a:((a-b)>>1)+b;}

