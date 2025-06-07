<h1 align="left">Library to interface with the 24c02</h1>
<!-- 24c02 [Description]-->
<p>
  This project was done years ago while working with Arduino microcontrollers. I needed a way to effectively store data in a non-volatile format that didn't
  utilize the onboard EEPROM, local to the ATMEGA328. To effectively retain important state variables between power cycles while also allowing for easier adaptation 
  of different technologies like FRAM to be integrated into the overall design of the board, that would supersede the limitations of EEPROM. 
</p>

## About the Project
<p align="center">
  <img src="https://github.com/TacTechGuy/24c02_EEPROM/blob/master/images/24c02_mounted.jpg?raw=true" height="700" width="700"/>
  <br>
  <i>The 24c02 was soldered to an 8-SOIC breakout board which was designed for a TMP75 and it was all that I had readily available at the time. Some minor adjustments had to be performed to accommodate the differences between the boards, 
  like drilling the VCC line and jumping ground through the SMD capacitor pads to the solder pads for adjustable pin configuration for I2C addressing.</i><br>
  <br>
  <img src="https://github.com/TacTechGuy/24c02_EEPROM/blob/master/images/board_topSize.jpg?raw=true" height="700" width="700"/><br>  
  <i>This picture is for reference only and is used to show the constraints of all of the THT and SMD's that prevented the 24c02 from being mounted on top. However, mounting on bottom allowed for easy soldering to the available pins for 
  I2C, plus the additional write control pin. </i><br>
</p>

<!-- 24c02 [About the project]-->
<p>
 
</p>



## Built With
<img src="https://cdn.worldvectorlogo.com/logos/arduino-1.svg" alt="arduino" width="40" height="40"/> <img src="https://raw.githubusercontent.com/devicons/devicon/master/icons/cplusplus/cplusplus-original.svg" alt="cplusplus" width="40" height="40"/>

## Contact
Ethan Dechant - [LinkedIn](https://www.linkedin.com/in/ethan-dechant/) - ethandechant22@gmail.com

Project Link: [https://github.com/github_TacTechGuy/24c02_EEPROM](https://github.com/TacTechGuy/24c02_EEPROM)
