#/bin/sh

isoDir=../iso
loopDev=loop19

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

unpack_iso