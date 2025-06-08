<h1 align="left">Library to interface with the 24c02</h1>
<!-- 24c02 [About the project]-->



<!-- 24c02 [Description]-->
<p>
 I did this project years ago while working with a custom build ATMEGA328 microcontroller. I needed a way to effectively store data in a non-volatile format that didn't 
  rely on the onboard EEPROM. With implementing an external option for maintaining important state variable retention between power cycles, this would ultimately allow for faster integration of newer 
  technologies like FRAM, into the overall design of the board and undoubtedly supersede the limitations of EEPROM, for future board revisions. 
</p>

## About the Project
<p>
 The main reason behind why I needed to implement something like the 24c02 into my project was that I needed a way to store updated information so the system could become smarter or at least adapt to changes
  and control unexpected issues that would arise. The 24c02 was initially de-soldered off of a computer monitor, and since it’s cheaper to test out issues on prototypes versus making new boards in-between 
  major revisions, the best option was to do an add-on to the existing board. 
</p>
<p align="center">
  <img src="https://github.com/TacTechGuy/24c02_EEPROM/blob/master/images/24c02_mounted.png?raw=true"/>
  <br>
  <i>The 24c02 was soldered to a custom 8-SOIC breakout board that was initially designed for a TMP75, which unfortunately was all that I had readily available at the time. Some minor adjustments had to be performed to accommodate the differences between the boards, 
  like drilling the VCC line and jumping ground through the SMD capacitor pads to the solder pads for adjustable pin configuration for I2C addressing.</i><br>
  <br>
  <img src="https://github.com/TacTechGuy/24c02_EEPROM/blob/master/images/board_topSize.png?raw=true"/><br>  
  <i>This picture is for reference only and is used to show the constraints of all of the THT and SMD's that prevented the 24c02 from being mounted on top. However, mounting on bottom allowed for easy soldering to the available pins for 
  I2C, plus the additional write control pin. </i><br>
</p>





## Built With
<img src="https://cdn.worldvectorlogo.com/logos/arduino-1.svg" alt="arduino" width="40" height="40"/> <img src="https://raw.githubusercontent.com/devicons/devicon/master/icons/cplusplus/cplusplus-original.svg" alt="cplusplus" width="40" height="40"/>

## Usage
<p>
One thing that needs to be considered when working with this IC is that you will need an additional digital pin for write protection. Furthermore, if you look through the code you will see that only a couple of addresses support the use of 
  float values. At the time (2023) of starting this project, I only needed use of one float value which occupies 2 locations starting at 0x13. The process breaks up a float and stores it across two memory locations. This same task can be easily 
  implemented if storing multiple float values throughout the IC is desired. The code was initially tested on a breadboard utilizing a SAMD21g18A, additionally you can reference the <a href="https://github.com/TacTechGuy/24c02_EEPROM/blob/master/revisionHistory.txt">revisionHistory.txt</a>
  file which outlines my progression from initial setup, to testing and working through bugs.
</p>

## Contact
Ethan Dechant - [LinkedIn](https://www.linkedin.com/in/ethan-dechant/) - ethandechant22@gmail.com

Project Link: [https://github.com/github_TacTechGuy/24c02_EEPROM](https://github.com/TacTechGuy/24c02_EEPROM)
