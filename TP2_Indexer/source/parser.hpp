#ifndef _parser_hpp
#define _parser_hpp

#include <string>
#include <vector>

#include "vocabulary.hpp"
#include "stop_words_table.hpp"

#include <unicode/utypes.h>
#include <unicode/unistr.h>
#include <unicode/translit.h>
#include "gumbo.h"

class Parser{

	private:

		std::string getCleanText(GumboNode* node) const;

	public:

		Parser();

		std::vector<std::string> getTerms(std::string &html, StopWordsTable &stop_words) const;

};

#endif