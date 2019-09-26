# RealDash CAN Arduino examples

[www.realdash.net](https://www.realdash.net)

RealDash 1.5.9 or newer required

## About

The RealDash CAN protocol enables custom implementations and DIY projects to connect to RealDash.

## Channel Description File (XML)

The Channel Description File ties the attached device and RealDash together and
describes how values are interpreted by RealDash. In essence, the XML file is a
map of which input value is set by which data from attached device.

Channel Description file contains any number of <frame> blocks. Each <frame>
block defines a CAN ID and values that fill up the standard 8-byte CAN frame.

These Arduino examples contain an example XML file that is commented in detail.


## RealDash CAN protocol

RealDash CAN protocol was designed to easily connect to existing CAN network and transfer data over serial to RealDash.
However, this protocol can be used with custom and DIY devices.

RealDash reads a stream of data in following structures:

CAN DATA FRAME:</br>
4 bytes - 0x44,0x33,0x22,0x11</br>
4 bytes - CAN frame id number (as 32bit little endian value)</br>
8 bytes - CAN frame payload</br>

TEXT EXTENSION FRAME:</br>
4 bytes - 0x55,0x33,0x22,0x11</br>
4 bytes - CAN frame id number (as 32bit little endian value)</br>
N bytes - text in UTF8 format, including null-terminator</br>

First 4 bytes are an identifier at the beginning of each CAN frame. This is
required for RealDash to 'catch-up' on ongoing stream of CAN frames.

Each data frame payload can contain 'any' data in any format. How data is
layed in frame is described in xml file that is imported to RealDash
connection settings. See below for instructions on how to set up the Arduino
example. The example xml file contains thorough comments on each block.

The text in Text Extension Frame must be UTF8 compatible string terminated with 0.
A text extension frame contains one, and only one text string. It always
consumes the entire frame. Each different text frame must use own unique CAN id.


## SET_VALUE frame

RealDash sends a SET_VALUE frame over serial when value is updated based on
RealDash actions, like buttons or data triggers.

SET_VALUE frame is always 17 bytes with following layout:

4 bytes - always 0x44,0x33,0x22,0x11</br>
4 bytes - CAN frame id number (as 32bit little endian value)</br>
8 bytes - CAN frame payload</br>
1 byte - checksum byte</br>

First 4 bytes are RealDash CAN protocol identification tag.</br>
Next 4 bytes are CAN frame id number to set value into.</br>
Next 8 bytes are the can frame payload containing all frame values.</br>
Last byte is a simple checksum of preceeding 13 bytes. Checksum value overflow is ignored.</br>

Note that CAN frame id number and index value must match the xml file imported into RealDash connection settings.


## Arduino Examples

### RealDash_CAN example

A simple Arduino sketch that pushes 3 'hand-build' CAN frames from Arduino
into RealDash. CAN frames contain static data for RPM, MAP,
Coolant temperature, and TPS. Next two frames contain the statuses of
Arduino digital and analog pins.

### RealDash_CAN_2way example

An extension of first example reads SET_FRAME commands from RealDash and sets
Arduino digital and analog pins accordingly.

These examples uses 115200 baud rate as default value. Change the value to
match your serial setup.


### How to run Arduino Examples

1. Upload the example sketch into Arduino.
2. Open RealDash and go to 'Garage->Connections' and add new connection.
3. Select 'RealDash CAN'.
4. Next steps select which ever way you connect to Arduino, be it Bluetooth, Serial port, or WiFi. If using serial port, the example sketch uses 115200 baud.
5. For last page, the "Custom Channel Description File", select the 'realdash_can_example.xml'.
6. Tap upper left corner 'DONE' until back in dashboard.
7. Wait for couple of seconds for the Arduino to connect to RealDash.

This folder also includes simple test dashboard 'realdash_can_arduino_test.rd'. If Arduino is uploaded with 'RealDash_CAN_2way' example,
the buttons in dashboard change the digital and some analog pin values in Arduino.

