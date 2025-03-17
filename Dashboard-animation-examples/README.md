# **Dashboard animation examples**

How to create animated dashboards with RealDash.

- Name the animation XML file as *dashboardname_anim.xml*
- Place the animation XML file into same folder with *.rd* file
- On Windows version of RealDash, use *File* menu *Import* option to import the XML file (in edit mode).
	* This is required since Windows Store apps are not allowed to access files without user interaction.
- When XML has changed, reload the .rd file, or press *F2* on Windows to reload the dash and apply changes.
- Every time .rd file is saved it also contains all animations, there is no need to distribute the XML with .rd file.

**NOTE** 
The *groups* and *group* attributes will not be supported in future versions of RealDash. To animate multiple gauges with one animation, use a Container Gauge in RealDash. Container gauge accepts all animation types and applies the animation to all contained gauges. Removal of the support for *groups* in XML does not affect the functionality of previously published dashboards.


&nbsp;
## **Files**
| file | description |
|:--------|:----------:|
| RealDash_animation_example.rd</br>RealDash_animation_example_anim.xml | Example dashboard that demonstrates most common animation techniques. |
| value_anim_example.rd</br>value_anim_example_anim.xml | Example of how to animate gauges based on input values. Requires RealDash version 2.4.7 or newer. |
| Multiview.rd</br>Multiview_anim.xml | Multiview dashboard and associated animation XML file. Multiview is available for free in RealDash gallery. |


&nbsp;
## **Animations**

&nbsp;
### **Animation types**
| type | description |
|:--------|:----------:|
| morph | animate the area of the gauge |
| fade | fade gauge |
| value-morph | animate the area of the gauge based on input value. Requires RealDash version 2.4.7 or newer. |
| value-fade | fade gauge based on input value. Requires RealDash version 2.4.7 or newer. |


&nbsp;
### **Animation easing types**
Animations use easing. Multiple easing types are available for animations.

Each easing type may have postfix In, Out, or InOut. Default for all is InOut.

Default easing for all animations is SineInOut.

Examples of easing in action: [https://matthewlein.com/tools/ceaser]

| type |
|:--------|
| Back(In, Out, InOut) |
| Bounce(In, Out, InOut) |
| Circ(In, Out, InOut) |
| Cubic(In, Out, InOut) |
| Elastic(In, Out, InOut) |
| Expo(In, Out, InOut) |
| Linear(In, Out, InOut) |
| Quad(In, Out, InOut) |
| Quart(In, Out, InOut) |
| Quint(In, Out, InOut) |
| Sine(In, Out, InOut) |


[realdash.net](https://www.realdash.net)

