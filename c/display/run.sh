#!/bin/bash
mkdir build
cd build
cmake ..
make -j4
openocd -f interface/raspberrypi-swd.cfg -f target/rp2040.cfg -c "program epd.elf verify reset"

