bool CaptureAndFind(unsigned char *cx, unsigned char *cy)
{
  unsigned char x, y;

  tv.capture();
  tv.fill(INVERT);

  //FilterNoise();
  
  for(y=0; y<H; y++)
  {
    for(x=0; x<W; x++)
      if(tv.get_pixel(x,y))
      {
        if(!MatchX(x,y,cx,cy))  // validate the object
          {ClearArea(x,y);  //clear the object if not X to avoid double-checking
          }
        else
          {
            tv.resume();
            return true;
          }
      }
      serialEvent();
      serialEvent2();
  }
  
  tv.resume();
  *cx=0;
  *cy=0;
  return false;
}

void FilterNoise(void)  //filters the noise in the frame
{                       //currently is too slow
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

bool MatchX(unsigned char x0, unsigned char y0, unsigned char *cx, unsigned char *cy)  //returns true if the area is a cross (X)
{                                              //the function assumes that we are close to the top of the object
  unsigned char lengtha, lengthb;
  unsigned char i, j, ax, ay, bx, by, x3, y3, x6, y6, x9, y9;

  move_ul(x0, y0, &x0, &y0);
  lengtha=length_dr(x0, y0, &ax, &ay);// + length_ul(x0, y0, &i, &j);
  move_ur(x0, y0, &x0, &y0);
  lengthb=length_dl(x0, y0, &bx, &by);// + length_ur(x0, y0, &i, &j);

  Serial.print("length (");
  Serial.print(lengtha);
  Serial.print(", ");
  Serial.print(lengthb);
  Serial.println(")");
  
  if(lengtha>>2 > lengthb) //left-upper corner of X
  {
    move_dr(x0, y0, &x6, &y6);
    move_rd(x0, y0, &i, &j);
    move_ru(i, j, &x3, &y3);
    move_lu(x6, y6, &i, &j);
    move_ld(i, j, &x9, &y9);
    Serial.print("R\n0=");
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

    if(dif(x0,x3)+dif(y0,y3)<lengtha>>2)
    {
      Serial.print("0-3=");
      Serial.println(dif(x0,x3)+dif(y0,y3));
      return false;
    }
    if(dif(x3,x6)+dif(y3,y6)<lengtha>>2)
    {
      Serial.print("3-6=");
      Serial.println(dif(x3,x6)+dif(y3,y6));
      return false;
    }
    if(dif(x6,x9)+dif(y6,y9)<lengtha>>2)
    {
      Serial.print("6-9=");
      Serial.println(dif(x6,x9)+dif(y6,y9));
      return false;
    }
    if(dif(x9,x0)+dif(y9,y0)<lengtha>>2)
    {
      Serial.print("9-0=");
      Serial.println(dif(x9,x0)+dif(y9,y0));
      return false;
    }

    lengtha=length_rd(x3, y3, &i, &j) + length_lu(x3, y3, &i, &j);
    lengthb=length_ld(x3, y3, &i, &j) + length_ru(x3, y3, &i, &j);
    if(!(lengtha>>2 > lengthb) && !(lengthb>>2 > lengtha))
    {
      Serial.println("Failed check at point 3");
      Serial.println(lengtha);
      Serial.println(lengthb);
      return false;
    }
    lengtha=length_dr(x9, y9, &i, &j) + length_ul(x9, y9, &i, &j);
    lengthb=length_dl(x9, y9, &i, &j) + length_ur(x9, y9, &i, &j);
    if(!(lengtha>>2 > lengthb) && !(lengthb>>2 > lengtha))
    {
      Serial.println("Failed check at point 9");
      Serial.println(lengtha);
      Serial.println(lengthb);
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
    move_dl(x0, y0, &x6, &y6);
    move_ld(x0, y0, &i, &j);
    move_lu(i, j, &x9, &y9);
    move_ru(x6, y6, &i, &j);
    move_rd(i, j, &x3, &y3);
    Serial.print("L\n0=");
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

    if(dif(x0,x3)+dif(y0,y3)<lengtha>>2)
    {
      Serial.print("0-3=");
      Serial.println(dif(x0,x3)+dif(y0,y3));
      return false;
    }
    if(dif(x3,x6)+dif(y3,y6)<lengtha>>2)
    {
      Serial.print("3-6=");
      Serial.println(dif(x3,x6)+dif(y3,y6));
      return false;
    }
    if(dif(x6,x9)+dif(y6,y9)<lengtha>>2)
    {
      Serial.print("6-9=");
      Serial.println(dif(x6,x9)+dif(y6,y9));
      return false;
    }
    if(dif(x9,x0)+dif(y9,y0)<lengtha>>2)
    {
      Serial.print("9-0=");
      Serial.println(dif(x9,x0)+dif(y9,y0));
      return false;
    }

    lengtha=length_rd(x3, y3, &i, &j) + length_lu(x3, y3, &i, &j);
    lengthb=length_ld(x3, y3, &i, &j) + length_ru(x3, y3, &i, &j);
    if(!(lengtha>>2 > lengthb) && !(lengthb>>2 > lengtha))
    {
      Serial.println("Failed check at point 3");
      Serial.println(lengtha);
      Serial.println(lengthb);
      return false;
    }
    lengtha=length_dr(x9, y9, &i, &j) + length_ul(x9, y9, &i, &j);
    lengthb=length_dl(x9, y9, &i, &j) + length_ur(x9, y9, &i, &j);
    if(!(lengtha>>2 > lengthb) && !(lengthb>>2 > lengtha))
    {
      Serial.println("Failed check at point 9");
      Serial.println(lengtha);
      Serial.println(lengthb);
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
  else if(lengtha>>2 > (ax-bx))
  {
    Serial.print("Might be a + at (");
    Serial.print(x0);
    Serial.print(", ");
    Serial.print(y0);
    Serial.println(")");
  }
  else
    return false;
  
  return false;
}

