#include "highlight.h"

#include "kwdoc.h"

#include <qfile.h>
#include <qgrid.h>
#include <qlayout.h>
#include <qvgroupbox.h>
#include <qlist.h>
#include <qgrpbox.h>
#include <qtstream.h>
#include <qlabel.h>
#include <qstringlist.h>
#include <qfileinfo.h>

// ISO/IEC 9899:1990 (aka ANSI C)
// "interrupt" isn´t an ANSI keyword, but an extension of some C compilers
const char *cKeywords[] = {
  "break", "case", "continue", "default", "do", "else", "enum", "extern",
  "for", "goto", "if", "interrupt",  "return", "sizeof", "struct", "switch",
  "typedef", "union", "while", 0L};

const char *cTypes[] = {
  "auto", "char", "const", "double", "float", "int", "long", "register",
  "short", "signed", "static",
  "unsigned", "void", "volatile", 0L};

// DANI/HAMMER 1996:1999 (aka ANSI DIV xD)
const char *divKeywords[] = {
	"begin", "break", "clone", "continue", "debug", "end", "for", "from", "function", 
	"global", "if", "local", "offset", "private", "program", "repeat", "sizeof", 
	"struct", "switch", "to", "until", "while", 0L};

const char *divTypes[] = {
	"byte", "int", "string", "word", 0L};


const char fontSizes[] = {4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,22,24,26,28,32,48,64,0};

//default item style indexes
const int dsNormal = 0;
const int dsKeyword = 1;
const int dsDataType = 2;
const int dsDecVal = 3;
const int dsBaseN = 4;
const int dsFloat = 5;
const int dsChar = 6;
const int dsString = 7;
const int dsComment = 8;
const int dsOthers = 9;


bool testWw(char c) {
  static unsigned char data[] = {0,0,0,0,0,0,255,3,254,255,255,135,254,255,255,7};
  if (c & 128) return false;
  return !(data[c >> 3] & (1 << (c & 7)));
}


// ---------------------------------------------------------------------------

HlItem::HlItem(int attribute, int context)
  : attr(attribute), ctx(context) {
}

// ---------------------------------------------------------------------------

HlItemWw::HlItemWw(int attribute, int context)
  : HlItem(attribute,context) {
}

// ---------------------------------------------------------------------------

HlCharDetect::HlCharDetect(int attribute, int context, char c)
  : HlItem(attribute,context), sChar(c) {
}

const char *HlCharDetect::checkHgl(const char *str) {
    if (*str == sChar) return str + 1;
   return 0L;
  

}

// ---------------------------------------------------------------------------

Hl2CharDetect::Hl2CharDetect(int attribute, int context, const char *s)
  : HlItem(attribute,context) {
  sChar[0] = s[0];
  sChar[1] = s[1];
}

const char *Hl2CharDetect::checkHgl(const char *str) {
  if (str[0] == sChar[0] && str[1] == sChar[1]) return str + 2;
  return 0L;
}

// ---------------------------------------------------------------------------

HlStringDetect::HlStringDetect(int attribute, int context, const char *s)
  : HlItem(attribute,context) {
  len = strlen(s);
  str = new char[len];
  memcpy(str,s,len);
}

HlStringDetect::~HlStringDetect() {
  delete str;
}

const char *HlStringDetect::checkHgl(const char *s) {
  if (memcmp(s,str,len) == 0) return s + len;
  return 0L;
}

// ---------------------------------------------------------------------------

HlRangeDetect::HlRangeDetect(int attribute, int context, const char *s)
  : HlItem(attribute,context) {
  sChar[0] = s[0];
  sChar[1] = s[1];
}

const char *HlRangeDetect::checkHgl(const char *s) {
  if (*s == sChar[0]) {
    do {
      s++;
      if (!*s) return 0L;
    } while (*s != sChar[1]);
    return s + 1;
  }
  return 0L;
}

// ---------------------------------------------------------------------------

KeywordData::KeywordData(const char *str) {
  len = strlen(str);
  s = new char[len];
  memcpy(s,str,len);
}

KeywordData::~KeywordData() {
  delete s;
}

// ---------------------------------------------------------------------------

HlKeyword::HlKeyword(int attribute, int context)
  : HlItemWw(attribute,context) {
  words.setAutoDelete(true);
}

HlKeyword::~HlKeyword() {
}


void HlKeyword::addWord(const char *s) {
  KeywordData *word;
  word = new KeywordData(s);
  words.append(word);
}

void HlKeyword::addList(const char **list) {

  while (*list) {
    addWord(*list);
    list++;
  }
}

const char *HlKeyword::checkHgl(const char *s) {
 int z, count, len;
  KeywordData *word;

  count = words.count();
  len = strlen(s);
  for (z = 0; z < count; z++) {
      word = words.at(z);
      if (len>=word->len && memcmp(s,word->s,word->len) == 0) {
          return s + word->len;
      }
  }
  return 0L;
}

// ---------------------------------------------------------------------------

HlCaseInsensitiveKeyword::HlCaseInsensitiveKeyword(int attribute, int context)
  : HlKeyword(attribute,context) {
}

HlCaseInsensitiveKeyword::~HlCaseInsensitiveKeyword() {
}


const char *HlCaseInsensitiveKeyword::checkHgl(const char *s) {
  int z, count;
  KeywordData *word;

  count = words.count();
  for (z = 0; z < count; z++) {
    word = words.at(z);
#ifndef _WIN32
	if (_strncasecmp(s,word->s,word->len) == 0) {
#else
	if (_strnicmp(s,word->s,word->len) == 0) {
#endif // !_WIN32
      return s + word->len;
    }
  }
  return 0L;
}

// ---------------------------------------------------------------------------
