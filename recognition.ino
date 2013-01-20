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

bool MatchX(unsigned char x, unsigned char y, unsigned char *cx, unsigned char *cy)  //returns true if the area is a cross (X)
{                                              //the function assumes that we are close to the top of the object
  unsigned char lengtha, lengthb;
  unsigned char i, j, ax, ay, bx, by, x0, y0, x3, y3, x6, y6, x9, y9;

  lengtha=length_dr(x, y, &ax, &ay) + length_ul(x, y, &x, &y);

  lengthb=length_dl(x, y, &bx, &by) + length_ur(x, y, &i, &j);

  Serial.print("length (");
  Serial.print(lengtha);
  Serial.print(", ");
  Serial.print(lengthb);
  Serial.println(")");
  
  if(lengtha>>2 > lengthb) //left-upper corner of X
  {
    /*
    if((length_ld(median(x,median(x,ax)), median(y,median(y,ay)), &i, &j) + length_ru(median(x,median(x,ax)), median(y,median(y,ay)), &i, &j))>lengtha>>2)
    {
      Serial.println("a");
      return false;
    }
    if((length_ld(median(median(x,ax),ax), median(median(y,ay),ay), &i, &j) + length_ru(median(median(x,ax),ax), median(median(y,ay),ay), &i, &j))>lengtha>>2)
    {
      Serial.println("b");
      return false;
    }
    return true;
    */

//    lengthb=FindXCenter_ru(x, y, ax, ay, lengtha, cx, cy);
//    Serial.println(lengthb);
//    if((lengthb==1)||(lengthb==2)||(lengthb==4))
//      return true;
//    return false;
    x0=x;
    y0=y;
    x6=ax;
    y6=ay;
    move_rd(x0, y0, &i, &j);
    move_ru(i, j, &x3, &y3);
    move_lu(x6, y6, &i, &j);
    move_ld(i, j, &x9, &y9);
    Serial.print("0=");
    Serial.print(x0);
    Serial.print(", ");
    Serial.println(y0);
    Serial.print("6=");
    Serial.print(x6);
    Serial.print(", ");
    Serial.println(y6);
    Serial.print("9=");
    Serial.print(x9);
    Serial.print(", ");
    Serial.println(y9);
    Serial.print("3=");
    Serial.print(x3);
    Serial.print(", ");
    Serial.println(y3);

    if(dif(x0,x3)+dif(y0,y3)<lengtha>>1)
    {
      Serial.println(dif(x0,x3)+dif(y0,y3));
      return false;
    }
    if(dif(x3,x6)+dif(y3,y6)<lengtha>>1)
    {
      Serial.println(dif(x3,x6)+dif(y3,y6));
      return false;
    }
    if(dif(x6,x9)+dif(y6,y9)<lengtha>>1)
    {
      Serial.println(dif(x6,x9)+dif(y6,y9));
      return false;
    }
    if(dif(x9,x0)+dif(y9,y0)<lengtha>>1)
    {
      Serial.println(dif(x9,x0)+dif(y9,y0));
      return false;
    }
    ax=median(x0, x6);
    ay=median(y0, y6);
    bx=median(x9, x3);
    by=median(y9, y3);
    *cx=median(ax, bx);
    *cy=median(ay, by);
    return true;
  }
  else if(lengthb>>2 > lengtha) //right-upper corner of X
  {
    /*
    if((length_rd(median(x,median(x,bx)), median(y,median(y,by)), &i, &j) + length_lu(median(x,median(x,bx)), median(y,median(y,by)), &i, &j))>(lengthb>>2))
      return false;
    if((length_rd(median(median(x,bx),bx), median(median(y,by),by), &i, &j) + length_lu(median(median(x,bx),bx), median(median(y,by),by), &i, &j))>(lengthb>>2))
      return false;
    return true;
    */
    return false;
  }
  else if(lengtha>>2 > (ax-bx))
  {
    Serial.print("Might be a + at (");
    Serial.print(x);
    Serial.print(", ");
    Serial.print(y);
    Serial.println(")");
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
  if((!tv.get_pixel(i, j)) && !((i>0)&&tv.get_pixel(i-1, j)) && !((j>0) && tv.get_pixel(i, j-1)) && !((i<W-1) && tv.get_pixel(i+1, j)) && !((j<H-1) && tv.get_pixel(i, j+1)))  //check the nearest also
    return false;
  if(((i==x1)&&(j==y1))||((i==x2)&&(j==y2))) //stop condition
    return true;
  return IsLinkedLine(i, j, x2, y2) && IsLinkedLine(x1, y1, i, j);
}

/*
Return value bit mask:
001 - found a center at the right edge
010 - found a center at the left edge
011 - the transversal length is continuosly high
000 - the transversal length is continuosly low
100 - found center but not at the edge
111 - multiple centers found
*/
unsigned char FindXCenter_ru(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char length, unsigned char *x, unsigned char *y)
{
  unsigned char i, j, i1, j1, c=0, t1, t2;
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
  if(tv.get_pixel(i, j))
  {
    i1=i;
    j1=j;
  }
  else if((i>0)&&tv.get_pixel(i-1, j))  //check the nearest also
  {
    i1=i-1;
    j1=j;
  }
  else if((j>0) && tv.get_pixel(i, j-1))
  {
    i1=i;
    j1=j-1;
  }
  else if((i<W-1) && tv.get_pixel(i+1, j))
  {
    i1=i+1;
    j1=j;
  }
  else if((j<H-1) && tv.get_pixel(i, j+1))
  {
    i1=i;
    j1=j+1;
  }
  else
  {
    Serial.println("WTF!");
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
    return 7;
  }
  

  if(((i==x1)&&(j==y1))||((i==x2)&&(j==y2)))
  {
    if(length_ru(i1, j1, &t1, &t2) > length>>2)
    {
      *x=i1;
      *y=j1;
      Serial.print("Found center at  ");
      Serial.print(i1);
      Serial.print(", ");
      Serial.print(j1);
      Serial.print(". length=");
      Serial.println(length_ru(i1, j1, &t1, &t2));
      return 3;
    }
    else
      return 0;
  }


  t1=FindXCenter_ru(x1, y1, i, j, length, x, y);
  t2=FindXCenter_ru(i, j, x2, y2, length, x, y);

  if((t1==7) || (t2==7)) //already found multiple centers
    return 7;

  if(length_ru(i1, j1, &t1, &t2) > length>>2)
  {
    c=3;
    *x=i1;
    *y=j1;
    Serial.print("Found center at ");
    Serial.print(i1);
    Serial.print(", ");
    Serial.print(j1);
    Serial.print(". length=");
    Serial.println(length_ru(i1, j1, &t1, &t2));
  }
  
/* This might be optimized with different return values */
  if(t1==4)
  {
    if(c==3)
      return 7;
    if(t2==4)  //else if
      return 7;
    if(t2==0)  //else if
      return 4;
    return 7;  //else
  }
  if(t2==4) //else
  {
    if(c==3)
      return 7;
    if(t1==0)  //else
      return 4;
    return 7;  //...
  }
  if(t1==0)
  {
    if(t2==1)
      return 7;
    if((c==3)&&(t2==0))
      return 4;
    if(t2==3)
      return 1;
    return t2;
  }
  if(t2==0)
  {
    if(t1==2)
      return 7;
    if(t1==3)
      return 2;
    return t1;
  }
  if(t1==3)
  {
    if((c==3)&&(t2==3))
      return 3;
    if((c==3)&&(t2==2))
      return 2;
    return 7;
  }
  if(t2==3)
  {
    if((c==3)&&(t1==1))
      return 1;
    return 7;
  }
  if((t1==2)||(t2==1))
    return 7;  
  if(c==3)
    return 4;

  return 7;
}

