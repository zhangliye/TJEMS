#pragma once
#include "toolimage.h"
#include "videoviewbase.h"
#include "trajectorypoint.h"
#include "trajedtdlg.h"

class TrajEdtDlg;
class ToolEditTrackMark : public ToolImage
{
	Q_OBJECT

public:
	ToolEditTrackMark(VideoViewBase *parent);
	~ToolEditTrackMark();

public:	
	VideoViewBase* mBaseView;
	QPair<TrajectoryPoint*, QGraphicsTextItem*> mNearPt;  // selected point by mouse
	TrajEdtDlg *mTrajEdtDlg;    // trajectory editor 

	public slots:
		void mouseMove( QMouseEvent *mouseEvent);
		void mousePress( QMouseEvent *mouseEvent);
		void keyPress( QKeyEvent *keyEvent);			
};

