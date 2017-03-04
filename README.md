# Arduino
There are 2 projects in this repo:  
1. Nixie clock with 4 nixie tubes, four IC КБ155ИД1 /KB155ID1 or SN74141/, DHT22, PIR and DS3231 clock module.  
2. Zhivomara is a clock with DHT22, DS3231 module and TM1637 display

I've tried to develop this projects without any "delay" commnands (i.e. sleep), also following best practices as much as could.  
This means the projects are far from polished, hopefully they are easy to understand.

#The Libraries
The libraries used are developed by their respective owners, I've only modified the TM1637Display library to add a method for displaying colons with:  
setColon(bool colon)
where colon parameter is 'true' for displaying the colons and 'false' for turning off the colons.  
Devices used: Arduino pro mini / UNO  

I'm available for questions and clarifications.
