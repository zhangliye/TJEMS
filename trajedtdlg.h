#pragma once
#include <QObject>
#include "ui_trajedtdlg.h"
#include "tooledittrackmark.h"

class ToolEditTrackMark;
class TrajEdtDlg : public QDialog, public Ui::TrajEdtDlg
{
	Q_OBJECT

public:
	TrajEdtDlg( QWidget *parent, ToolEditTrackMark *editTool );
	~TrajEdtDlg(void);
	
	void setBikeType( int bikeType );

	int getBikeType();

public slots:
	void actSave();
	void actCancel();

public:
	ToolEditTrackMark *mTrajEditTool;
};

