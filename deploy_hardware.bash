cp myos.iso /boot/myos.iso
rm /etc/grub.d/40_custom
echo "#!/bin/sh
exec tail -n +3 \$0
# This file provides an easy way to add custom menu entries.  Simply type the
# menu entries you want to add after this comment.  Be careful not to change
# the 'exec tail' line above." > /etc/grub.d/40_custom
cat grub.cfg >> /etc/grub.d/40_custom
chmod +x /etc/grub.d/40_custom
grub-mkconfig -o /boot/grub/grub.cfg
