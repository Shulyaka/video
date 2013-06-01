#define sonaraAddress 0x70 // page 4 of datasheet
#define sonarbAddress 0x71 // custom address

unsigned int rangea=0;
unsigned int rangeb=0;

void sonar_init(void)
{
  tv.set_vbi_hook(&sonar_callback);
}

void sonar_callback(void)
{
  static unsigned char sonar_num=-1;
  static unsigned long prev_time;
  unsigned long cur_time=millis();

  if(sonar_num==-1) //first time
  {
    sonar_num=0;
    prev_time=cur_time;
    sendByteI2C(sonaraAddress, 0x51);
    return;
  }

  if(cur_time-prev_time<100)
    return;
  
  prev_time=cur_time;
  
  if(sonar_num==0)
  {
    rangea=readWordI2C(sonaraAddress);

    imu_z=((long int)rangea)<<15;
    Serial2.write("RU");
    Serial2.write((unsigned char *)&imu_z, sizeof(fixed));
    Serial2.write("\n");
    
    sendByteI2C(sonarbAddress, 0x51);
  }
  else
  {
    rangeb=readWordI2C(sonarbAddress);

    wall=((long int)rangeb)<<15;
    Serial2.write("WU");
    Serial2.write((unsigned char *)&wall, sizeof(fixed));
    Serial2.write("\n");
    
    sendByteI2C(sonaraAddress, 0x51);
  }
  
  sonar_num^=1;
}
/*
void sonara_measure(void)
{
  sendByteI2C(sonaraAddress, 0x51);
  delay(100);
  rangea=readWordWaitI2C(sonaraAddress, 10);
}

void sonarb_measure(void)
{
  sendByteI2C(sonarbAddress, 0x51);
  delay(100);
  rangeb=readWordWaitI2C(sonarbAddress, 10);
}
*/
void sonar_address_change(void)
{
  Wire.beginTransmission(sonaraAddress);
  Wire.write(0xAA);
  Wire.write(0xA5);
  Wire.write(sonarbAddress<<1);
  Wire.endTransmission();
  
}
