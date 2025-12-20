#!/bin/sh

# source env
. ./env.sh

# copy aws shared libs
sudo rsync -rva ../lib/aws/linux/bin/ /usr/sbin

# update linker
sudo ldconfig
