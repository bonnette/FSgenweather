# FSgenweather
![Client Photo](https://github.com/bonnette/FSgenweather/blob/main/IMG_3409s.jpg)
<br>
A variation of my TFT_WeatherClient repository This is a newer (and different) version of my TFT_WeatherClient. It is built on a Raspberry Pi Zero using C++ and QT creator instead of Python and TK. The FS in "FSweather" stands for Full Screen.The "gen" in the name stands for "general" in that instead of grabbing data from a "SwitchDoc" weather station at the house. I use the api provided by openweathermap.com as well as QJsondocument to extract json data from the website. The Operating system is Buster vertion of Raspian. The case shown is a new version of the case and the pi inside is an older version of a pizero with an Edimax network dongle soldered to the bottom of it.
<br>
The case in the photo is from my Thingiverse repository and can be found here :
<br>
https://www.thingiverse.com/thing:4566312</br></br>

To auto start the application in X-Windows after the X11 server has started edit:</br>
/etc/xdg/lxsession/LXDE-pi/autostart </br></br>
@lxpanel --profile LXDE-pi</br>
@pcmanfm --desktop --profile LXDE-pi</br>
@xscreensaver -no-splash</br>
@xset s off</br>
@xset -dpms</br>
@xset s noblank</br>
@/home/pi/fsweather  <-------- Add this to the end of the file. </br>
(Assuming that fsweather is the application you want to start)</br>

