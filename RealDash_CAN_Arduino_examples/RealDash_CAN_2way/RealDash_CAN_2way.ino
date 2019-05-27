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
  const unsigned long serialBlockTag = 0x11223344;
  Serial.write((const byte*)&serialBlockTag, 4);

  // the CAN frame id number
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
    const unsigned long textExtensionBlockTag = 0x11223355;
    Serial.write((const byte*)&textExtensionBlockTag, 4);

    // the CAN frame id number
    Serial.write((const byte*)&canFrameId, 4);

    // text payload
    Serial.write(text, strlen(text) + 1);
  }
}


void ReadIncomingSerialData()
{
  while (Serial.available() > 16)
  {
    // we have at least 17 bytes available (one RealDash 'SET VALUE' frame)

    // read entire set value frame to byte buffer
    byte frame[17] = { 0 };
    Serial.readBytes(frame, 17);

    // first four bytes contain set value frame separator bytes
    unsigned long blockTag = 0;
    memcpy(&blockTag, frame, 4);

    // next four bytes contain set value CAN frame id
    unsigned long canFrameId = 0;
    memcpy(&canFrameId, frame + 4, 4);

    // next 8 bytes are the frame data
    byte data[8] = { 0 };
    memcpy(data, frame + 8, 8);

    // last byte is check byte calculated as sum of previous 13 bytes (ignore overflow)
    byte checkByte = 0;
    for (int i=0; i<16; i++)
    {
      checkByte += frame[i];
    }
    
    const unsigned long serialBlockTag = 0x11223344;
    if (blockTag == serialBlockTag && frame[16] == checkByte)
    {
      // this is a valid set value-frame:
      // it begins with RealDash CAN frame separator and last check byte calculation match
      HandleIncomingSetValueFrame(canFrameId, data);
    }
  }
}


void HandleIncomingSetValueFrame(unsigned long canFrameId, byte data[8])
{
  if (canFrameId == 3201)
  {
    memcpy(&digitalPins, data, 2);
    memcpy(&analogPins[0], data + 2, 2);
    memcpy(&analogPins[1], data + 4, 2);
    memcpy(&analogPins[2], data + 6, 2);
    
#if defined (READWRITE_PINS)
    // write digital pins
    for (int i=0; i<13; i++)
    {
      digitalWrite(i + 1, (digitalPins | (1 << i)) ? HIGH : LOW);
    }
    
    analogWrite(0, analogPins[0]);
    analogWrite(1, analogPins[1]);
    analogWrite(2, analogPins[2]);
#endif
  }
  else if (canFrameId == 3202)
  {
    memcpy(&analogPins[3], data + 0, 2);
    memcpy(&analogPins[4], data + 2, 2);
    memcpy(&analogPins[5], data + 4, 2);
    memcpy(&analogPins[6], data + 6, 2);
    
#if defined (READWRITE_PINS)
    analogWrite(3, analogPins[3]);
    analogWrite(4, analogPins[4]);
    analogWrite(5, analogPins[5]);
    analogWrite(6, analogPins[6]);
#endif
  }
}
