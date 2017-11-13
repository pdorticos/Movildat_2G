# README

This repository is created for testing and developping tools for the main project.

We will try to list and briefly explain each function within this document.

##  TCP Client

This tool could be useful for customizing conections to any server.

##  HEADER

Prints a string with the following aplication layer format:

| Segment | Length (B) | HX digits |
|     :---:      | :---         | :---         |
|Timestamp|4|8|
|Timestamp ext|1|2|
|Priority|1|2|
|Longitude|4|8|
|Latitude|4|8|
|Altitude|2|4|
|Angle|2|4|
|Satelites|1|2|
|Speed|2|4|
|HDOP|1|2|
|ID of IO|1|2|

## dt2ts: Device time to timestamp

Conversion form device's time format to 4 byte timestamp as required.

    usage: ./dtme2TS YY/MM/DD,HH:MM:SS+ZZ


## Format Header    
Fills a struct containing header info.

    usage: ./format YY/MM/DD,HH:MM:SS+ZZ tstamXtension priority

