#include "aimpl.h"

/* 
 *  Constructs a aImp1 which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
aImp1::aImp1( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : Form1( parent, name, modal, fl )
{
}

/*  
 *  Destroys the object and frees any allocated resources
 */
aImp1::~aImp1()
{
    // no need to delete child widgets, Qt does it all for us
}

