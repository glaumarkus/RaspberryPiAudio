sudo apt-get install libasound2-dev

check if working
arecord -D hw:1,0 -d 1 -c 2