# Qcode-Breakout-Board

![3X PCB](https://github.com/Sciguy429/Qcode-Breakout-Board/blob/main/Images/Qcode-Breakout-PCB-3X-V1-1.png)

This is an in-progress project that attempts to re add Qcode displays to lower end Asus motherboards. Asus has chosen to only provide this feature on its extremely high end boards, which now cost significantly more than they used too. This project started out as a breadboard proof of concept posted to [Reddit](https://www.reddit.com/r/techsupportmacgyver/comments/16v3af7/asus_removed_qcode_displays_from_low_end/) and gained a decent bit of traction. This repository aims to archive all of the work I have done to make this thing a reality. Do keep in mind, I am a hobbyist, not a professional PCB designer. I take no responsibility if you choose to replicate my work and end up frying your motherboard, or worse.

# How it all works
Still here? Cool lets go...

Part of what confused me the most about all of this was just how easy it was to set up. It all started when I purchased a new x570 board to replace my aging Crosshair Vii. The new board of course did not come with a Qcode display but while I was installing it into my case I noticed a weird header on the bottom, the same size as an Arduino ICSP header, labeled 'COM_DEBUG'. I am far to curious for my own good so of course I googled it once my PC was up and running again a few hours later. I found a few differing sites that had references to the thing, a post on the [LTT forums](https://linustechtips.com/topic/1373425-phd-6000-connector-on-asus-mb-what-i-fond/) had the most information, including a pinout found in an official Asus manual. Oddly enough nobody on the forum had done any further testing with there own boards.

The connector pinout in question looks like this:
![COM_DEBUG Pinout](https://github.com/Sciguy429/Qcode-Breakout-Board/blob/main/Images/COM_DEBUG-Pinout.png)

There are quite literally only two pins to try here, and the name 'COM_DEBUG' implies serial to me. So I hooked up a cheap USB to UART to both of them and turns out that the bottom left pin (SOUTC_P80) just dumps out post codes as single bytes at 115200 baud. No weird formatting, no weird speeds, it just dumps em out. (The other pin appears to be something else, whatever protocol it is using, it is not normal serial. I was unable to read anything out of it.)

This kinda floored me, as I quickly realized that it would be trivial to make a cheap MCU read in those bits and put them up onto some 7-segment displays. I then built the original breadboard prototype that is seen in the Reddit post, using some old code from a clock I made years ago to drive the two displays.

# PCB's
Next up was making some PCB's and I currently have two prototype designs.

![1X PCB](https://github.com/Sciguy429/Qcode-Breakout-Board/blob/main/Images/Qcode-Breakout-PCB-V1-0.png)
The first is a one to one clone of the breadboard circuit, using two generic 7-segment displays and a relatively compact board layout.

![3X PCB](https://github.com/Sciguy429/Qcode-Breakout-Board/blob/main/Images/Qcode-Breakout-PCB-3X-V1-1.png)
The second is a much improved version, adding an additional 4 displays driven off of the ADC pins for more potential functionality. This board is much larger though, and more expensive to manufacture.

Prototypes of both of these boards are currently on order from JLPCB and should arrive within a month or so. Anyone interested in them is more than welcome to have their own boards manufactured. I had originally setup tolerances to allow for PCB milling (I have a little engraving machine) but I gave up on this once I created the 3X version. The tolerances are still set though, both designs should be millable, though you will need a nicer mill than I have.

Both designs use a 2x3 pin header for the COM_DEBUG port as well as have a single 1x2 header next to it for use with dupont jumpers. The header is the same size as an Arduino ICSP header and the ribbon cables for them do fit (The header on the board is missing a pin but that dose not prevent the cable from attaching). It is also theoretically possible to solder a female 2x3 header to the backside of the board and plug it directly in, though you will need some good luck with your clearances.

Unfortunately, the COM_DEBUG header dose not provide a +5V on it. It has a +3.3V pin, but no +5V power. As such in order to power the Arduino Mini an auxiliary source of +5V is needed. To the left side of the MCU a header has been provided for this. You can wire up the +5V pin directly to a +5V source, I am using a USB2 header myself, or you can connect the RAW pin to any voltage between +7V-12V (a fan header for instance). The USB2 header has the nice side effect for me of turning the display off when my PC is in hard off power states but not while it is sleeping.

# Firmware
Currently only preliminary firmware is available. This code only works on the 1X board variant and it nearly identical to what was running on the bread board prototype. It is a fairly simple affair that just spends the entire main loop checking for new POST code data and has a 100hz interrupt driving the multiplexed displays. This firmware is likely fine for 98% of potential users though and will likely remain as the only firmware amiable for the simple 1X board.

As for the 3X board, I intend to add a proper settings menu to it using the extra display real estate among other things. By default the displays will likely 'scroll' though the last three codes received, giving you easy access to more than just one of the POST codes your motherboard has sent. It may even be possible to store basic descriptions of the codes and display them on the screens, though that is likely exiting the scope of what a single Atmel-368p can do on it's own.

# Future plans
Someone mentioned something interesting to me on the Reddit post. They were wondering if an ESP32 could plug into the motherboard and send the codes to a phone. This got my attention as ESP32's use +3.3V power, which is already being provided right on the COM_DEBUG connector. I found one for fairly cheep and ordered it, along with a compatible TFT LCD screen. There are a few cool possibilities for this.

It would be pretty easy to design a small board that accepts a cheap ESP32 module and plugs right into the motherboard, supplying both power and serial data to the ESP32. You could then connect to it with a mobile device and have easy access to POST data, all at an extremely cheap cost.

The other idea that I am interested in involves that TFT LCD. The 8-bit MCU in an Arduino simply can't do graphics, but an ESP32 certainly can. It shouldn't be that hard to hook one up to a cheap TFT LCD on a board somewhere between the size as the 1X and 3X. With an LCD the possibilities for the firmware are endless. An ESP32 would be capable of storing thousands of past codes in history and likely a complete lexicon of code descriptions easily displayed on the TFT LCD screen.

# So, are you going to sell it?
Honestly, I am not sure...

I have never sold a PCB project like this before. The base parts for the 1X and even the 3X boards bearly even add up to $10 on Aliexpress. I built the prototype with random stuff that I just had on hand. If there is a solid interest in people who want to buy one, I might consider it, but as of now I want to wait and see.

# Disclaimer
Neither my self or this project is associated with Asus or AsusTEK in any way shape or form. I am an independent hobbiest.
