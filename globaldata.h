#ifndef GLOBALDATA_H
#define GLOBALDATA_H

#include <QtCore>
#include <iostream>
using namespace std;

class MainWindow;

class globaldata
{
public:
	globaldata();
	static globaldata *getInstance();
public:
	//#gui data
	QString G_WINDOW_TITLE; 

	//# software pointers
	MainWindow *g_mainwind;

	QString g_base_dir;

	QObject *g_app;	

	static globaldata *instance;
};

#endif // GLOBALDATA_H
