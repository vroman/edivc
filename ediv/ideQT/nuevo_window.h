#include <qapplication.h>
#include <qwidget.h>


class QWidget;

class nuevo_window : public QWidget
{
	Q_OBJECT
public:
	nuevo_window( QWidget* parent = 0, const char* name = 0, WFlags f = WType_TopLevel );

};

