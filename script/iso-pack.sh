#/bin/sh

# pack up initrd
#cd ../iso/build/initrd
#sudo find . -print0 | cpio --null -ov --format=newc > ../initramfs.img
#cd ..
#sudo gzip initramfs.img

# overwrite verion in data
#sudo mv data/initrd.gz data/initrd.gz.bk
#sudo mv initramfs.img.gz data/initrd.gz

# pack up the final iso
#sudo xorriso -as mkisofs -iso-level 3 -r -V "pnet" -J -joliet-long -append_partition 2 0xef boot.img -partition_cyl_align all -o ../out/pnet.iso data
#sudo xorriso -as mkisofs -iso-level 3 -r -V "pnet" -J -joliet-long -append_partition 2 0x01 boot.img -partition_cyl_align all -o ../out/pnet.iso data


# here
sudo grub-mkrescue -o ../iso/out/pnet.iso ../iso/build/data
