#/bin/sh

isoDir=../iso
loopDev=loop19

# pack bootable iso
pack_iso() {
    # build bootable iso
    sudo grub-mkrescue -v -o $isoDir/pnet.iso $isoDir/data
    return 0
}

pack_iso
