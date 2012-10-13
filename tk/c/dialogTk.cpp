#include "stdafx.h"

#include <string>
#include <cstring>
#include <sstream>

#include <bllayer/Waybill.h>
#include <bllayer/WaybillItem.h>
#include <bllayer/Goods.h>
#include <bllayer/Unit.h>
#include <bllayer/Contragent.h>

#include "DialogAppTk.h"
#include "DialogMsgTk.h"

#include "functions.h"
/////////////////////////////
#define createTclFunc(funcName) Tcl_CreateCommand(interp, (#funcName), (funcName), (ClientData*) NULL, (Tcl_CmdDeleteProc*) NULL)
/////////////////////////////
Shift* shiftPtr;
FRSession *frSessionPtr;

bool loginIsAdmin_;
bool shiftIsOpened_;
int shiftIsOpened;

char* login;
char* passw;

Tcl_Interp *interp;
int tclEval;

/////////////////////////////
int testLogin ()
{
	login = "SYSDBA";
	passw = "masterkey";
	std::cout << "\nLogin: \"" << login << "\" ,  Password: \"" << passw << "\"\n\n";
	try
	{
		BLDBSession::instance()->login(login, passw);
		loginIsAdmin_ = BLDBSession::instance()->isAdmin();
		if( Shift::isOpened() )
		{
			shiftPtr = &(Shift::getOpened());
			frSessionPtr = &(shiftPtr->getFRSession());
			shiftIsOpened_ = frSessionPtr->isOpened();
		}
		else
		{
			shiftIsOpened_ = false;
		}
		std::cout << "\nLogin OK\n\n";
	}
	catch(IBPP::Exception&)
	{
		std::string errormsg("Ошибка подключения к базе!");
		std::cout << errormsg << "\n\n";
	}
	return 0;
}
int getAllContragents()
{
	try
	{
		int rowId = 1;
		std::auto_ptr<ResultSet> rs = Contragent::getAllContragents();
		while(rs->next())
		{
//			std::cout << "Contragent " << rowId << "\n";
			rowId++;
		}
		std::cout << "\n getContragents \n----------\n";
	}
	catch(std::runtime_error e)
	{
		std::cout << e.what() << "\naaaa TEST aaaaa\n\n";
	}
	return 0;
}
/////////////////////////////

int main(int argc, char *argv[])
{
	SoilCommon::CmdLine cmd_line(argc, argv, "Dialog", "T. Chigarkova");
	try
	{
		cmd_line.setParam('i', "Module instance number", false, "instance");
		cmd_line.setParam('c', "Configuration file", false, "filename");
		cmd_line.process(true);

	}
	catch (SoilCommon::CommandLineException e)
	{

		std::cerr << "Error in command line: " << e.getCause() << std::endl;
		cmd_line.usage();
		return -1;
	}
	TDialogAppTk app;
	app.init(&cmd_line);
//	testLogin();
//	getAllContragents();
	Tk_Main(argc, argv, Tcl_AppInit);
	return 0;
}

