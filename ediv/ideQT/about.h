#ifndef __ABOUT_H__
#define __ABOUT_H__

#include <qwidget.h>

class About : public QWidget
{ 
    Q_OBJECT

public:
    About( QWidget* parent = 0, const char* name = 0, WFlags fl = 0, QWidget* padre = 0 );
};

#endif //__ABOUT_H__
