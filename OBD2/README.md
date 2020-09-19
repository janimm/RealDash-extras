# **RealDash-extras**
[realdash.net](https://www.realdash.net)

&nbsp;
## **OBD2**
This folder contains OBD2 XML description files used by default in RealDash. These can be used as starting points for custom modifications.

| file | description |
|:--------|:----------:|
| realdash_obd2.xml | RealDash default OBD2 description file.</br>This file is bundled with RealDash app and used as a default. |
| realdash_obd2_bosch_mp70.xml | Description file for 'Bosch mp7.0' ECU |
| realdash_obd2_january_5_1.xml | Description file for 'January 5.1' ECU |


&nbsp;
## **OBD2 XML description file**
The communication between OBD2 (via ELM327 compatible adapter) is described for RealDash in a XML text file and is fully customizable.

Note that while there are some similarities, the OBD2 XML file is **not** in same format as [RealDash CAN XML file](../RealDash-CAN/README.md).

The XML file starts and ends with **OBD2** tags and contains two main sections, **init** and **rotation**.

    <?xml version="1.0" encoding="utf-8"?>
    <OBD2>
        <!-- contains 'init' and 'rotation' sections-->
    </OBD2>


&nbsp;
## **The 'init' section**
The commands in init section are send after RealDash has connected to ELM327 adapter. In case you need to activate custom headers or other initialization, this is the correct section to add required commands. Here is the **init** section from RealDash default OBD2 XML:

    <init>
        <command send="atd"></command>
        <command send="atz"></command>
        <command send="atat1"></command>
        <command send="atst62"></command>
        <command send="atsp0"></command>
        <command send="ate0"></command>
        <command send="atl0"></command>
        <command send="ats0"></command>
        <command send="ath1"></command>
        <command send="atdpn"></command>
        <command send="0100"></command>
        <command send="0120"></command>
    </init>

The **init** section contains list of commands to send. Note that **atl0**, **ate0**, and **ats0** commands are required for RealDash to correctly parse the replys.

The commands **0100** and **0120** are the first ones that expect reply from the vehicle. Their reply contains information on vehicle supported PIDs. When RealDash sends these commands, you will see the *Checking PID support* message in RealDash.

There is good info on standard OBD2 PIDs on [Wikipedia](https://en.wikipedia.org/wiki/OBD-II_PIDs) page.


&nbsp;
## **The 'rotation' section**
After commands in init section are sent and responses received, RealDash starts sending the commands in rotation section. These commands are sent to OBD2 until connection is terminated. Here is one command in rotation section as an example:

    <command send="010c" skipCount="0" targetId="37" conversion="V/4"></command>

**command** tag parameters are:

&nbsp;
### **send**
This is the command to send to adapter. In above example **010c**, which is command to request engine RPM.

&nbsp;
### **skipCount**
Number of times to skip sending this command. This value can be used to request a certain values less frequently and therefore get more time critical data faster. For example, using 5 as **skipCount** would send the command and later skip sending the command 5 times before sending it again. For some values like ambient temperature **skipCount** can be relatively large value, like 60.

&nbsp;
### **targetId**
This value links the received value into RealDash input. The **targetId="37"** writes the received value to RealDash RPM input. Check out full listing of [RealDash targetIds](www.realdash.net/manuals/targetid.php).

&nbsp;
### **name [optional to targetId]**
Instead of mapping the value to existing RealDash input, **name** parameter can be used to create new input into RealDash *ECU Specific* category. Name parameter is used only if **targetId** is not present in command. Example:

    <command send="010c" skipCount="0" name="MYECU: Special RPM" conversion="V/4"></command>

Note that above example do not use the **targetId**, but **name** instead. When RealDash reads the XML file, a new custom input is created into *ECU Specific* category called **MYECU: Special RPM**. New custom input can be used like any other input in RealDash for gauges and triggers/actions.

Note: if you make your own dashboard that links into custom inputs, remember that other users need to have same XML available for the dashboard to work correctly. Another solution would be to make the dashboard use [RealDash build-in inputs](www.realdash.net/manuals/targetid.php) and use the *Input Mapping* feature in RealDash *Settings->Units & Values->Input Mapping*.

&nbsp;
### **conversion [optional]**
The **conversion** is a formula that is applied to received value. For example, **conversion="V/4"** takes incoming value and divides it by 4 before writing the value to its input. Conversion can be any valid mathematical formula with these variables:

* V (VALUE). This is the entire returned value as is (all received bytes).
* B0 - First byte of the reply (reading from left)
* B1 - Second byte of the reply
    * ...
    * B256
    * ...

&nbsp;
### **conversionABC [optional]**
The **conversionABC** is otherwise identical with **conversion**, but bytes are identified with **A,B,C,...,AA,AB,AC,...**, etc. This makes it easier to convert the Equation from other popular apps. For example **conversion="B0+15*(B1-43)"** and **conversionABC="A+15*(B-43)"** produce the same result.

&nbsp;
### **Conversion examples**

    conversion="V/10"
    - result is incoming value / 10

    conversion="B0+15*(B1-43)"
    - result is first byte + 15 * (second byte - 43)

&nbsp;
### **units [optional]**
Optional info to apply automatic unit conversions. Valid values are **units="C"**, **units="F"**, **units="km/h"**, **units="mph"**, **units="bar"**, **units="psi"**, **units="bit"**. If units is set to **bit** the value is considered to be an on/off (0 or 1) valued. RealDash reads the **bit** value from the lowest bit. Therefore there is a need for a bitshift to the right on conversion. For example **conversion="(V>>1)"** will read second bit on incoming value.

&nbsp;
### **header [optional]**
Header to set before sending the command. The set header remains until its set again, so consecutive commands with different header must specify the header. The full AT command to set the header must be included. If all commands use same header, its best to be placed in **init** section. Examples of **header** usage:

    <command send="010c" header="atsh7e0" skipCount="0" targetId="37" conversion="V/4"></command>
    <command send="2010" header="atsh7e5" skipCount="0" name="MYECU: custom value 1" conversion="V/4"></command>

&nbsp;
### **enum [optional]**
With **enum** parameter, the values in data can be directly interpreted as a text or another value. **enum** is a list of comma separated *value:display value* pairs. For example:

    <command send="221951" skipCount="3" name="OBD2: GM Shifter position" enum="72:P,24:R,80:N,48:D,120:S,40:S2,#:err"></command>

The above example uses **enum** to show shifter position as a character in RealDash. If value is 72, the character '**P**' is shown. If value is 24, the character '**R**' is shown, and so on. The hash tag **#** is used as default value, which in above example would show text **err**.

&nbsp;
### **Receive multiple values on one command**
It is possible to receive multiple values for one sent command. The individual values are listed in **command** as **values**. The parameters for the **value** are the same as with command with only one target value.

    <command send="2101" skipCount="0">
      <values>
        <value targetId="37" conversion="B11*40"></value>
        <value targetId="81" units="km/h" conversion="B17"></value>
        <value targetId="12" conversion="(B18*12.8/255)+5.2"></value>
        <value targetId="14" units="C" conversion="B8-40"></value>
        <value targetId="42" conversion="B10"></value>
        <value targetId="35" conversion="B22*2.04/255+B23*522.24/255"></value>
        <value targetId="0" conversion="14.7*(B9+128)/256"></value>
        <value targetId="30" conversion="B26*42.5/255+B27*10880/255"></value>
      </values>
    </command>

&nbsp;
## **How to import customized XML to RealDash**
1. Save your XML file to a place that is accessible from your device running RealDash.
2. Open RealDash Go to Garage, open vehicle door and tap the instrument cluster
3. On the *Connections* list, tap your OBD2 connection.
4. Tap *Select Vehicle* button and then select *Custom Channel Description File* and browse to your XML file.
5. Tap the upper left corner *Done* button and exit the Garage.
6. RealDash will reconnect and use your custom XML file for communications.

&nbsp;
## **OBD2 communication optimization settings**
RealDash has two settings that may increase the data rate on some vehicles. You can access these settings:

&nbsp;
### **Use Multipid Request**
This setting works if you have relatively new vehicle with CAN bus. When this option is enabled, RealDash constructs a *multipid* request command and if supported by vehicle can receive multiple values in one reply. This will considerably increase the data rate. On older vehicles this setting may slow down the time to connect to vehicle and therefore its disabled by default.

&nbsp;
### **Request Only First Reply**
On newer vehicles some requested information may be contained in several modules. For example engine RPM may be stored in ECU and TCU of the vehicle. Enabling this option will instruct the adapter to respond with first available value and not wait until all modules have responeded to the request. This may increase the data rate in some cases.

