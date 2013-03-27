#define sonaraAddress 0x70 // page 4 of datasheet
#define sonarbAddress 0x80 // custom address

unsigned int rangea=0;
unsigned int rangeb=0;

void sonar_init(void)
{
  //not implemented yet
}

void sonara_measure()
{
  sendByteI2C(sonaraAddress, 0x51);
  delay(100);
  rangea=readWordWaitI2C(sonaraAddress, 10);
}

void sonarb_measure()
{
  sendByteI2C(sonarbAddress, 0x51);
  delay(100);
  rangeb=readWordWaitI2C(sonarbAddress, 10);
}
