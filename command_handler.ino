#define CMDUNKNOWN 0
#define CMDPING    1 // echo test
#define CMDRANGEA  2
#define CMDRANGEB  3
#define CMDDEBUG   9 // toggle debug flag
#define CMDUSR1    13
#define CMDUSR2    14
#define CMDUSR3    15

#define cmdBufLen 127
char cmdBuf[cmdBufLen]={0};
unsigned char cmdPos=0;
char cmdBuf2[cmdBufLen]={0};
unsigned char cmdPos2=0;

void serialEvent(void)
{
  unsigned char cmd;
  int param=0;
  while(Serial.available())
  {
    cmdBuf[cmdPos++]=(char)Serial.read();
    if ((cmdBuf[cmdPos-1]=='\n') || (cmdBuf[cmdPos-1]=='\r'))
      break;
    if (cmdPos==cmdBufLen)
    {
      error("The command is too long");
      Serial.flush();
      clear_cmdBuf();
      break;
    }
  }
  if (cmdPos==0 || ((cmdBuf[cmdPos-1]!='\n') && (cmdBuf[cmdPos-1]!='\r')))
    return;
  if(debug) print_cmdBuf();
  cmd=parse_cmd(&param);
  clear_cmdBuf();
  
  if(debug)
  {
    Serial.print("Command: ");
    Serial.print((int)cmd);
    Serial.print(", Params: ");
    Serial.print(param);
    Serial.print(".\n");
  }
  
  switch(cmd)
  {
    case CMDPING:
      cmd_ping();
      break;
    case CMDDEBUG:
      cmd_debug(param);
      break;
    case CMDRANGEA:
      cmd_rangea();
      break;
    case CMDRANGEB:
      cmd_rangeb();
      break;

    case CMDUNKNOWN:
      error("Unknown command");
      break;
    default:
      error("The command has been disabled");
  }
  if(Serial.available())
    serialEvent();
}

void serialEvent2(void)
{
  static char cmdLen=0;
  char rc;
  
  while(Serial2.available())
  {
    rc=(char)Serial2.read();
    if(cmdLen<0)
    {
      if(rc=='\r')
        continue;
      if(rc=='\n')
        cmdLen--;
      else
        cmdLen=-1;
      if(cmdLen==-3)
        cmdLen=0;
//      Serial.print("new cmdLen=");
//      Serial.println(cmdLen, DEC);
      continue;
    }
    cmdBuf2[cmdPos2++]=rc;
    if(cmdPos2==1)
      cmdLen=cmdBuf2[0];
    Serial.print("cmdLen=");
    Serial.println(cmdLen, DEC);
    if(cmdLen<3 || cmdLen>20)
    {
      cmdLen=-1;
      cmdPos2=0;
      error("invalid command len");
      continue;
    }
    if (cmdPos2==cmdLen)
      break;
  }
  if (cmdPos2!=cmdLen)
    return;
  if(debug) print_cmdBuf2();
  parse_cmd2();
  clear_cmdBuf2();

  if(Serial2.available())
    serialEvent2();
}

unsigned char parse_cmd(int *param)
{
  if (!memcmp(cmdBuf,"ping",4))
    return CMDPING;
  if (!memcmp(cmdBuf,"debug",5))
  {
    if (!memcmp(cmdBuf+6,"on",2))
      *param=true;
    else if (!memcmp(cmdBuf+6,"off",3))
      *param=false;
    else
      return CMDUNKNOWN;
    return CMDDEBUG;
  }
  if (!memcmp(cmdBuf,"ra",2))
    return CMDRANGEA;
  if (!memcmp(cmdBuf,"rb",2))
    return CMDRANGEB;

  return CMDUNKNOWN;
}

int parse_cmd2(void)
{
  if (!memcmp(cmdBuf2+1,"ping",4))
    return cmd_ping();
  if (!memcmp(cmdBuf2+1,"debug",5))
  {
    if (!memcmp(cmdBuf2+7,"on",2))
      return cmd_debug(true);
    else if (!memcmp(cmdBuf2+7,"off",3))
      return cmd_debug(false);
    
    error("debug: wrong parameters");
    return -1;
  }
  if (!memcmp(cmdBuf2+1,"ra",2))
    return cmd_rangea();
  if (!memcmp(cmdBuf2+1,"rb",2))
    return cmd_rangeb();
  if (!memcmp(cmdBuf2+1,"QU",2)) //Quaternion Update command
    return cmd_QU((quaternion*)(cmdBuf2+3));

  error("Unknown command");
  return -1;
}

void print_cmdBuf(void)
{
  Serial.print("Incomming command (");
  Serial.print(strlen(cmdBuf));
  Serial.print("):\n"); // a smiley
  Serial.println(cmdBuf);
}

void clear_cmdBuf(void)
{
  memset(cmdBuf,'\0',cmdBufLen);
  cmdPos=0;
}

void print_cmdBuf2(void)
{
  Serial.print("Incomming command (");
  Serial.print(strlen(cmdBuf2));
  Serial.print("):\n");
  Serial.println(cmdBuf2);
}

void clear_cmdBuf2(void)
{
  memset(cmdBuf2,'\0',cmdBufLen);
  cmdPos2=0;
}

