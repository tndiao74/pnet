#!/bin/sh

# source env
. ./env.sh

# unpack initrd image
pack_iso() {

    # build bootable iso
    sudo grub-mkrescue -v -o $diriso/pnet.iso $diriso/data

    return 0
}

# run mkinitramfs to make initial file system image
make_initrd() {

    # backup and overwrite configuration files
    scriptPath="init-bottom"
    sudo mv /etc/initramfs-tools/initramfs.conf /etc/initramfs-tools/initramfs.conf.bk
    sudo cp $diriso/mods/initramfs.conf /etc/initramfs-tools
    sudo cp $diriso/mods/pnetb /etc/initramfs-tools/hooks
    sudo cp $diriso/mods/master-init /etc/initramfs-tools/scripts/$scriptPath/master-init

    # build the ram fs img
    sudo mkinitramfs -v -o $diriso/initrd.img

    # restore
    sudo mv /etc/initramfs-tools/initramfs.conf.bk /etc/initramfs-tools/initramfs.conf
    sudo rm -rf /etc/initramfs-tools/hooks/pnetb
    sudo rm -rf /etc/initramfs-tools/scripts/$scriptPath/master-init

    return 0
}

# unpack initrd image
unpack_initrd() {

    sudo rm -rf $diriso/initrd
    sudo mkdir $diriso/initrd
    sudo unmkinitramfs $diriso/initrd.img $diriso/initrd

    return 0
}

# make iso data
make_data() {

    sudo rm -rf $diriso/data
    sudo mkdir $isoData/data

    # grub config
    sudo mkdir -p $diriso/data/boot/grub
    sudo cp $diriso/mods/grub.cfg $diriso/data/boot/grub

    # initrd and kernel
    sudo cp $diriso/initrd.img $diriso/data/initrd
    sudo cp /boot/vmlinuz $diriso/data/linux
    
    return 0
}

# make the iso
make_initrd
#unpack_initrd
make_data
pack_iso

echo ""
echo "*** ISO FINISHED ***"
echo ""

exit 0
