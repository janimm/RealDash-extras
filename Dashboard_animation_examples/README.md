# Dashboard animation examples

How to create animated dashboards with RealDash.

- Name the animation XML file as 'dashboardname_anim.xml'
- Place the animation XML file into same folder with .rd file
- On Windows version of RealDash, use File menu 'Import' option to import the XML file (in edit mode).
	* This is required since Windows 10 does not allow accessing files without user interaction.
- When XML has changed, reload the .rd file, or press F2 on Windows to reload the dash and apply changes.
- Every time .rd file is saved it also contains all animations, there is no need to distribute the XML with .rd file.


## Files

| file | description |
|:--------|:----------:|
| RealDash_animation_example.xml</br>RealDash_animation_example_anim.rd | Example dashboard that demonstrates most common animation techniques. |
| Multiview.rd</br>Multiview_anim.xml | Multiview dashboard and associated animation XML file. Multiview is available for free in RealDash gallery. |


## Animations

### Animation types

| type | description |
|:--------|:----------:|
| morph | morph gauge or group area to another. Move, Resize etc. |
| fade | fade gauge or group |


### Animation easing types

Animations use easing. Multiple easing types are available for animations.

Each easing type may have postfix In, Out, or InOut. Default for all is InOut.

Default easing for all animations is SineInOut.

Examples of easing in action: [https://matthewlein.com/tools/ceaser]

| type | description |
|:--------|:----------:|
| Back | --- |
| Bounce | --- |
| Circ | --- |
| Cubic | --- |
| Elastic | --- |
| Expo | --- |
| Linear | --- |
| Quad | --- |
| Quart | --- |
| Quint | --- |
| Sine | --- |


[realdash.net](https://www.realdash.net)

