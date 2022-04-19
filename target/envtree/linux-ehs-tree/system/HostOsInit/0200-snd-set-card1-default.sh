#!/bin/bash
echo -e "
pcm.!default {
   type hw
   card 1
}

ctl.!default {
  type hw
   card 1
}
" > /etc/asound.conf
