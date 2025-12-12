#!/bin/sh

# run the build
cd ../build
make
cd ../script

# copy boot loader
#sudo cp ../build/boot/pnetb ../iso/build/initrd/bin/

