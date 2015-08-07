/*
  AeroQuad v2.5 Beta 1 - July 2011
  www.AeroQuad.com
  Copyright (c) 2011 Ted Carancho.  All rights reserved.
  An Open Source Arduino based multicopter.
 
  This program is free software: you can redistribute it and/or modify 
  it under the terms of the GNU General Public License as published by 
  the Free Software Foundation, either version 3 of the License, or 
  (at your option) any later version. 

  This program is distributed in the hope that it will be useful, 
  but WITHOUT ANY WARRANTY; without even the implied warranty of 
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
  GNU General Public License for more details. 

  You should have received a copy of the GNU General Public License 
  along with this program. If not, see <http://www.gnu.org/licenses/>. 
*/

/* Modified by Denis Shulyaka */

// I2C functions

void sendByteI2C(const int deviceAddress, const byte dataValue) {
  Wire.beginTransmission(deviceAddress);
  Wire.write(dataValue);
  Wire.endTransmission();
}

byte readByteI2C(const int deviceAddress) {
    Wire.requestFrom(deviceAddress, 1);
    return Wire.read();
}

int readWordI2C(const int deviceAddress) {
  Wire.requestFrom(deviceAddress, 2);
  return (Wire.read() << 8) | Wire.read();
}

int readWordI2C() {
  return (Wire.read() << 8) | Wire.read();
}

int readWordWaitI2C(int deviceAddress) {
  byte msb, lsb;
  Wire.requestFrom(deviceAddress, 2); // request two bytes
  while(!Wire.available()); // wait until data available
  msb = Wire.read();
  while(!Wire.available()); // wait until data available
  lsb = Wire.read();
  return (((int)msb<<8) | ((int)lsb));
}

int readWordWaitI2C(int deviceAddress, unsigned char retryCount=0) {
  Wire.requestFrom(deviceAddress, 2); // request two bytes
  while(Wire.available()<2) // wait until data available
  {
    if(!retryCount--)       // or retryCount is zero
      return -1;
    delay(10);
  }
  return (Wire.read()<<8) | Wire.read();
}

int readReverseWordI2C(const int deviceAddress) {
  Wire.requestFrom(deviceAddress, 2);
  return Wire.read()|(Wire.read() << 8);
}

int readReverseWordI2C() {
  return Wire.read()|(Wire.read() << 8);
}

byte readWhoI2C(const int deviceAddress) {
  // read the ID of the I2C device
  sendByteI2C(deviceAddress, 0x00);
  delay(100);
  return readByteI2C(deviceAddress);
}

byte readRegisterI2C(const int deviceAddress, const byte dataAddress) {
  sendByteI2C(deviceAddress, dataAddress);
  return readByteI2C(deviceAddress);
}

byte readRegisterI2C(const int deviceAddress, const byte dataAddress, const byte dataMask) {
  return readRegisterI2C(deviceAddress, dataAddress) & dataMask;
}

void updateRegisterI2C(const int deviceAddress, const byte dataAddress, const byte dataValue) {
  Wire.beginTransmission(deviceAddress);
  Wire.write(dataAddress);
  Wire.write(dataValue);
  Wire.endTransmission();
}  

void updateRegisterI2C(const int deviceAddress, const byte dataAddress, const byte dataValue, const byte dataMask) {
  updateRegisterI2C(deviceAddress, dataAddress, readRegisterI2C(deviceAddress, dataAddress, ~dataMask) | (dataValue & dataMask) );
}

