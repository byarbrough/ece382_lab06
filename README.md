ece382_lab06
============

[Lab 6 - PWM - Robot Motion](http://ece382.com/labs/lab6/index.html)

####Prelab
The prelab to this lab was very important, as without it you were very liekly to end up burning fuses, motor driver chips, and even the MSP430 - not to mention the robot almost certainly wouldn't work. 

###Wiring
The first question I needed to ask myself from a hardware standpoint was what method I wanted to use to output the pulse width modulation signals (PWM) which would power the motors. The lab offers multiple possibilities, and before I decided on which one, I consulted the tech docs.

##The Motor Driver Chip
The [motor driver tech doc](http://ece382.com/datasheets/SN754410.pdf) gives a schematic of the chip and a truth table. Here is the truth table for a single port:

| A | EN | Y |
|:-:|:--:|:-:|
| H |  H | H |
| L |  H | L |
| X |  L | L |

Essentially, when enable is high, the output mirrors the input. This means that I could wire each input to a port on the MSP430 and each output to a terminal on the motor. The issue emerges when you start to think about going backwards.
Misunderstanding the tech doc led me to believe that I could not apply an effectively negative voltage to the input; it must be either HIGH or LOW, which meant that I had to be very creative to run current the opposite direction through the DC motor in order to control direction.

Here is the resulting schematic:
![alt text]( Origional Schematic")

Let me just say, I was extremely excited about this masterpiece. The transistors are NMOS MOSFET transistors. BS-170, to be exact, with a threshold voltage of 2.1 volts, the 3.3 V output would have been enough to turn these on. This would have opened the correct path to ground, and this combined with toggling which PWM signal was being outputted, would have forced current to run through the motor the proper direction.

###Pseudocode
Once I had my schematic drawn up, I had to begin thinking about how I would implement things. I wanted to make it effecient, so many of my controls would be done through #define statements and the rest would be controlled by calling the proper case for a switch. This can be seen in my 
[pseudocode](https://github.com/byarbrough/ece382_lab06/blob/master/pseudoCode.txt) text file.

###The Reality
As it turned out, this prelab was educational and very entertaining, but not ultimately how I built my circuit. First, the maximum continuous current for the BS-170 is 500 mA. This meant that I would be very limited to using only half of my robot's speed and power. Also, I did not understand the Timer A PWM functions. I thought that when the specified value was reached an interrupt was triggered, within which I would have to manually toggle the appropriate bits. On the contrary, using Set / Reset mode automatically SETS and RESETS the pin. Of course, the pin must be tied to the appropraite timer as well as configured to read off of it. But when I looked at [Dr. Coulston's code]( and heard that it alone would cause the robot to drive forward - given proper wiring - I was shocked.

Furthermore, it turns out that a LOW output can be treated as a ground, so I didn't need transisotrs at all (though I still think they would have been AWESOME). This means that I could greatly simplify my schematic:
