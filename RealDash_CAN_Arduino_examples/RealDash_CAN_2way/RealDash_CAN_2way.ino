/**
 * ============================================================================
 *  Name        : RealDash_CAN_2way.ino
 *  Part of     : RealDash
 *  Author      : Jani Immonen
 *  Created     : 15.10.2017
 *
 * Arduino example sketch of how to use RealDash CAN protocol.
 * 
 * This example code is free for any use.
 * 
 * www.realdash.net
 * ============================================================================
**/


// Arduino digital and analog pins
unsigned int digitalPins = 0;
int analogPins[7] = {0};

unsigned int rpm = 0;
unsigned int kpa = 992; // 99.2
unsigned int tps = 965; // 96.5
unsigned int clt = 80;  // 80 - 100
unsigned int textCounter = 0;

// incoming data
byte incomingFrame[17] = { 0 };
unsigned int incomingFramePos = 0;

// if READWRITE_PINS is defined, the values are read from and written to Arduino
// digital and analog pins.
//#define READWRITE_PINS 

void setup()
{
  // init serial
  Serial.begin(115200);
  delay(100);
}


void loop()
{
  ReadDigitalStatuses();
  ReadAnalogStatuses();
  SendCANFramesToSerial();
  ReadIncomingSerialData();

  // just some dummy values for simulated engine parameters
  if (rpm++ > 10000)
  {
    rpm = 500;
  }
  if (kpa++ > 2500)
  {
    kpa = 10;
  }
  if (tps++ > 1000)
  {
    tps = 0;
  }
  if (clt++ > 230)
  {
    // all values in frame are handled as unsigned values. To have negative values,
    // offset actual value and write corresponding conversion to xml file imported to RealDash
    clt = 0;
  }
  if (textCounter++ > 4000)
  {
    textCounter = 0;
  }

  delay(5);
}


void ReadDigitalStatuses()
{
#if defined (READWRITE_PINS)
  // read status of digital pins (1-13)
  digitalPins = 0;

  int bitposition = 0;
  for (int i=1; i<14; i++)
  {
    if (digitalRead(i) == HIGH) digitalPins |= (1 << bitposition);
    bitposition++;
  }
#endif
}


void ReadAnalogStatuses()
{
#if defined (READWRITE_PINS)
  // read analog pins (0-7)
  for (int i=0; i<7; i++)
  {
    analogPins[i] = analogRead(i);
  }
#endif
}


void SendCANFramesToSerial()
{
  byte buf[8];

  // build & send CAN frames to RealDash.
  // a CAN frame payload is always 8 bytes containing data in a manner
  // described by the RealDash custom channel description XML file
  // all multibyte values are handled as little endian by default.
  // endianess of the values can be specified in XML file if it is required to use big endian values

  // build 1st CAN frame, RPM, MAP, CLT, TPS (just example data)
  memcpy(buf, &rpm, 2);
  memcpy(buf + 2, &kpa, 2);
  memcpy(buf + 4, &clt, 2);
  memcpy(buf + 6, &tps, 2);

  // write first CAN frame to serial
  SendCANFrameToSerial(3200, buf);

  // build 2nd CAN frame, Arduino digital pins and 2 analog values
  memcpy(buf, &digitalPins, 2);
  memcpy(buf + 2, &analogPins[0], 2);
  memcpy(buf + 4, &analogPins[1], 2);
  memcpy(buf + 6, &analogPins[2], 2);

  // write 2nd CAN frame to serial
  SendCANFrameToSerial(3201, buf);

  // build 3rd CAN frame, rest of Arduino analog values
  memcpy(buf, &analogPins[3], 2);
  memcpy(buf + 2, &analogPins[4], 2);
  memcpy(buf + 4, &analogPins[5], 2);
  memcpy(buf + 6, &analogPins[6], 2);

  // write 3rd CAN frame to serial
  SendCANFrameToSerial(3202, buf);

  // build 4th frame, this is a text extension frame
  // only send once at 1000 loops
  if (textCounter == 0)
  {
    SendTextExtensionFrameToSerial(3203, "Hello RealDash, this is Arduino sending some text data");
  }
  else if (textCounter == 1000)
  {
    SendTextExtensionFrameToSerial(3203, "Tomorrow's forecast: Lots of sun and 30 degrees centigate");
  }
  else if (textCounter == 2000)
  {
    SendTextExtensionFrameToSerial(3203, "Now Playing: Insert your favorite song info here");
  }
  else if (textCounter == 3000)
  {
    SendTextExtensionFrameToSerial(3203, "Message from Arduino: All systems running at nominal efficiency");
  }
}


