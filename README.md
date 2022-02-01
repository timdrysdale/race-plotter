# race-plotter
Open-source hardware-based boat racing GPS plotter

## Background

There are some excellent race gadgets available - at a price, and with limited availability. E.g. [speedpuck](https://www.velocitek.com/pages/speedpuck), [prostart](https://www.velocitek.com/pages/prostart), [sailracer](https://www.sailracer.net/inkdisplay/).

It's attractive to consider an open-source device, to allow playing about with different ways of representing data during training, e.g. heading variation plotted with time, or chart plotter showing race track (with race committee, pin end buttons, and a potentiometer for distance to top mark, and another for bearing to top mark, as these are often read out over the radio prior to the start).

## Constraints

- Daylight
- Water
- Shocks

## Parts

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


 






## Fonts

An updated version of Derek Higg's hand-traced mil spec aviation instrumentation [font](https://www.simpits.org/fileproc/dload.php?file=MS33558FONT.ZIP) is available here:
https://forums.eagle.ru/topic/266630-ms33558-font/


This font was prepared for the e-ink display in the size 210 high by 160 px wide using [font2bytes](https://github.com/ayoy/font2bytes) GUI version on linux/amd64 (remember to initialise and update submodules to get the gsl library files needed for the build).

