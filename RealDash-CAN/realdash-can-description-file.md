&nbsp;
# **Channel Description File (XML)**
The RealDash CAN Channel Description File describes how data in CAN frames are interpreted by RealDash.

Channel description file starts with tag **RealDashCAN** including the file format version **version="2"**. Next in file is a list of **frames**.

    <?xml version="1.0" encoding="utf-8"?>
    <RealDashCAN version="2">
        <frames>
        <!-- list of frames here -->
        </frames>
    </RealDashCAN>

&nbsp;
## **'baseId' parameter [optional]**
**frames** may optionally contain a parameter **baseId** that is used for the enclosed **frame** tags. Example:

    <frames baseId="3200">

Specifies the **baseId** of 3200. Any id value in later **frame** is added to the base id. For example, if **frames baseId** is 3200 and it contains a frame with **id** 1, then the frame is considered to have an **id** of 3201.

The **baseId** and the **id** in **frame** can also be specified as hexadecimal value with prefix of **0x**. Example:

    <frames baseId="0xc80"> <!-- same as baseId="3200"-->

&nbsp;
## **frame**
**frames** contain a list of **frame** tags. Each **frame** specifies an *id* number for the frame and other optional parameters. Each **frame** also contains a list of **value** tags that describe how data in frame is interpreted and linked to RealDash inputs. An example **frame**:

    <frame id="3200">
      <value targetId="37" offset="0" length="2"></value>
      <value targetId="31" offset="2" length="2" conversion="V/10"></value>
      <value targetId="14" units="C" offset="4" length="2" conversion="V-100"></value>
      <value targetId="42" offset="6" length="2" conversion="V/10"></value>
    </frame>

&nbsp;
## **frame 'id' parameter**
Every **frame** must have an **id** number, specified either in decimal or hexadecimal number (with **0x** prefix). Note that if enclosing **frames** have a **baseId** specified, the resulting id number will be **baseId + id**.

    <frame id="3200">
    or
    <frame id="0xc80">

&nbsp;
### **Composite ID support (optional)**
RealDash XML format supports composite IDs where CAN frame contain varying data and a **running** id value to identify the frame. The composite ID is specified as:
* CAN_ID: the frame CAN ID.
* COMPOSITE_ID: Composite ID that is read from frame data.
* COMPOSITE_ID_OFFSET: Offset (in bytes) where Composite ID is in frame data.
* COMPOSITE_ID_LENGTH: Length (in bytes) of the Composite ID in frame data.
---

    <frame id="0x3E8:5533,0,2">

The above example specifies a frame with CAN ID of 3E8 hexadecimal, composite id of 5533 decimal. Composite id is read from frame data from first (0) byte and its length is 2 bytes.


&nbsp;
## **frame 'endianess' parameter (optional)**
Each **frame** can optionally specify the *endianess* of how multibyte values are interpreted in frame data. Accepted values for **endianess** are **endianess="big"** or **endianess="little"** (the default setting). The endianess parameter can also be specified individually for each **value**. An example of how to specify all values in frame to be considered *big endian*:

    <frame id="3200" endianess="big">

&nbsp;
## **frame 'signed' parameter (optional)**
Each **frame** can optionally specify the frame values as *signed* to force RealDash to handle the data as signed values. The signed parameter can also be specified individually for each **value**. By default bytes are handled as *unsigned*. An example of how to specify all values in frame to be *signed*:

    <frame id="3200" signed="true">

&nbsp;
## **frame 'writeInterval' parameter (optional)**
**writeInterval** parameter is used to specify a case when RealDash is requested to write this frame back to the device on certain interval. The value in **writeInterval** is milliseconds. For example, to specify that RealDash should write frame 3200 to CAN once a second, use:

    <frame id="3200" writeInterval="1000">

