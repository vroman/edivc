#include <qapplication.h>

#define main_cpp
#include "main.h"
#include "ideqtwindow.h"

#ifdef _WIN32
#	include <windows.h>
#else
#	include <stdlib.h>
#endif


int main( int argc, char** argv )
{
	QApplication app( argc, argv );
	
#ifdef _WIN32
	HKEY subkey;
	if(RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", 0, KEY_READ, &subkey) == ERROR_SUCCESS) {
		DWORD dwType,dwLen;
		unsigned char datos[512];
		RegQueryValueEx(subkey,"Personal",NULL,&dwType,datos,&dwLen);
		RegCloseKey(subkey);
		Main.personal_folder.setLatin1((char*)datos);
	}
#else
	Main.personal_folder.setLatin1(getenv("HOME"));
#endif
	Main.projects_folder = Main.personal_folder + "/" PROJECTS_FOLDER_NAME;

	MainWindow=new ideQTWindow(0,"MainWindow");
	app.setMainWidget(MainWindow);

	MainWindow->show();

	return app.exec();
}

