/****************************************************************************
** Form interface generated from reading ui file '.\ayuda.ui'
**
** Created: Wed Jul 10 21:15:41 2002
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef FORM1_H
#define FORM1_H

#include <qvariant.h>
#include <qdialog.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QGroupBox;
class QLabel;
class QLineEdit;
class QListBox;
class QListBoxItem;

class Form1 : public QDialog
{ 
    Q_OBJECT

public:
    Form1( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~Form1();

    QGroupBox* GroupBox1;
    QLineEdit* LineEdit1;
    QLineEdit* LineEdit2;
    QLabel* TextLabel1;
    QLabel* TextLabel2;
    QListBox* ListBox1;

protected:
    QVBoxLayout* Form1Layout;
    QHBoxLayout* GroupBox1Layout;
    QHBoxLayout* Layout2;
    QGridLayout* Layout1;
};

#endif // FORM1_H
