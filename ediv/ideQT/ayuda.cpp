/****************************************************************************
** Form implementation generated from reading ui file '.\ayuda.ui'
**
** Created: Wed Jul 10 21:15:41 2002
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "ayuda.h"

#include <qgroupbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlistbox.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/* 
 *  Constructs a Form1 which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
Form1::Form1( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "Form1" );
    resize( 463, 405 ); 
    setProperty( "caption", tr( "Form1" ) );
    Form1Layout = new QVBoxLayout( this ); 
    Form1Layout->setSpacing( 6 );
    Form1Layout->setMargin( 11 );

    GroupBox1 = new QGroupBox( this, "GroupBox1" );
    GroupBox1->setProperty( "title", tr( "GroupBox1" ) );
    GroupBox1->setColumnLayout(0, Qt::Vertical );
    GroupBox1->layout()->setSpacing( 0 );
    GroupBox1->layout()->setMargin( 0 );
    GroupBox1Layout = new QHBoxLayout( GroupBox1->layout() );
    GroupBox1Layout->setAlignment( Qt::AlignTop );
    GroupBox1Layout->setSpacing( 6 );
    GroupBox1Layout->setMargin( 11 );

    Layout2 = new QHBoxLayout; 
    Layout2->setSpacing( 6 );
    Layout2->setMargin( 0 );

    Layout1 = new QGridLayout; 
    Layout1->setSpacing( 6 );
    Layout1->setMargin( 0 );

    LineEdit1 = new QLineEdit( GroupBox1, "LineEdit1" );

    Layout1->addWidget( LineEdit1, 0, 1 );

    LineEdit2 = new QLineEdit( GroupBox1, "LineEdit2" );

    Layout1->addWidget( LineEdit2, 1, 1 );

    TextLabel1 = new QLabel( GroupBox1, "TextLabel1" );
    TextLabel1->setProperty( "text", tr( "TextLabel1" ) );

    Layout1->addWidget( TextLabel1, 0, 0 );

    TextLabel2 = new QLabel( GroupBox1, "TextLabel2" );
    TextLabel2->setProperty( "text", tr( "TextLabel2" ) );

    Layout1->addWidget( TextLabel2, 1, 0 );
    Layout2->addLayout( Layout1 );

    ListBox1 = new QListBox( GroupBox1, "ListBox1" );
    ListBox1->insertItem( tr( "New Item" ) );
    Layout2->addWidget( ListBox1 );
    GroupBox1Layout->addLayout( Layout2 );
    Form1Layout->addWidget( GroupBox1 );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
Form1::~Form1()
{
    // no need to delete child widgets, Qt does it all for us
}