/////////////////////////////
int errorMsgBox(Tcl_Interp *interp, std::string errStr)
{
	TDialogMsgTk msg;
	std::string message(errStr);
	msg.tkMsgBox(interp, message);
	return 0;
}
/////////////////////////////
int exitDialog(ClientData clientData, Tcl_Interp* interp, int argc, char *argv[])
{
	Tcl_DeleteInterp(interp);
	std::cout << "\nExit Dialog !!!\n\n";
	exit (0);
}
/////////////////////////////
int getTrksInfo(ClientData clientData, Tcl_Interp* interp, int argc, char *argv[])
{
	try
	{
		std::auto_ptr<ResultSet> rs = Pistol::getByFuel(95);
		rs->refresh();
		int rowId = 2;
		while(rs->next())
		{
			char CODE[6];
			char CUR_SHIFT_TIK[20];
			char ACTUAL_SHIFT_TIK[20];
			char CUR_SHIFT_QTY[20];
			char ACTUAL_SHIFT_QTY[20];
			strcpy(CODE, rs->getField("CODE").getData<TextData>());
			strcpy(CUR_SHIFT_TIK, long2char(rs->getField("CUR_SHIFT_TIK").getData<LongData>()));
			strcpy(ACTUAL_SHIFT_TIK, long2char(rs->getField("ACTUAL_SHIFT_TIK").getData<LongData>()));
			strcpy(CUR_SHIFT_QTY, long2char(rs->getField("CUR_SHIFT_QTY").getData<LongData>()));
			strcpy(ACTUAL_SHIFT_QTY, long2char(rs->getField("ACTUAL_SHIFT_QTY").getData<LongData>()));

			Tcl_SetVar(interp, "trk_code", CODE, 0);
			Tcl_SetVar(interp, "cur_shift_tik", CUR_SHIFT_TIK, 0);
			Tcl_SetVar(interp, "actual_shift_tik", ACTUAL_SHIFT_TIK, 0);
			Tcl_SetVar(interp, "cur_shift_qty", CUR_SHIFT_QTY, 0);
			Tcl_SetVar(interp, "actual_shift_qty", ACTUAL_SHIFT_QTY, 0);

			Tcl_SetVar(interp, "rowId", long2char(rowId), 0);
			Tcl_Eval(interp, "set trksTable($rowId,0) $trk_code");
			Tcl_Eval(interp, "set trksTable($rowId,1) $cur_shift_tik");
			Tcl_Eval(interp, "set trksTable($rowId,2) $actual_shift_tik");
			Tcl_Eval(interp, "set trksTable($rowId,3) $cur_shift_qty");
			Tcl_Eval(interp, "set trksTable($rowId,4) $actual_shift_qty");

			rowId++;
		}
	}
	catch(IBPP::Exception&)
	{
		std::string DBerr = BLDBSession::instance()->getError();
		std::cout << DBerr << "\n\n";
	}
	return 0;
}
/////////////////////////////
int getTanksInfo(ClientData clientData, Tcl_Interp* interp, int argc, char *argv[])
{
	try
	{
		std::auto_ptr<ResultSet> rs = Tank::getByFuel(95);
		rs->refresh();
		int rowId = 2;
		Tcl_SetVar(interp, "command", "set goods_code [encoding convertfrom cp1251 $goods_code]", 0);
		while(rs->next())
		{
			char CODE[5];
			char GOODS_CODE[10];
			char CUR_QTY[20];
			char ACTUAL_QTY[20];
			char SHORTAGE[20];
			char FUEL_HEIGHT[10];
			char WATER_HEIGHT[10];
			strcpy(CODE, rs->getField("CODE").getData<TextData>());
			strcpy(GOODS_CODE, rs->getField("GOODS_CODE").getData<TextData>());
			strcpy(CUR_QTY, long2char(rs->getField("CUR_QTY").getData<LongData>()));
			strcpy(ACTUAL_QTY, long2char(rs->getField("ACTUAL_QTY").getData<LongData>()));
			strcpy(SHORTAGE, long2char(rs->getField("SHORTAGE").getData<LongData>()));
			strcpy(FUEL_HEIGHT, long2char(rs->getField("FUEL_HEIGHT").getData<LongData>()));
			strcpy(WATER_HEIGHT, long2char(rs->getField("WATER_HEIGHT").getData<LongData>()));

			Tcl_SetVar(interp, "tank_code", CODE, 0);
			Tcl_SetVar(interp, "goods_code", GOODS_CODE, 0);
			Tcl_SetVar(interp, "cur_qty", CUR_QTY, 0);
			Tcl_SetVar(interp, "actual_qty", ACTUAL_QTY, 0);
			Tcl_SetVar(interp, "shortage", SHORTAGE, 0);
			Tcl_SetVar(interp, "fuel_height", FUEL_HEIGHT, 0);
			Tcl_SetVar(interp, "water_height", WATER_HEIGHT, 0);

			Tcl_SetVar(interp, "rowId", long2char(rowId), 0);
			Tcl_Eval(interp, "set tanksTable($rowId,0) $tank_code");
			Tcl_Eval(interp, "eval $command");
			Tcl_Eval(interp, "set tanksTable($rowId,1) $goods_code");
			Tcl_Eval(interp, "set tanksTable($rowId,2) $cur_qty");
			Tcl_Eval(interp, "set tanksTable($rowId,3) $actual_qty");
			Tcl_Eval(interp, "set tanksTable($rowId,4) $shortage");
			Tcl_Eval(interp, "set tanksTable($rowId,5) $fuel_height");
			Tcl_Eval(interp, "set tanksTable($rowId,6) $water_height");

			rowId++;

		}
	}
	catch(IBPP::Exception&)
	{
		std::string DBerr = BLDBSession::instance()->getError();
		std::cout << DBerr << "\n\n";
	}
	return 0;
}
/////////////////////////////
int getGoodsUnits(ClientData clientData, Tcl_Interp* interp, int argc, char *argv[])
{
	try
	{
		std::auto_ptr<ResultSet> rs = Goods::getAllGoods();
		rs->refresh();
		char GOODS_NAME[110];
		char UNIT_NAME[10];
		Tcl_SetVar(interp, "command", "set tmpVar [encoding convertfrom cp1251 $tmpVar]", 0);
		while(rs->next())
		{
			strcpy(GOODS_NAME, rs->getField("NAME").getData<TextData>());
			strcpy(UNIT_NAME, rs->getField("UNIT_NAME").getData<TextData>());
			Tcl_SetVar(interp, "tmpVar", GOODS_NAME, 0);
			Tcl_Eval(interp, "eval $command");
			Tcl_Eval(interp, "set name $tmpVar");
			Tcl_SetVar(interp, "tmpVar", UNIT_NAME, 0);
			Tcl_Eval(interp, "eval $command");
			Tcl_Eval(interp, "set unit $tmpVar");
			Tcl_Eval(interp, "set goodsUnits($name) $unit");
		}
	}
	catch(IBPP::Exception&)
	{
		std::string DBerr = BLDBSession::instance()->getError();
		char* errorMsg = const_cast<char*>(DBerr.c_str());
		Tcl_SetVar(interp, "errorMsg", errorMsg, 0);
		Tcl_Eval(interp, "errMsgBox .");
//		std::cout << DBerr << "\n\n";
	}
	return 0;
}
/////////////////////////////
int getGoodsList(ClientData clientData, Tcl_Interp* interp, int argc, char *argv[])
{
	try
	{
		std::auto_ptr<ResultSet> rs = Goods::getAllGoods();
		rs->refresh();
		int rowId = 1;
		char GOODS_ID[10];
		char GOODS_NAME[110];
		char UNIT_NAME[10];
		Tcl_SetVar(interp, "command", "set tmpVar [encoding convertfrom cp1251 $tmpVar]", 0);
		while(rs->next())
		{
			strcpy(GOODS_ID, long2char(rs->getField("ID").getData<LongData>()));
			strcpy(GOODS_NAME, rs->getField("NAME").getData<TextData>());
			strcpy(UNIT_NAME, rs->getField("UNIT_NAME").getData<TextData>());

			Tcl_SetVar(interp, "rowId", long2char(rowId), 0);
			Tcl_SetVar(interp, "tmpVar", GOODS_NAME, 0);
			Tcl_Eval(interp, "eval $command");
			Tcl_Eval(interp, "set goodsTable($rowId,0) $tmpVar");
			Tcl_SetVar(interp, "tmpVar", UNIT_NAME, 0);
			Tcl_Eval(interp, "eval $command");
			Tcl_Eval(interp, "set goodsTable($rowId,1) $tmpVar");
			Tcl_SetVar(interp, "tmpVar", GOODS_ID, 0);
			Tcl_Eval(interp, "set goodsTable($rowId,2) $tmpVar");
			rowId++;
		}
	}
	catch(IBPP::Exception&)
	{
		std::string DBerr = BLDBSession::instance()->getError();
		char* errorMsg = const_cast<char*>(DBerr.c_str());
		Tcl_SetVar(interp, "errorMsg", errorMsg, 0);
		Tcl_Eval(interp, "errMsgBox .");
//		std::cout << DBerr << "\n\n";
	}
	return 0;
}
/////////////////////////////
// getContragents
int getContragents(ClientData clientData, Tcl_Interp* interp, int argc, char *argv[])
{
	try
	{
		int rowId = 1;
		std::auto_ptr<ResultSet> rs = Contragent::getAllContragents();
//		while(rs->next())
//		{
//			std::cout << "Contragent " << rowId << "\n";
//			rowId++;
//		}
		std::cout << "\n getContragents \n----------\n";
	}
	catch(std::runtime_error e)
	{
		std::cout << e.what() << "\naaaaaaaaaa\n\n";
	}
	return 0;
}
/////////////////////////////
// init WayBillDocument
int createWaybillDoc(ClientData clientData, Tcl_Interp* interp, int argc, char *argv[])
{
	char* docNum = argv[1];
	char* docDate = argv[2];
	char* car = argv[3];
	char* driver = argv[4];
	char* docComment = argv[5];
//	Waybill doc = Waybill::create(contr,docNo,date,car,driver);
//rsAll_ = Contragent::getAllContragents();
	std::cout << "\n initWayBillDoc \n----------\n";
	return 0;
}
/////////////////////////////
// add WayBillItem
int setWayBillItem(ClientData clientData, Tcl_Interp* interp, int argc, char *argv[])
{
	long goodsId = atol(argv[1]);
	char* itemQuantDoc = argv[2];
	char* itemQuantFac = argv[3];
	char* itemPrice = argv[4];

	Goods goods = Goods::get(goodsId);
	Unit unit = goods.getUnit();

//	WaybillItem item = WaybillItem::create(doc_, goods, unit, itemQuantDoc, itemQuantFac, itemPrice);
//	doc_.addItem(item);
	
	std::cout << "\nsetWayBillItem:"
			 << "\ngoodsId = " << goodsId
			 << "\nitemUnit = " << unit.getShortName()
			 << "\nitemQuantDoc = " << itemQuantDoc
			 << "\nitemQuantFac = " << itemQuantFac
			 << "\nitemPrice = " << itemPrice << "\n----------\n";
	return 0;
}
/////////////////////////////
int setGoodsItemData(ClientData clientData, Tcl_Interp* interp, int argc, char *argv[])
{
	char* goodsName = argv[1];
	char* goodsUnit = argv[2];
	std::cout << "\nsetGoodsItemData:"
			 << "\ngoodsName = " << goodsName
			 << "\ngoodsUnit = " << goodsUnit << "\n----------\n";
	return 0;
}
/////////////////////////////
int showPetrolInfo()
{
	std::cout << "showPetrolInfo\n\n";
	return 0;
}
/////////////////////////////
int showInputMoney(Tcl_Interp* interp)
{
	char* money = "123.45";
	char* unit = "руб";
	std::cout << "showInputMoney: " << money << " " << unit << "\n\n";
	Tcl_SetVar(interp, "inputMoney", money, 0);
	Tcl_SetVar(interp, "unitMoney", unit, 0);
	tclEval = Tcl_Eval(interp, "showInputMoney");
	return 0;
}
/////////////////////////////
int setShiftStatus(ClientData clientData, Tcl_Interp* interp, int argc, char *argv[])
{
	try
	{
		if( shiftIsOpened_ )
		{
			Shift shift = Shift::getOpened();
			FRSession frSession = shift.getFRSession();
			frSession.close();
			if( loginIsAdmin_ ) 
			{
				Tcl_Eval(interp, "showTanksInfo");
				Tcl_Eval(interp, "showTrksInfo");
				Shift::close();
			}	
			shiftIsOpened_ = false;
			shiftIsOpened = 0;
		}
		else
		{
			showInputMoney(interp);
			if( loginIsAdmin_ )
			{
				Shift shift = Shift::open();
			}
			Shift shift = Shift::getOpened();
			FRSession frSession = shift.getFRSession();
			frSession.open(login, passw);
			shiftIsOpened_ = true;
			shiftIsOpened = 1;
		}
		tclEval = Tcl_Eval(interp, "showShiftStatus");
	}
	catch(IBPP::Exception&)
	{
		std::string DBerr = BLDBSession::instance()->getError();
		std::cout << DBerr << "\n\n";
	}
	return 0;
}
/////////////////////////////
int getTankInfo(ClientData clientData, Tcl_Interp* interp, int argc, char *argv[])
{
	long int tankId;
	__int64 allQtyPetrolData_;
	__int64 allQtyFactPetrolData_;
	Tcl_LinkVar(interp, "tankId", (char*) &tankId, TCL_LINK_INT);
	Tcl_LinkVar(interp, "allQtyPetrolData", (char*) &allQtyPetrolData_, TCL_LINK_INT);
	Tcl_LinkVar(interp, "allQtyFactPetrolData", (char*) &allQtyFactPetrolData_, TCL_LINK_INT);
	try
	{
		Tank::getTotalByFuel(tankId, allQtyPetrolData_, allQtyFactPetrolData_);
		tclEval = Tcl_Eval(interp, "showTankInfo");
	}
	catch(IBPP::Exception&)
	{
		std::string DBerr = BLDBSession::instance()->getError();
		errorMsgBox(interp, DBerr);
	}
	return 0;
}
/////////////////////////////
int checkLogin (ClientData clientData, Tcl_Interp* interp, int argc, char *argv[])
{
	login = argv[1];
	passw = argv[2];
	std::cout << "\nLogin: \"" << login << "\" ,  Password: \"" << passw << "\"\n\n";
	try
	{
		BLDBSession::instance()->login(login, passw);
		loginIsAdmin_ = BLDBSession::instance()->isAdmin();
		if( Shift::isOpened() )
		{
			shiftPtr = &(Shift::getOpened());
			frSessionPtr = &(shiftPtr->getFRSession());
			shiftIsOpened_ = frSessionPtr->isOpened();
			shiftIsOpened = 1;
		}
		else
		{
			shiftIsOpened_ = false;
			shiftIsOpened = 0;
		}
		Tcl_SetVar(interp, "passw", "", 0);
		Tcl_SetVar(interp, "sessionLogin", login, 0);
		Tcl_SetVar(interp, "sessionPassw", passw, 0);
		tclEval = Tcl_Eval(interp, "destroy .loginDlg; activateMenu");
		std::cout << "\nLogin OK!!!!\n\n";
		tclEval = Tcl_Eval(interp, "showShiftStatus");
	}
	catch(IBPP::Exception&)
	{
		std::string errormsg("Ошибка подключения к базе!");
		errorMsgBox(interp, errormsg);
	}
	if(loginIsAdmin_)
	{
		std::cout << "Is Admin\n\n";
	}
	else
	{
		std::cout << "Is Not Admin\n\n";
	}
	Tcl_SetVar(interp, "passw", "", 0);
	return 0;
}
/////////////////////////////
int testFunc(ClientData clientData, Tcl_Interp* interp, int argc, char *argv[])
{
	std::cout << "\n====== Test Function ======\n\n";
	return 0;
}
/////////////////////////////

