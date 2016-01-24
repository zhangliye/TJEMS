#ifndef TOOLSEMITRACKING_H
#define TOOLSEMITRACKING_H

#include "toolimage.h"
#include "videoviewbase.h"
#include "trajectorypoint.h"

class ToolSemiTracking : public ToolImage
{
	Q_OBJECT

public:
	ToolSemiTracking( VideoViewBase *parent );
	~ToolSemiTracking();

public:
	QVector<QPointF> mBoundaryPts;       //#[(x1, y1), ...]  points of detector boundary

	// temporal drawing
	// 
	QPoint mPrePoint;

	// bicycle loop detector area  	
	QList< TrajectoryPoint* > mPointItems;
	QList< QGraphicsLineItem* > mLineItems;

public slots:
	void mouseMove( QMouseEvent *mouseEvent);
	void mousePress( QMouseEvent *mouseEvent);
	void keyPress( QKeyEvent *keyEvent);	
	void actNextID();

public:
	// trajectory
	//
	int mCurrentVehicleID;  // vehicle ID maximum

private:
	Mat mMixedImage;   // bottom image mixed with background image to show
	Mat mSubImage;     // real image by selecting using mouse click

	int mEnlargeRate;  // enlarge rate to show in right window
	int mSubWndWidth;  // sub window width/2		
};


#endif // TOOLSEMITRACKING_H
