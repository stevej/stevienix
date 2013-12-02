#!/bin/bash -v

# Originally borrowed from rhombus OS: https://github.com/nickbjohnson4224/rhombus

export BUILDDIR=${PWD}

mkdir -p ${BUILDDIR}/scratch/cd/boot/grub
cp ${BUILDDIR}/grub/* ${BUILDDIR}/scratch/cd/boot/grub

#tar -C ${BUILDDIR}/boot -vvcf ${BUILDDIR}/run/boot.tar `ls ${BUILDDIR}/boot`
#cp ${BUILDDIR}/run/boot.tar ${BUILDDIR}/run/cd/boot.tar
#cp ${BUILDDIR}/sbin/init ${BUILDDIR}/run/cd/init
#cp ${BUILDDIR}/lib/dl.so ${BUILDDIR}/run/cd/dl.so
cp ${BUILDDIR}/src/kernel.elf ${BUILDDIR}/scratch/cd

mkisofs -vvv -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -o stevienix.iso scratch/cd