&nbsp;
## **frame 'size' parameter (optional)**
Typically CAN frames contain 8 byte payload. When connecting directly to a CAN device with supported [CAN Adapters](http://realdash.net/manuals/supported_can_lin_analyzers.php), a CAN frame may contain less than standard 8 bytes. Also, when using *RealDash CAN* protocol CAN frame may contain up to 64 bytes of payload data. While it is not required to set the **size** parameter when using *RealDash CAN* protocol and frame is larger than 8 bytes, it is a good practise to ensure compatibility with possible CAN FD compatible adapters.

The **size** parameter can be used to specify a custom number (1-64) of bytes contained by CAN frame:

    <frame id="3200" size="6">

&nbsp;
## **values in a frame**
A **frame** contains a list of **value** tags that specify individual values in one frame and how they are interpreted and linked to RealDash inputs. An example of one **value**:

      <value targetId="37" offset="0" length="2"></value>

&nbsp;
## **value 'targetId' parameter**
This value links the received value into RealDash input. The **targetId="37"** writes the received value to RealDash RPM input. Check out full listing of [RealDash targetIds](www.realdash.net/manuals/targetid.php).

&nbsp;
## **value 'name' (optional to targetId)**
Instead of mapping the value to existing RealDash input, **name** parameter can be used to create new input into RealDash *ECU Specific* category. Name parameter is used only if **targetId** is not present in command. Example:

     <value name="MYECU: Special RPM" offset="0" length="2"></value>

Note that above example do not use the **targetId**, but **name** instead. When RealDash reads the XML file, a new custom input is created into *ECU Specific* category called **MYECU: Special RPM**. New custom input can be used like any other input in RealDash for gauges and triggers/actions.

Note: if you make your own dashboard that links into custom inputs, remember that other users need to have same XML available for the dashboard to work correctly. Another solution would be to make the dashboard use [RealDash build-in inputs](www.realdash.net/manuals/targetid.php) and use the *Input Mapping* feature in RealDash *Settings->Units & Values->Input Mapping*.

&nbsp;
## **value 'offset' parameter**
**offset** parameter specifies where in frame data the value data begins. Value is specified in bytes. To specify that value data begins from 3rd byte in frame, use **offset="2"**:

    <value targetId="31" offset="2" length="2"></value>

&nbsp;
## **value 'length' parameter**
**length** parameter specifies how many bytes is used for value data. Combined with **offset** parameter, the corresponding bytes are read as a value. In previous example we read the data beginning 3rd byte of the frame (**offset="2"**) and read 2 bytes (**length="2"**) to form a value:

    <value targetId="31" offset="2" length="2"></value>

&nbsp;
## **value 'startbit' and 'bitcount' parameters (optional)**
In addition to **offset** and **length** parameters, the **startbit** and **bitcount** can be used to specify from where the value is read in frame data. The **startbit** is the index of the first bit of the value, and **bitcount** is a number of bits used by the value. These can be combined with **offset** parameter, but **bitcount** will override the **length** if present.

&nbsp;
## **value 'endianess' parameter (optional)**
Value can optionally specify the *endianess* of how multibyte values are interpreted in value data. Accepted values for **endianess** are **endianess="big"** or **endianess="little"** (the default setting). The endianess parameter can also be specified in enclosing **frame** to apply it to all values. An example of how to specify the value to be considered *big endian*:

    <value targetId="37" offset="0" length="2" endianess="big"></value>

&nbsp;
## **value 'signed' parameter (optional)**
Each **value** can optionally specify the *signed* of the frame values to force RealDash to handle the bytes as signed values. The signed parameter can also be specified in enclosing **frame** to apply it to all values. By default bytes are handled as *unsigned*. An example of how to specify a value to be handled as *signed*:

    <value targetId="14" offset="4" length="2" signed="true"></value>

&nbsp;
## **value 'rangeMin' and 'rangeMax' parameters (optional)**
if **name** is used instead of **targetId**, these parameters specify a recommended value range in RealDash.

&nbsp;
## **value 'initialValue' parameter (optional)**
If this parameter is present, value is written to CAN after connection has been made to the CAN bus.

&nbsp;
## **value 'conversion' (optional)**
The **conversion** parameter is a formula that is applied to received value. For example, **conversion="V/4"** takes incoming value and divides it by 4 before writing the value to its input. Conversion can be any valid mathematical formula with these variables:

* V (VALUE). This is the value itself, specified by parameters in **value** tag.
* B0 - First byte of the value (reading from left)
* B1 - Second byte of the value
    * ...
    * B256
    * ...

&nbsp;
## **value 'conversionABC' (optional)**
The **conversionABC** is otherwise identical with **conversion**, but bytes are identified with **A,B,C,...,AA,AB,AC,...**, etc. This makes it easier to convert the Equation from other popular apps. For example **conversion="B0+15*(B1-43)"** and **conversionABC="A+15*(B-43)"** produce the same result.

&nbsp;
## **Conversion examples**

    conversion="V/10"
    - result is incoming value / 10

    conversion="B0+15*(B1-43)"
    - result is first byte + 15 * (second byte - 43)

&nbsp;
## **value 'units' parameter (optional)**
Optional info to apply automatic unit conversions. Valid values are **units="C"**, **units="F"**, **units="km/h"**, **units="mph"**, **units="bar"**, **units="psi"**, **units="bit"**. If units is set to **bit** the value is considered to be an on/off (0 or 1) valued. RealDash reads the **bit** value from the lowest bit. Therefore there is a need for a bitshift to the right on conversion. For example **conversion="(V>>1)"** will read second bit on incoming value.

&nbsp;
## **value 'enum' parameter (optional)**
With **enum** parameter, the values in data can be directly interpreted as a text or another value. **enum** is a list of comma separated *value:display value* pairs. For example:

    <value name="GMLS: Shifter position" offset="7" length="1" enum="72:P,24:R,80:N,48:D,120:S,40:S2,#:err"></value>

The above example uses **enum** to show shifter position as a character in RealDash. If value is 72, the character '**P**' is shown. If value is 24, the character '**R**' is shown, and so on. The hash tag **#** is used as default value, which in above example would show text **err**.

In addition, **enum** supports a range of values with *tilde* **~** operator. For example:

    <value name="Custom Shifter position" offset="7" length="1" enum="0~10:P,11~20:R,21~100:N,150~200:D,#:err"></value>


