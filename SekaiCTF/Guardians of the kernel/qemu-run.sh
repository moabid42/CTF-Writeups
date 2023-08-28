#!/bin/sh

/usr/bin/qemu-system-x86_64 \
    -m 64M \
    -kernel bzImage \
    -initrd initramfs-patched.cpio.gz \
    -nographic \
    -monitor none \
    -no-reboot \
    -cpu kvm64,+smep,+smap \
    -virtfs local,security_model=mapped-xattr,path=./shared,mount_tag=shared \
    -append "console=ttyS0 nokaslr quiet" $@
