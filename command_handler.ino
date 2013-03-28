#define CMDUNKNOWN 0
#define CMDPING    1 // echo test
#define CMDRANGEA  2
#define CMDRANGEB  3
#define CMDDETECT  4
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
    case CMDDETECT:
      cmd_detect();
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
  unsigned char cmd;
  int param=0;
  while(Serial2.available())
  {
    cmdBuf2[cmdPos2++]=(char)Serial2.read();
    if ((cmdBuf2[cmdPos2-1]=='\n') || (cmdBuf2[cmdPos2-1]=='\r'))
      break;
    if (cmdPos2==cmdBufLen)
    {
      error("The command is too long");
      Serial2.flush();
      clear_cmdBuf2();
      break;
    }
  }
  if (cmdPos2==0 || ((cmdBuf2[cmdPos2-1]!='\n') && (cmdBuf2[cmdPos2-1]!='\r')))
    return;
  if(debug) print_cmdBuf2();
  cmd=parse_cmd(&param);
  clear_cmdBuf2();
  
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
    case CMDDETECT:
      cmd_detect();
      break;

    case CMDUNKNOWN:
      error("Unknown command");
      break;
    default:
      error("The command has been disabled");
  }
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
  if (!memcmp(cmdBuf,"detect",5))
    return CMDDETECT;
  if (!memcmp(cmdBuf,"ra",2))
    return CMDRANGEA;
  if (!memcmp(cmdBuf,"rb",2))
    return CMDRANGEB;

  return CMDUNKNOWN;
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
  Serial.print("):\n"); // a smiley
  Serial.println(cmdBuf2);
}

void clear_cmdBuf2(void)
{
  memset(cmdBuf2,'\0',cmdBufLen);
  cmdPos2=0;
}

