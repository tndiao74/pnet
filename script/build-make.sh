#!/bin/sh

# run the build
cd ../src/build
make
cd ../../script

# copy boot loader
sudo cp ../src/build/boot/pnetb ../iso/build/initrd/bin/

