#ifndef _HIGHLIGHT_H_
#define _HIGHLIGHT_H_

#include <qlist.h>
#include <qchkbox.h>
#include <qcombo.h>
#include <qdialog.h>

#define CTX_UNDEF   -1

class TextLine;
class Attribute;

bool testWw(char c); //whole word check: false for '_','0'-'9','A'-'Z','a'-'z'

class HlItem {
  public:
    HlItem(int attribute, int context);
    virtual bool startEnable(char) {return true;}
    virtual bool endEnable(char) {return true;}
    virtual const char *checkHgl(const char *) = 0;
    int attr;
    int ctx;
};

class HlItemWw : public HlItem {
  public:
    HlItemWw(int attribute, int context);
    virtual bool startEnable(char c) {return testWw(c);}
    virtual bool endEnable(char c) {return testWw(c);}
};

class HlCharDetect : public HlItem {
  public:
    HlCharDetect(int attribute, int context, char);
    virtual const char *checkHgl(const char *);
  protected:
    char sChar;
};

class Hl2CharDetect : public HlItem {
  public:
    Hl2CharDetect(int attribute, int context, const char *);
    virtual const char *checkHgl(const char *);
  protected:
    char sChar[2];
};

class HlStringDetect : public HlItem {
  public:
    HlStringDetect(int attribute, int context, const char *);
    virtual ~HlStringDetect();
    virtual const char *checkHgl(const char *);
  protected:
    char *str;
    int len;
};

class HlRangeDetect : public HlItem {
  public:
    HlRangeDetect(int attribute, int context, const char *);
    virtual const char *checkHgl(const char *);
  protected:
    char sChar[2];
};

class KeywordData {
  public:
    KeywordData(const char *);
    ~KeywordData();
    char *s;
    int len;
};

class HlKeyword : public HlItemWw {
  public:
    HlKeyword(int attribute, int context);
    virtual ~HlKeyword();
    void addWord(const char *);
    void addList(const char **);
    virtual const char *checkHgl(const char *);
  protected:
    QList<KeywordData> words;
};

class HlCaseInsensitiveKeyword : public HlKeyword {
  public:
    HlCaseInsensitiveKeyword(int attribute, int context);
    virtual ~HlCaseInsensitiveKeyword();
    virtual const char *checkHgl(const char *);
};

#endif //_HIGHLIGHT_H_
