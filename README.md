# Qcode-Breakout-Board

![3X PCB](https://github.com/Sciguy429/Qcode-Breakout-Board/blob/main/Images/Qcode-Breakout-PCB-3X-V1-1.png)

This is an in-progress project that attempts to re add Qcode displays to lower end Asus motherboards. Asus has chosen to only provide this feature on its extremely high end boards, which now cost significantly more than they used too. This project started out as a breadboard proof of concept posted to [Reddit](https://www.reddit.com/r/techsupportmacgyver/comments/16v3af7/asus_removed_qcode_displays_from_low_end/) and gained a decent bit of traction. This repository aims to archive all of the work I have done to make this thing a reality. Do keep in mind, I am a hobbyist, not a professional PCB designer. I take no responsibility if you choose to replicate my work and end up frying your motherboard, or worse.

# How it all works
Still here? Cool lets go...

Part of what confused me the most about all of this was just how easy it was to set up. It all started when I purchased a new x570 board to replace my aging Crosshair Vii. The new board of course did not come with a Qcode display but while I was installing it into my case I noticed a weird header on the botton, the same size as an Arduino ICSP header, labled 'COM_DEBUG'. I am far to currious for my own good so of course I googled it once my PC was up and running again a few hours later. I found a few differing sites that had references to the thing, a post on the [LTT fourms](https://linustechtips.com/topic/1373425-phd-6000-connector-on-asus-mb-what-i-fond/) had the most information, including a pinout found in an offical Asus manual. Oddly enough nobody on the fourm had done any further testing with there own boards.

The connector pinout in question looks like this:
![COM_DEBUG Pinout](https://github.com/Sciguy429/Qcode-Breakout-Board/blob/main/Images/COM_DEBUG-Pinout.png)

There are quite literally only two pins to try here, and the name 'COM_DEBUG' implies serial to me. So I hooked up a cheap USB to UART to both of them and turns out that the bottom left pin (SOUTC_P80) just dumps out post codes as single bytes at 115200 baud. No weird formatting, no weird speeds, it just dumps em out. (The other pin appears to be something else, whatever protocol it is using, it is not normal serial. I was unable to read anything out of it.)

This kinda floored me, as I quickly realized that it would be trvial to make a cheap MCU read in those bits and put them up onto some 7-segment displays. I then built the orginal breadboard prototype that is seen in the Reddit post, using some old code from a clock I made years ago to drive the two displays.

# PCB's
Next up was making some PCB's and I currently have two prototype designs.

![1X PCB](https://github.com/Sciguy429/Qcode-Breakout-Board/blob/main/Images/Qcode-Breakout-PCB-V1-0.png)
The first is a one to one clone of the breadboard circuit, using two generic 7-segment displays and a relatively compact board layout.

![3X PCB](https://github.com/Sciguy429/Qcode-Breakout-Board/blob/main/Images/Qcode-Breakout-PCB-3X-V1-1.png)
The second it a much improved version, adding an adtional 4 displays driven off of the ADC pins for more potential functionality. This board is much larger though, and more expensive to manufacture.

Prototypes of both of these boards are currently on order from JLPCB and should arrive within a month or so. Anyone interested in them is more than welcome to have their own boards manufactured. I had orginally setup tollerneces to allow for PCB milling (I have a little engraving machine) but I gave up on this once I created the 3X version. The tollerences are still set though, both designs should be millable, though you will need a nicer mill than I have.

Both designes use a 2x3 pin header for the COM_DEBUG port as well as have a single 1x2 header next to it for use with dupont jumpers. The header is the same size as an Arduino ICSP header and the ribbon cables for them do fit (The header on the board is missing a pin but that dose not prevent the cable from attaching). It is also therecitally possible to solder a female 2x3 header to the backside of the board and plug it dirrectly in, though you will need some good luck with your clearences.

Unforunatly, the COM_DEBUG header dose not provide a +5V on it. It has a +3.3V pin, but no +5V power. As such in order to power the Arduino Mini an auxilarly source of +5V is needed. To the left side of the MCU a header has been provided for this. You can wire up the +5V pin dirrectly to a +5V source, I am using a USB2 header myself, or you can connect the RAW pin to any voltage between +7V-12V (a fan header for instance). The USB2 header has the nice side effect for me of turning the display off when my PC is in hard off power states but not while it is sleeping.

# Firmware
