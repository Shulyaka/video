#define sonar_time 100

const byte sonarAddress[] = {0x70, 0x71}; // page 4 of datasheet

unsigned int rangea=0;
unsigned int rangeb=0;

void sonar_init(void)
{
  tv.set_vbi_hook(&sonar_int);        //https://nootropicdesign.com/forum/viewtopic.php?f=25&t=2394&p=2648&hilit=I2C#p2648
}

void sonar_int(void)
{
  static unsigned char sonar_num=0;
  static unsigned long prev_time;
  unsigned long cur_time=millis();

  if(sonar_num==0) //first time
  {
    sonar_num=1;
    prev_time=cur_time;
    interrupts();
    sendByteI2C(sonarAddress[0], 0x51);
    return;
  }

  if(cur_time-prev_time<sonar_time)
    return;
  
  prev_time=cur_time;

  interrupts();

  if(sonar_num==1)
  {
    rangea=readWordWaitI2C(sonarAddress[0],1);

    imu_z=((long int)rangea)<<15;
    Serial2.write(2+sizeof(fixed));
    Serial2.write("RU");
    Serial2.write((unsigned char *)&imu_z, sizeof(fixed));
    Serial2.write("\n\n\n");
    
    sendByteI2C(sonarAddress[1], 0x51);
    sonar_num=2;
  }
  else
  {
    rangeb=readWordWaitI2C(sonarAddress[1],1);

    wall=((long int)rangeb)<<15;
    Serial2.write(2+sizeof(fixed));
    Serial2.write("WU");
    Serial2.write((unsigned char *)&wall, sizeof(fixed));
    Serial2.write("\n\n\n");
    
    sendByteI2C(sonarAddress[0], 0x51);
    sonar_num=1;
  }
}

void sonar_address_change(void) //only call it once and with one sensor connected
{
  Wire.beginTransmission(sonarAddress[0]);
  Wire.write(0xAA);
  Wire.write(0xA5);
  Wire.write(sonarAddress[1]<<1);
  Wire.endTransmission();
}
