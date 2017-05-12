#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "../source/dumper.hpp"

int main(){
	char urls[10][100] = { 
		"http://www.decolar.com",
		"http://www.tripadvisor.com.br",
		"http://www.latam.com",
		"http://www.voeazul.com.br",
		"http://www.cvc.com.br",
		"http://directory.google.com/Top/Recreation/Outdoors/Hiking/Backpacking/",
		"http://username:password@www.chilkatsoft.com/purchase2.asp#buyZip",
		"http://www.chilkatsoft.com:80/purchase2.asp",
		"https://www.paypal.com:443/",
		"http://www.ChilkatSoft.com/"
	};

	Dumper dumper("test", "test.dump", 0);

	for(int i=0; i<10; i++){
		CkString data;
		data.append(urls[i]);
		dumper.write(data);
	}

	dumper.close();

	return 0;
}