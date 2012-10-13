
#ifndef DIALOGAPPTK_H
#define DIALOGAPPTK_H

#include <common/cmdline.h>
#include <bllayer/BLLayer.h>
#include <bllayer/Tank.h>
#include <bllayer/Pistol.h>
#include <bllayer/FuelDistribution.h>

#include <tcl.h>
#include <tk.h>

/////////////////////////////////////////////////////////////////////////////

class TDialogAppTk
{
public:
	TDialogAppTk();
	virtual ~TDialogAppTk();
	void init(SoilCommon::CmdLine* cmdLine);
private:
	std::string username_;
	std::string password_;
};

#endif
