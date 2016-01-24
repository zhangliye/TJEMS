#include "globaldata.h"

globaldata* globaldata::instance = 0;
globaldata* globaldata::getInstance() {
	if(!instance) {
		instance = new globaldata();
		cout << "getInstance(): First instance\n";
		return instance;
	}
	else {
		cout << "getInstance(): previous instance\n";
		return instance;
	}
}

globaldata::globaldata()
{
	G_WINDOW_TITLE = "TJEMS - Bicycle ( v1.1) 2013-2015, ZHANG Liye";
	g_mainwind = NULL;
	g_base_dir = "";
	g_app = NULL;
}
