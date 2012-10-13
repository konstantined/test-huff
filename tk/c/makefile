TARGET	=	soildialogTk

LIBS	=	-lACE -lrhtv -ltvfintl -lcontrols -lbllayer -lstdc++ -lsoilcommon -ltcl83 -lm -ltk83

SOURCES	=	dialogTk.cpp DialogAppTk.cpp DialogMsgTk.cpp stdafx.cpp functions.cpp

include $(SMARTOIL21_ROOT_DIR)/rules.mak

CPPFLAGS += -I$(SMARTOIL_ROOT)/reuse/ace \
	    -I$(SMARTOIL_ROOT)/reuse/rhtvision \
	    -I$(SMARTOIL_ROOT)/src/Shared \
	    -I$(SMARTOIL_ROOT)/src/UI/util  \
	    -I$(SMARTOIL_ROOT)/src/Bl \
	    -I$(SMARTOIL_ROOT)/reuse \
	    -I$(SMARTOIL_ROOT)/src/UI \
	    -DIBPP_LINUX -DIBPP_GCC \
	    -I/usr/local/include/tk8.3 \
	    -I/usr/local/include/tcl8.3 \
	    -I/usr/X11R6/include
