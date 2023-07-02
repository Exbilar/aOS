if ./grub/grub-file --is-x86-multiboot aOS.bin; then
  echo multiboot confirmed
else
  echo the file is not multiboot
fi
