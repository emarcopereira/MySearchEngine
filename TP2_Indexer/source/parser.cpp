#include "parser.hpp"

#include <cstring>
#include <cstdio>

using namespace std;

Parser::Parser(){
}

string Parser::getCleanText(GumboNode* node) const{
	if(node->type == GUMBO_NODE_TEXT) {
		return string(node->v.text.text);

    } else if (node->type == GUMBO_NODE_ELEMENT && node->v.element.tag != GUMBO_TAG_SCRIPT
    											&& node->v.element.tag != GUMBO_TAG_STYLE) {
		string contents = "";
    	GumboVector* children = &node->v.element.children;

    	for(unsigned int i = 0; i < children->length; ++i) {
    		const string text = getCleanText((GumboNode*) children->data[i]);
    		if (i != 0 && !text.empty()) {
    			contents.append(" ");
    		}
    		contents.append(text);
    	}

    	return contents;
	} else { return ""; }
}

UnicodeString TRANSLITERATION_RULES(
    ":: any-NFKD;"									/* Converts to Compatibility Decomposed Normalization */
    ":: [[:Nonspacing Mark:][']] remove;"			/* Removes Accents and apostrophe */
    "[^[:Letter:][:Number:][:WHITESPACE:]] > ' ';"	/* Replaces Special Characters by Space */
    ":: lower;"										/* Sets all to lowercase */
    ":: any-NFKC;"									/* Converts to Compatibility Composed Normalization */
    );

queue<string> Parser::getTerms(const string &html, StopWordsTable *stop_words) const{
	/* Cleaning Html Text */
	GumboOutput *output = gumbo_parse(html.c_str());
	string text = this->getCleanText(output->root);
	text.shrink_to_fit();
	gumbo_destroy_output(&kGumboDefaultOptions, output);
	//fprintf(stderr, "PARSED:%s\n", text.c_str());

	/* Formating Html Text */
	//UnicodeString source = UnicodeString::fromUTF8(StringPiece(text));
	UnicodeString source = UnicodeString(text.c_str());
  	UErrorCode status = U_ZERO_ERROR;
  	UParseError parse_error;
  	/*Transliterator *converter = Transliterator::createInstance(
  		"any-NFD; [^[:Letter:][:Number:][:WHITESPACE:]] any-remove; lower; any-NFC;", UTRANS_FORWARD, U_ZERO_ERROR, status);*/
  	Transliterator *converter = Transliterator::createFromRules("", TRANSLITERATION_RULES, UTRANS_FORWARD, parse_error, status);
  	converter->transliterate(source);
  	string result;
  	source.toUTF8String(result);
  	//fprintf(stderr, "FORMATED:%s\n", result.c_str());

	/* Tokenizing Html Text */
	queue<string> terms;
	string term;
	char *token = strtok(const_cast<char *>(result.c_str()), " \n"); 
	while(token != NULL){
		string term = token;
		if(term.size() != 0){
    		if(not stop_words->constains(term))
    			terms.push(term);
    	}

    	token = strtok(NULL, " \n"); 
	}
  
	return terms;
}