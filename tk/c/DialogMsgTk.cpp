#include "stdafx.h"
#include "DialogMsgTk.h"

/////////////////////////////////////////////////////////////////////////////
TDialogMsgTk::TDialogMsgTk(){}
TDialogMsgTk::~TDialogMsgTk(){}
/////////////////////////////////////////////////////////////////////////////

int TDialogMsgTk::tkMsgBox(Tcl_Interp *interp, std::string msgStr)
{
	int tclEval;
	char* message = const_cast<char*>(msgStr.c_str());
	Tcl_SetVar(interp, "errormsg", message, 0);
	tclEval = Tcl_Eval(interp, "errMsg .loginDlg");
	Tcl_SetVar(interp, "errormsg", "", 0);

	return 0;
}
