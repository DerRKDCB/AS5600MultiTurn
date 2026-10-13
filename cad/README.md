# Knob CAD and assembly

This directory contains the editable FreeCAD assembly and printable 3MF files for the AS5600 multi-turn knob. The model shows the rotating knob above a bearing-supported mounting body, with the AS5600 breakout positioned below it.

![Transparent CAD view showing the knob, mounting body, bearing and sensor board](../img/Screenshot_14-Oct_00-16-26_16558.png)

## Files

| File | Use |
|---|---|
| [`multiturn_potentiometer_as5600.FCStd`](multiturn_potentiometer_as5600.FCStd) | Editable FreeCAD model, including the knob, body and assembly/reference geometry. |
| [`3mf/multiturn_potentiometer_as5600-Body.3mf`](3mf/multiturn_potentiometer_as5600-Body.3mf) | Printable mounting body. |
| [`3mf/multiturn_potentiometer_as5600-knob.3mf`](3mf/multiturn_potentiometer_as5600-knob.3mf) | Printable rotating knob. |
| [`3mf/multiturn_potentiometer_as5600-4x.3mf`](3mf/multiturn_potentiometer_as5600-4x.3mf) | Slicer project laid out for four bodies and four knobs; review its settings for your printer before slicing. |

Use the individual 3MF files to print one knob and body, or open the 4x project to print multiple sets. Edit the `.FCStd` file in FreeCAD if you need to adapt the fit or mounting. FreeCAD's `.FCBak` files are local backups, not print sources.

## Other parts

| Part | Notes |
|---|---|
| AS5600 breakout board | Design reference: white AS5600 module. See the [AS5600 datasheet](https://www.mouser.com/pdfdocs/AMS_AS5600_Datasheet_EN.PDF), [Adafruit guide](https://learn.adafruit.com/adafruit-as5600-magnetic-angle-sensor/overview) and [example module listing](https://de.aliexpress.com/item/1005009738272341.html). Confirm your board's dimensions and pinout against the CAD. |
| Ball bearing | **6000-2RS**, 10 mm inner diameter × 26 mm outer diameter × 8 mm width. [Bearing illustration](https://www.nskbearingcatalogue.com/wp-content/uploads/2022/12/i.jpg) · [Example listing](https://www.amazon.com/10x26x8mm-Pre-Lubricated-Performance-Effective-6000-RS/dp/B0FD9RSSXF?th=1). |
| Magnet | Use a suitable **diametrically magnetized** magnet centered over the AS5600 and secured to the rotating knob; check alignment and air gap against your sensor module. |
| Mounting hardware | Fasteners for your board and installation; verify dimensions against your printed parts. |

## Assembly outline

1. Print the mounting body and knob from the 3MF files. Check the fit of the bearing and your AS5600 board before final assembly.
2. Seat the bearing in the body, then fit the knob so it can rotate freely without rubbing the stationary board.
3. Secure the magnet to the rotating part, centered above the sensor IC. Mount the AS5600 board below it and verify that the sensor reports a changing angle as the knob turns.
4. Connect the sensor to your microcontroller using the [wiring and library instructions](../README.md#electronics). Run the [firmware demo](../src/main.cpp) or an [example sketch](../examples/BasicExample/BasicExample.ino) to check the reading.

See the [gallery](../img/README.md) for part photos, CAD views, and a demonstration video.
