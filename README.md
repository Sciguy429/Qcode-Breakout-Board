# Qcode-Breakout-Board

![3X PCB](https://github.com/Sciguy429/Qcode-Breakout-Board/blob/main/Images/Qcode-Breakout-PCB-3X-V1-1.png)

This is an in-progress project that attempts to re add Qcode displays to lower end Asus motherboards. Asus has chosen to only provide this feature on its extremely high end boards, which now cost significantly more than they used too. This project started out as a breadboard proof of concept posted to [Reddit](https://www.reddit.com/r/techsupportmacgyver/comments/16v3af7/asus_removed_qcode_displays_from_low_end/) and gained a decent bit of traction. This repository aims to archive all of the work I have done to make this thing a reality. Do keep in mind, I am a hobbyist, not a professional PCB designer. I take no responsibility if you choose to replicate my work and end up frying your motherboard, or worse.

# How it all works
Still here? Cool lets go...

Part of what confused me the most about all of this was just how easy it was to set up. It all started when I purchased a new x570 board to replace my aging Crosshair Vii. The new board of course did not come with a Qcode display but while I was installing it into my case I noticed a weird header on the botton, the same size as an Arduino ICSP header, labled 'COM_DEBUG'. I am far to currious for my own good so of course I googled it once my PC was up and running again a few hours later. I found a few differing sites that had references to the thing, a post on the [LTT fourms](https://linustechtips.com/topic/1373425-phd-6000-connector-on-asus-mb-what-i-fond/) had the most information, including a pinout found in an offical Asus manual. Oddly enough nobody on the fourm had done any further testing with there own boards.\

The connector pinout in question looks like this:
![COM_DEBUG Pinout](https://github.com/Sciguy429/Qcode-Breakout-Board/blob/main/Images/COM_DEBUG-Pinout.png)

There are quite literally only two pins to try here, and the name 'COM_DEBUG' implies serial to me. So I hooked up a cheap USB to UART to both of them and turns out that the bottom left pin (SOUTC_P80) just dumps out post codes as single bytes at 115200 baud. No weird formatting, no weird speeds, it just dumps em out. (The other pin appears to be something else, whatever protocol it is using, it is not normal serial. I was unable to read anything out of it.)

This kinda floored me, as I quickly realized that it would be trvial to make a cheap MCU read in those bits and put them up onto some 7-segment displays. I then built the orginal breadboard prototype that is seen in the Reddit post, using some old code from a clock I made years ago to drive the two displays.
