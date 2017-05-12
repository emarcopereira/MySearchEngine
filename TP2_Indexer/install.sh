
#sudo apt-get update
sudo apt-get install libicu-dev libcunit1-dev automake autotools-dev build-essential libtool

cd gumbo-parser
./autogen.sh
./configure
make
sudo make install