#!/bin/bash
if [ -z "${INX_SERIAL}" ]; then
	echo "Please set INX_SERIAL to the serial number to print"
	exit
fi
if [ -z "${INX_PIN}" ]; then
	echo "Please set INX_PIN to the PIN to print"
	exit
fi
if [ 1 == 0 ]; then

cp ./template.html ./toprint.html
sed -i "s/SERIAL/${INX_SERIAL}/g" ./toprint.html
sed -i "s/PIN/${INX_PIN}/g" ./toprint.html
#xvfb-run wkhtmltopdf -O landscape -p Letter ./toprint.html ./toprint.pdf
xvfb-run wkhtmltoimage --disable-smart-width --width 400 ./toprint.html ./text.png
convert text.png -crop 125x85+145+25 cropped.png
convert backerlogo.png cropped.png -geometry +130+20 -composite toprint.png
lp -d ql500 -o landscape -o media=ShippingLabel ./toprint.png
#rm ./toprint.pdf
else
echo -e "DEVMAN ID:    \tHRDx-v1.0" > serials.txt
echo -e "${INX_SERIAL} \tPIN: ${INX_PIN}" >> serials.txt
#This works for a QL-500 (ubuntu's ppd config file at least)
lp -d QL-500 -o landscape -o lpi=8 -o cpi=12 -o media=CompactFlashTape serials.txt

#lp -d ql500 -o landscape -o media=ShippingLabel serials.txt

fi
