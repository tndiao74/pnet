#/bin/sh

# make loop device for data partition
# sudo losetup --offset 32768 --sizelimit 6340313088 /dev/loop18 ../source/ubuntu-focal-mini-amd64.iso
sudo losetup --offset 0 --sizelimit 71303168 /dev/loop18 ../iso/source/ubuntu-focal-mini-amd64.iso

# mount the data
sudo mount /dev/loop18 ../iso/mnt/iso-data

# copy the data
sudo rsync -rva ../iso/mnt/iso-data/ ../iso/build/data

# clean up
sudo losetup -d /dev/loop18
sudo umount ../iso/mnt/iso-data

# unpack initramfs
#sudo unmkinitramfs -v data/casper/initrd initrd/
sudo unmkinitramfs -v ../iso/build/data/initrd.gz ../iso/build/initrd/

# copy over base changes
sudo cp ../iso/iso-changes/boot/grub/grub.cfg ../iso/build/data/boot/grub/grub.cfg
sudo cp ../iso/iso-changes/initrd/init ../iso/build/initrd/init

# make device for EFI partition
# sudo losetup --offset 6340345856 --sizelimit 5201920 /dev/loop18 ../source/ubuntu-24.04.3-desktop-amd64.iso
#sudo losetup --offset 71303168 --sizelimit 6291456 /dev/loop18 ../source/ubuntu-focal-mini-amd64.iso

# dd partition to boot image
#sudo dd if=/dev/loop18 of=boot.img

# clean up
#sudo losetup -d /dev/loop18
