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

void cmd_sonara(void)
{
  Serial.print("Sonar A: The debug flag is ");
  Serial.print(debug);
  Serial.print(" now.\n");
}

void cmd_sonarb(void)
{
  Serial.print("Sonar B: The debug flag is ");
  Serial.print(debug);
  Serial.print(" now.\n");
}

void cmd_cross(void)
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


