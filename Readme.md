# C-WebServer
A small C HTTP web server which is used to send minor telementary data

## Setup
Currently the WebServer telementery only supports Linux.
By pulling this repo and running make a small "main" file will be compiled.
Starting the Main file with ./main the program will output logs which will show if it started up sucessfully.
By Default the port used for localhost is 8080.

## Endpoints

Endpoint: /cpu \
Returns the cpu temperature. \
Endpoint: /memTotal \
Returns the total memory (in kb) on the machine. \
(uses meminfo) \
Endpoint: /memAvailable \
Returns the total memory (in kb) avaiable on the machine. \
(uses meminfo) \
Endpoint: /totalStorage \
Returns the total storage (in Gb) on the machine. \
Endpoint: /availableStorage \
Returns the total storage (in Gb) avaiable on the machine. \
Endpoint: /getProccessData?process=... \
Returns information about the running process. At the moment it searches for the process with the user steam.
