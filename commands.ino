int cmd_ping(void)
{
  Serial.println("Pong.");
  return 0;
}

int cmd_debug(int flag)
{
  debug=flag;
  Serial.print("The debug flag is ");
  Serial.print(debug);
  Serial.print(" now.\n");
  return 0;
}

int cmd_rangea(void)
{
  sonara_measure();
  Serial.print("Sonar A range is ");
  Serial.print(rangea);
  Serial.print(".\n");
  imu_z=((long int)rangea)<<15;
  print("imu_z", imu_z);

  Serial2.write("RU");
  Serial2.write((unsigned char *)&rangea, sizeof(fixed));
  Serial2.write("\n");
  
  return 0;
}

int cmd_rangeb(void)
{
  Serial.print("Sonar B range is ");
  Serial.print(rangeb);
  Serial.print(".\n");
  return 0;
}

int cmd_detect(void)
{
  unsigned char x, y;
  quaternion q=imu_q;
  quaternion cam=ident;
  fixed h_res=one>>4;
  fixed w_res=one>>4;
  
  fixed arr[3];
  
  if(!CaptureAndFind(&x, &y))
  {
    Serial.println("Object not detected");
    return 1;
  }
  
  Serial.print("Object detected at ");
  Serial.print(x);
  Serial.print(", ");
  Serial.print(y);
  Serial.println(".");
  tv.draw_rect(x-5, y-5, 10, 10, 1);
  
  arr[0]=fixed((x-H/2)*0x2000000LL)*h_res;   //0x20000000LL=one/(H/2)
  arr[1]=fixed((y-H/2)*0x2000000LL)*w_res;
  arr[2]=sqrt(one%one-arr[0]%arr[0]-arr[1]%arr[1]);
  
  q=q*cam*sqrt(quaternion(arr[2], -arr[1], arr[0], 0));
  
  print("q", q);
  
  arr[0]=-(q.w*q.y+q.x*q.z)<<1;         //q*quaternion(0,0,0,one)*conjugate(q);
  arr[1]=-(q.w*q.x+q.y*q.z)<<1;
  arr[2]=one-((q.x*q.x+q.y*q.y)<<1);
  
  print("arr[0]", arr[0]);
  print("arr[1]", arr[1]);
  print("arr[2]", arr[2]);
  
  if(arr[2]>=imu_z)
  {
    arr[2]=imu_z%one/arr[2];
    arr[0]=arr[0]*arr[2];
    arr[1]=arr[1]*arr[2];
  }
  else
  {
    arr[2]=arr[2]%one/imu_z;
    if(arr[0]<arr[2])
      arr[0]=arr[0]%one/arr[2];
    else
      arr[0]=one;
    if(arr[1]<arr[2])
      arr[1]=arr[1]%one/arr[2];
    else
      arr[1]=one;
  }

  print("imu_x", arr[0]);
  print("imu_y", arr[1]);
  
  Serial2.write("LU");
  Serial2.write((unsigned char *)&arr[0], sizeof(fixed));
  Serial2.write((unsigned char *)&arr[1], sizeof(fixed));
  Serial2.write("\n");
  
  return 0;
}

int cmd_QU(quaternion *q)
{
  imu_q=*q;

  return 0;
}
