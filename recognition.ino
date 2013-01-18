int FilterNoise(void)  //filters the noise in the frame
{
  return 0;   //not implemented yet
}

void ClearArea(unsigned char x, unsigned char y)  //Clears the linked area recursively
{                                                //WARNING: The current implemetation only clears lines > y and pixels >=x on the current line
  unsigned char i;
  for(i=x; (i<W)&&(tv.get_pixel(i, y)); i++)  //find the right bound
    continue;

  if(i==x)
    return; //pixel already 0

  tv.draw_row(y, x, --i, 0);

  if(++y==H)
    return;

  for(i--; i>=x; i--)
    if(!tv.get_pixel(i, y))
    {
      if(tv.get_pixel(i+1, y))
        ClearArea(i+1, y);
      else
        i--;
    }

  if(tv.get_pixel(x, y))
  {
    for(i=x-1; (i!=255)&&(tv.get_pixel(i, y)); i--)
      continue;
    ClearArea(i+1, y);
  }
  return;
}

bool MatchX(unsigned char x, unsigned char y)  //returns true if the area is a cross (X)
{
  return false;   //not implemented yet
}

