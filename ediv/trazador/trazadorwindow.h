#include <qmainwindow.h>
#include <qtextview.h>


class TrazadorWindow : public QMainWindow
{
	Q_OBJECT
public:
	TrazadorWindow( QWidget* parent = 0, const char* name = 0, WFlags f = WType_TopLevel );
	~TrazadorWindow();

	int CargaListado(char* archivo);
	QTextView *listado;

private:

	char* buffer_listado;
};