void SendCANFrameToSerial(unsigned long canFrameId, const byte* frameData)
{
  // the 4 byte identifier at the beginning of each CAN frame
  // this is required for RealDash to 'catch-up' on ongoing stream of CAN frames
  const byte serialBlockTag[4] = { 0x44, 0x33, 0x22, 0x11 };
  Serial.write(serialBlockTag, 4);

  // the CAN frame id number (as 32bit little endian value)
  Serial.write((const byte*)&canFrameId, 4);

  // CAN frame payload
  Serial.write(frameData, 8);
}


void SendTextExtensionFrameToSerial(unsigned long canFrameId, const char* text)
{
  if (text)
  {
    // the 4 byte identifier at the beginning of each CAN frame
    // this is required for RealDash to 'catch-up' on ongoing stream of CAN frames
    const byte textExtensionBlockTag[4] = { 0x55, 0x33, 0x22, 0x11 };
    Serial.write(textExtensionBlockTag, 4);

    // the CAN frame id number (as 32bit little endian value)
    Serial.write((const byte*)&canFrameId, 4);

    // text payload
    Serial.write(text, strlen(text) + 1);
  }
}


void ReadIncomingSerialData()
{
  while (Serial.available() > 0)
  {
    // little bit of extra effort here, since especially Bluetooth connections
    // may leave unsent/received data in internal buffer for a long time
    // therefore, we cannot be sure that incoming byte stream really starts at
    // where we expect it to start.

    // read one byte from serial stream
    incomingFrame[incomingFramePos++] = Serial.read();

    // check the first incoming bytes tag (0x44, 0x33, 0x22, 0x11)
    if (incomingFrame[0] != 0x44)
    {
      // first incoming byte is not 0x44, 
      // the tag at the beginning of the frame does not match, this is an invalid frame
      // just zero the incomingFrame buffer and start expecting first byte again
      memset(incomingFrame, 0, 17);
      incomingFramePos = 0;
    }

    if (incomingFramePos >= 17)
    {
      // frame complete, process it
      ProcessIncomingFrame(incomingFrame);
      
      // zero the incomingFrame buffer and start expecting first byte again
      memset(incomingFrame, 0, 17);
      incomingFramePos = 0;
    }
  }
}


void ProcessIncomingFrame(const byte* frame)
{
  // first four bytes contain set value frame separator bytes, always 0x44,0x33,0x22,x11
  // check that first 4 bytes match the tag
  if (frame[0] != 0x44 ||
      frame[1] != 0x33 ||
      frame[2] != 0x22 ||
      frame[3] != 0x11)
  {
    // frame tag does not match, wait for another frame
    return;
  }

  // next four bytes contain set value CAN frame id in little endian form
  unsigned long canFrameId = 0;
  memcpy(&canFrameId, frame + 4, 4);

  // next 8 bytes are the frame data
  // ...
  
  // last byte is check byte calculated as sum of previous 13 bytes (ignore overflow)
  byte checkByte = 0;
  for (int i=0; i<16; i++)
  {
    checkByte += frame[i];
  }

  if (frame[16] == checkByte)
  {
    // checksum match, this is a valid set value-frame:
    // the frame payload data is in frame + 8 bytes
    HandleIncomingSetValueFrame(canFrameId, frame + 8);
  }
}


void HandleIncomingSetValueFrame(unsigned long canFrameId, const byte* frameData)
{
  if (canFrameId == 3201)
  {
    memcpy(&digitalPins, frameData, 2);
    memcpy(&analogPins[0], frameData + 2, 2);
    memcpy(&analogPins[1], frameData + 4, 2);
    memcpy(&analogPins[2], frameData + 6, 2);
    
#if defined (READWRITE_PINS)
    // write digital pins
    for (int i=0; i<13; i++)
    {
       pinMode(i+1,OUTPUT);
      digitalWrite(i + 1, (digitalPins & (1 << i)) ? HIGH : LOW);
    }
    
    analogWrite(0, analogPins[0]);
    analogWrite(1, analogPins[1]);
    analogWrite(2, analogPins[2]);
#endif
  }
  else if (canFrameId == 3202)
  {
    memcpy(&analogPins[3], frameData + 0, 2);
    memcpy(&analogPins[4], frameData + 2, 2);
    memcpy(&analogPins[5], frameData + 4, 2);
    memcpy(&analogPins[6], frameData + 6, 2);
    
#if defined (READWRITE_PINS)
    analogWrite(3, analogPins[3]);
    analogWrite(4, analogPins[4]);
    analogWrite(5, analogPins[5]);
    analogWrite(6, analogPins[6]);
#endif
  }
}



