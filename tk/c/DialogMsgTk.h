
#ifndef DIALOGMSGTK_H
#define DIALOGMSGTK_H

#include <tcl.h>
#include <tk.h>

/////////////////////////////////////////////////////////////////////////////

class TDialogMsgTk
{
public:
	TDialogMsgTk();
	virtual ~TDialogMsgTk();
	int tkMsgBox(Tcl_Interp *interp, std::string msgStr);
};

#endif
