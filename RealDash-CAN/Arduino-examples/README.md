# **RealDash CAN Arduino examples**
[www.realdash.net](https://www.realdash.net)

First, see information about [RealDash CAN protocol and XML description file](../README.md)

&nbsp;
### **RealDash_CAN example**
A simple Arduino sketch that pushes 3 'hand-build' CAN frames from Arduino
into RealDash. CAN frames contain static data for RPM, MAP,
Coolant temperature, and TPS. Next two frames contain the statuses of
Arduino digital and analog pins.

&nbsp;
### **RealDash_CAN_2way example**
An extension of first example reads **SET_FRAME** commands from RealDash and sets
Arduino digital and analog pins accordingly.

These examples uses 115200 baud rate as default value. Change the value to
match your serial setup.

&nbsp;
### **How to run Arduino Examples**
1. Upload the example sketch into Arduino.
2. Open RealDash and go to 'Garage->Connections' and add new connection.
3. Select 'RealDash CAN'.
4. Next steps select which ever way you connect to Arduino, be it Bluetooth, Serial port, or WiFi. If using serial port, the example sketch uses 115200 baud.
5. For last page, the "Custom Channel Description File", select the 'realdash_can_example.xml'.
6. Tap upper left corner 'DONE' until back in dashboard.
7. Wait for couple of seconds for the Arduino to connect to RealDash.

This folder also includes simple test dashboard 'realdash_can_arduino_test.rd'. If Arduino is uploaded with 'RealDash_CAN_2way' example,
the buttons in dashboard change the digital and some analog pin values in Arduino.

