#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>

#include <cstring>
#include <array>

/*#include <unicode/utypes.h>
#include <unicode/unistr.h>
#include <unicode/translit.h>*/

#include "../source/parser.hpp"
#include "../source/indexer.hpp"

using namespace std;

int main(){

	//for(int i=0; i<256; i++)
	//	printf("%d: %c\n", i, i);
	/*int a[500];
	for(int i=0; i<500; i++) a[i] = i;

	FILE *arq = fopen("test.txt", "w");
	fprintf(arq, "%c\n", 134);
	fclose(arq);*/

	string str = "<html> <script> for </script> La mort <body> d'Olivier, Bécaille. -- Émile Zola </body> </html> ç ñ .?'!ºª[]{}*//%.$&@ 12345 a123";
	/*UnicodeString source = UnicodeString::fromUTF8(StringPiece(str));
	UErrorCode status = U_ZERO_ERROR;
	Transliterator *converter = Transliterator::createInstance(
	      "NFKD; [:Mn:] Remove; NFKC", UTRANS_FORWARD, status);
	converter->transliterate(source);
	string result;
	source.toUTF8String(result);
	cout << result << '\n';*/

	Parser parser;

	FILE *sw_file = fopen("input/br_en_stopwords.txt", "r");
	//Indexer indexer(sw_file);

	//fprintf(stderr, "STRING: %s\n", str.c_str());

	string text;
	ifstream ifs("test/one_html.txt", std::ifstream::in);
	getline(ifs, text);

	//fprintf(stderr, "STRING: %s\n", text.c_str());

	/*const queue<string> &terms = parser.getTerms(text, indexer.stop_words_table);

	FILE *out_file = fopen("test/out.txt", "w");
	int size = terms.size();
	while(size--){
		fprintf(out_file, "%s\n", terms.front().c_str());
	}
	fclose(out_file);

	for(auto item : indexer.stop_words_table.stop_words)
		fprintf(stderr, "%s\n", item.first.c_str());*/

	fclose(sw_file);

	return 0;
}