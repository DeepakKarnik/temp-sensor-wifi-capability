set ip proto 18						                              //Turn on HTTP mode=0x10 and TCP mode=0x2
set dns name wifimodule1.appspot.com			              //set web server name
set ip host 0						                                //Turn on DNS
set ip remote 80					              	              //Set the web server port, 80 is standard
set com remote GET$/wifimodule1.appspot.com/record?   	//Server application
set option format 1					                            //Convert binary data to ASCII hex format
scan							                                      //Scan for available Wi-Fi access points
set wlan passphrase <password>				                  //Enter password for desired available access point
join <access_point_name>					                      //Enter SSID of desired available access point
