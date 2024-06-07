# Requirements
## Hardware
<ul>
  <li>Arduino Uno</li>
  <li>8×8 LED Matrix MAX7219</li>
  <li>5 males to females pin cables</li>
</ul>

## Software
<ul>
  <li>Aseprite</li>
  <li>PlatformIO</li>
  <li>(The project relies on the LedControl library : https://github.com/wayoda/LedControl)</li>
</ul>

# How to use
<ul>
  <li>In Aseprite, drop the file ./aseprite/script/ByteConverter.lua in the Aseprite script folder</li>
  <li>Upload main.cpp to the Arduino Uno</li>
  <li>In Aseprite, run the script ByteConverter to launch the FramesAPI.exe and display the sprites to the LED matrix</li>
</ul>

# To Do (if i ever decide not being lazy)
<ul>
  <li>Use binary instead of a txt file to send the bytes</li>
  <li>Add some contrast using the luminosity</li>
  <li>Fix the Bug that mess up the conversion to bytes when one row or column is empty (contains no pixel) in the sprite</li>
</ul>

# DEMO (homemade quality U_U)

https://github.com/NicolasDortu/Aseprite-LED-Matrix-Uno/assets/126513916/d5b2b0f2-d4da-4e05-8282-d0466711cfc8

