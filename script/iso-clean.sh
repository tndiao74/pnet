#!/bin/sh

. ./env.sh

# remove data
sudo rm -rf $diriso/data
sudo rm -rf $diriso/initrd
sudo rm -rf $diriso/pnet.iso
sudo rm -rf $diriso/initrd.img
