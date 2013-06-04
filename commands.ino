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
  Serial.print("Sonar A range is ");
  Serial.print(rangea);
  Serial.print(".\n");

  return 0;
}

int cmd_rangeb(void)
{
  Serial.print("Sonar B range is ");
  Serial.print(rangeb);
  Serial.print(".\n");

  return 0;
}

int cmd_QU(quaternion *q)
{
  imu_q=*q;
  //print("q", imu_q);
  return 0;
}
