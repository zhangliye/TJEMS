#ifndef NEWPROJECTDLG_H
#define NEWPROJECTDLG_H

#include <QObject>
#include "ui_newprojectdlg.h"

class NewProjectDlg : public QDialog, public Ui::NewProjectDlg
{
	Q_OBJECT

public:
	NewProjectDlg(QWidget *parent);
	~NewProjectDlg();
	
	//////////////////////////////////////////////////////////////////////////
	// access properties
public slots:
	void actSelectDir();	

private:
	int mStep;  // forward step, defalt is 1
};

#endif // NEWPROJECTDLG_H