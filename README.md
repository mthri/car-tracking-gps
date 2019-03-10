# Car Tracking by Arduino Uno (GSM a6 mini &amp; GPS NEO-6M)


Before upload project to arduino you must add "Tiny GPS Plus Plus" to your arduino ide.
This liberaray(Tiny GPS Plus Plus) is available on this repository.


Connections:


  GSM a6 mini:
  
    VCC => 5v
    G   => GND
    URX => 11
    UTX => 10


   GPS NEO-6M:
   
    VCC => 3.3v
    GND => GND
    RX  => 3 
    TX  => 2

When you send lc_psw_12345 by sms to this device ,recive location link
