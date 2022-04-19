#!/bin/bash
echo -e "
# csound volume control
pcm.soundscape {
    type softvol
    slave.pcm	\"default\"
    control.name \"Soundscape\"
    control.card 0
}

" > /etc/asound.conf
#Need to try it to make it appear!
apt-get update
apt-get -y install 
aplay -L |grep soundscape || speaker-test -Dsoundscape -c2 -twav -l 1

#assume the ambifier package is installed before EHS.
sed -i 's/-o dac\S*/-o dac:soundscape/g' /etc/ambifier.conf