int Tcl_AppInit(Tcl_Interp *interp) {
	if (Tcl_Init(interp) == TCL_ERROR) {
		return TCL_ERROR;
	}
	if (Tk_Init(interp) == TCL_ERROR) {
		return TCL_ERROR;
	}

	Tcl_LinkVar(interp, "shiftIsOpened", (char*) &shiftIsOpened, TCL_LINK_INT);

	Tcl_CreateCommand(interp, "checkLogin", checkLogin, (ClientData*) NULL, (Tcl_CmdDeleteProc*) NULL);
	Tcl_CreateCommand(interp, "exitDialog", exitDialog, (ClientData*) NULL, (Tcl_CmdDeleteProc*) NULL);
	Tcl_CreateCommand(interp, "getTankInfo", getTankInfo, (ClientData*) NULL, (Tcl_CmdDeleteProc*) NULL);
	Tcl_CreateCommand(interp, "setShiftStatus", setShiftStatus, (ClientData*) NULL, (Tcl_CmdDeleteProc*) NULL);
	Tcl_CreateCommand(interp, "getTanksInfo", getTanksInfo, (ClientData*) NULL, (Tcl_CmdDeleteProc*) NULL);
	Tcl_CreateCommand(interp, "getTrksInfo", getTrksInfo, (ClientData*) NULL, (Tcl_CmdDeleteProc*) NULL);
	Tcl_CreateCommand(interp, "getGoodsList", getGoodsList, (ClientData*) NULL, (Tcl_CmdDeleteProc*) NULL);
	Tcl_CreateCommand(interp, "setGoodsItemData", setGoodsItemData, (ClientData*) NULL, (Tcl_CmdDeleteProc*) NULL);
	Tcl_CreateCommand(interp, "setWayBillItem", setWayBillItem, (ClientData*) NULL, (Tcl_CmdDeleteProc*) NULL);
	Tcl_CreateCommand(interp, "createWaybillDoc", createWaybillDoc, (ClientData*) NULL, (Tcl_CmdDeleteProc*) NULL);
	Tcl_CreateCommand(interp, "getContragents", getContragents, (ClientData*) NULL, (Tcl_CmdDeleteProc*) NULL);
//	Tcl_CreateCommand(interp, "getGoodsUnits", getGoodsUnits, (ClientData*) NULL, (Tcl_CmdDeleteProc*) NULL);
	createTclFunc(getGoodsUnits);
	createTclFunc(testFunc);
//	Tcl_CreateCommand(interp, "testFunc", testFunc, (ClientData*) NULL, (Tcl_CmdDeleteProc*) NULL);

	Tcl_EvalFile(interp, "Tcl/dialog.tcl");

	Tcl_SetVar(interp, "login", "SYSDBA", 0);
	Tcl_SetVar(interp, "passw", "masterkey", 0);
	tclEval = Tcl_Eval(interp, "showLoginDlg");

	return TCL_OK;
}
