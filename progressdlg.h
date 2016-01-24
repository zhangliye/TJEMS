#ifndef PROGRESSDLG_H
#define PROGRESSDLG_H

#include <QObject>
#include "ui_progressdlg.h"

class ProgressDlg : public QDialog, public Ui::ProgressDlg
{
	Q_OBJECT

public:
	ProgressDlg(QWidget *parent);
	~ProgressDlg();

	void setRange(int start, int end);

	void next();

//////////////////////////////////////////////////////////////////////////
// access properties
public:
	int step() { return mStep; }
	void step( int step ) { mStep=step; }

private:
	int mStep;  // forward step, defalt is 1
};

#endif // PROGRESSDLG_H
