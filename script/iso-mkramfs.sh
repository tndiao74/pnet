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

# unpack source iso
unpack_iso() {

    sudo losetup /dev/$loopDev $isoDir/ubuntu-focal-mini-amd64.iso

    # mount the data
    sudo rm -rf $isoDir/mnt
    mkdir $isoDir/mnt
    sudo mount /dev/$loopDev $isoDir/mnt

    # copy the data
    sudo rm -rf $isoDir/data
    mkdir $isoDir/data
    sudo rsync -rva $isoDir/mnt/ $isoDir/data

    # clean up
    sudo umount $isoDir/mnt
    sudo losetup -d /dev/$loopDev

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

# pack bootable iso
pack_iso() {
    # build bootable iso
    sudo grub-mkrescue -v -o $isoDir/pnet.iso $isoDir/data
    return 0
}

#unpack_iso
copy_initramfs
make_initramfs
make_grub
pack_iso

exit 0



# mount the data
mkdir ../iso/mnt
sudo mount /dev/loop18 ../iso/mnt

# copy the data
sudo rm -rf ../iso/data
mkdir ../iso/data
sudo rsync -rva ../iso/mnt/ ../iso/data

exit 0

# make a local copy of boot directory
sudo rm -rf ../iso/boot
mkdir ../iso/boot
sudo rsync -rva /boot/ ../iso/boot

# make new grub config
sudo mv /etc/default/grub /etc/default/grub_bk
sudo cp ../iso/grub-src/grub /etc/default/grub
sudo grub-mkconfig -o ../iso/boot-src/grub/grub.cfg
sudo mv /etc/default/grub_bk /etc/default/grub

# overwrite with modifications
sudo rsync -rva ../iso/boot-src/ ../iso/boot

# build bootable iso
sudo grub-mkrescue -v -o ../iso/pnet.iso ../iso/boot
