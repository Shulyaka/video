void cmd_ping(void)
{
  Serial.println("Pong.");
}

void cmd_debug(int flag)
{
  debug=flag;
  Serial.print("The debug flag is ");
  Serial.print(debug);
  Serial.print(" now.\n");
}

void cmd_rangea(void)
{
  sonara_measure();
  Serial.print("Sonar A range is ");
  Serial.print(rangea);
  Serial.print(".\n");
}

void cmd_rangeb(void)
{
  Serial.print("Sonar B range is ");
  Serial.print(rangeb);
  Serial.print(".\n");
}

void cmd_detect(void)
{
  unsigned char x, y;
  
  if(CaptureAndFind(&x, &y))
  {
    Serial.print("Object detectad at ");
    Serial.print(x);
    Serial.print(", ");
    Serial.print(y);
    Serial.println(".");
    tv.draw_rect(x-5, y-5, 10, 10, 1);
  }
  else
    Serial.println("Object not detected");
}


