#!/bin/sh

isoDir=../iso
loopDev=loop19

# make a local copy of the initramfs-tools
copy_initramfs() {

    sudo rm -rf $isoDir/initramfs-tools
    mkdir $isoDir/initramfs-tools
    sudo rsync -rva /etc/initramfs-tools/ $isoDir/initramfs-tools

    # overwrite with modifications
    sudo rsync -rva $isoDir/initramfs-tools-src/ $isoDir/initramfs-tools

    return 0
}

# make new initramfs
make_initramfs() {
    # overwrite existing initrd from .iso with ours
    sudo mkinitramfs -v -d $isoDir/initramfs-tools -o $isoDir/data/initrd.gz
    return 0
}

# reconfigure grub
make_grub() {
    # overwrite existing grub config
    sudo cp $isoDir/grub-src/boot/grub/grub.cfg $isoDir/data/boot/grub/grub.cfg
    sudo chmod -w $isoDir/data/boot/grub/grub.cfg
    return 0
}

copy_initramfs
make_initramfs
make_grub

exit 0
