# race-plotter
Open-source hardware-based boat racing GPS plotter.

![status](https://img.shields.io/badge/STATUS-INCOMPLETE-red)

<figure>

![demo](./img/display-demo.gif)

<figcaption align = "left"><b>Fig. 1 - Mock-up of compass and shift indicator as if lifting on starboard tack</b></figcaption>

</figure>


## Background

Yacht racers benefit from knowing their speed and heading, and how it is changing with time. With increasing access to global navigation systems, electronic navigation aids have become available and are class legal to a greater or lesser degree.

The UK/RYA [class rules](https://assets.rya.org.uk/assetbank-rya-assets/action/directLinkImage?assetId=53776) for a closed class such as the Hunter 707 do not permit anything not specifically mentioned. However they do permit:

0. Electrical power only socket fitting for portable GPS unit.
0. Stand alone or portable GPS (not interfaced) which may be electrically fed from
the yacht’s battery
0. Self-contained electronic timing devices, and either an electronic digital
compass or an additional magnetic (non-electronic) compass

This rules out masthead wind strength and direction instruments, and cruiser-style GPS systems with wiring to separate chart plotters etc.  However, portable GPS typically contain a chartplotter and all manner of derived properties can be displayed. Since large-screen portable GPS units are fairly expensive, and fiddly to use, it's tempting to consider a more specialised instrument for yacht racing.


There are some excellent specialised electronic navigation aids available for racing. They can seem expensive compared to consumer gadgets, presumably due to the smaller production volumes, and ruggedisation to handle difficult conditions. Some gadgets I am aware of include the [speedpuck](https://www.velocitek.com/pages/speedpuck), [prostart](https://www.velocitek.com/pages/prostart), [sailracer](https://www.sailracer.net/inkdisplay/).

Given there are some stock availability issues with the speedpuck and sailracer display just now, it's attractive to consider developing an open-source hardware system. This has the benefit of allowing the freedom to play about with different ways of representing data during training, e.g. heading variation plotted with time, or chart plotter showing race track (with race committee, pin end buttons, and a potentiometer for distance to top mark, and another for bearing to top mark, as these are often read out over the radio prior to the start). Initially, a button-free device is intended because that is easier to waterproof, and one fewer thing to have a setting that can be wrong.

## Constraints

- Daylight
- Water
- Shocks

A flexible e-ink screen with fast refresh should handle daylight visibility and shocks. It remains to be seen how well an e-ink display refreshes in bright sunlight. Perhaps a UV filtering cover will be required.

Waterproofing will be most straightforward if an existing IP68 enclosure is purchased, perhaps with some 3D printed mounts for the internal components.

## Development hardware

The following items have been purchased for the development work

0. Raspberry Pi Pico £3.60
0. Waveshare 3.7" e-Paper e-Ink Display for Raspberry Pi Pico (480x280) £24.00
0. GNSS Module for Raspberry Pi Pico (L76B) £25.00
0. ML1220 rechargeable battery £3
0. Waveshare UPS Module for Raspberry Pi Pico £19.00
0. 10,000mAh 3.7V Lithium battery £20.99

The total cost so far is £95.59 (nearly a third of the way to the cost of a speedpuck, with no code, no testing, and no case!)

<img src="./img/pico.jpg" alt="Pico" width="200"/>
<img src="./img/e-ink.jpg" alt="E-ink" width="200"/>
<img src="./img/gnss.jpg" alt="GNSS" width="200"/>
<img src="./img/ups.jpg" alt="UPS" width="200"/>
<img src="./img/battery.jpg" alt="Battery" width="200"/>



## Development notes

### Fonts

An updated version of Derek Higg's hand-traced mil spec aviation instrumentation [font](https://www.simpits.org/fileproc/dload.php?file=MS33558FONT.ZIP) is available here:
https://forums.eagle.ru/topic/266630-ms33558-font/


The digits in this font were prepared for the e-ink display in the size 189 high by 144 px wide using [font2bytes](https://github.com/ayoy/font2bytes) GUI version on linux/amd64 (remember to initialise and update submodules to get the gsl library files needed for the build).


## Parts - Original options considered

### GPS data on speed and heading

It's [reported](https://www.u-blox.com/en/press-releases/u-blox-and-velocitek-chosen-several-world%E2%80%99s-top-sailing-teams) that the velocitek SC1 uses ublox GPS, and [speculated](https://portal.u-blox.com/s/question/0D52p00008HKDqaCAH/stolen-device-velocitek-speedpuck-possible-to-locatetrack-the-ublox-antaris-lea4a-chip) that the speedpuck uses the LEA-4A, or [alternatively](https://www.gps-speedsurfing.com/default.aspx?mnu=forum&forum=6&val=100929) LEA-5M.

[GPS with RTC](https://thepihut.com/products/raspberry-pi-gps-hat) £45

[External antenna](https://thepihut.com/products/gps-antenna-external-active-antenna-3-5v-28db-5-meter-sma) £16

### Display
Custom LCD displays are used to get the large numbers and custom heading shift graphics. E-ink is used in the sailracer display. Fast-refresh e-ink could be a useful solution, subject to some concerns around fading during refresh in direct sunlight. 

[A flexible, fast 10" display](https://shop.sb-components.co.uk/products/10-3inch-1872x1404-resolution-flexible-e-ink-display-hat-for-raspberry-pi) for £210

### Case
[IP68 polycarb](https://uk.rs-online.com/web/p/general-purpose-enclosures/7739657) to fit the above display £48
[wall mount](https://uk.rs-online.com/web/p/enclosure-mounting-brackets/7739682) £6
### Raspberry pi 4
[RPI](https://thepihut.com/products/raspberry-pi-4-model-b) 2GB in stock £44

### RPI UPS Hat for charging/lithium battery operation
[UPS](https://thepihut.com/products/raspberry-pi-ups-hat) £16

### Battery 
[10Ah 3,7V](https://amazon.co.uk/Seamuing-Rechargeable-Integrated-Protective-Insulation/dp/B0953L98RK) for £21

### Costing
```
 45 GPS
 16 Antenna
210 Display
 48 Case
  6 Wall mount
 44 Pi
 16 UPS
 21 Battery
-----------
404 Total
```

Plus development work.

## Aspects to consider
Do all of the pi-hats stack?
Would need to 3D print a tray to hold the display in the front window of the case, and mounting tray for the components behind it, including antenna mount on top side.
Making waterproof charging port, or open up each time?
Add buttons to box for controls or do via bluetooth/wifi?

## Alternatives for similar or less expense:

### Inkplate
An alternative approach for less expensive hardware would be to use the original 6" inkplate, which for £99 comes with ESP32 microcontroller, and GPIO pins to which a GPS could be connected. As a glass-plate display it would be more likely to get broken, unless some shock absorbing mounts were used inside the waterproof enclosure.

### Rugged tablet
[mantis](mantistech.co.uk/RhinoTAB) has a 10000mAh battery and an IP68 tablet.

RaceQs does not show track during race.
Sail Racer has received some varied feedback on how well it works
https://tactiqs.io/ is for iOs.

So .... options to consider!






