#ifndef TOOLMOVEMARK_H
#define TOOLMOVEMARK_H

#include "toolimage.h"
#include "videoviewbase.h"
#include "trajectorypoint.h"

class ToolMoveTrackMark : public ToolImage
{
	Q_OBJECT

public:
	ToolMoveTrackMark(VideoViewBase *parent);
	~ToolMoveTrackMark();

public:
	QVector<QPointF> mBoundaryPts;       //#[(x1, y1), ...]  points of detector boundary

	// temporal drawing
	// 
	QPoint mPrePoint;

	// bicycle loop detector area  	
	QList<TrajectoryPoint*> mPointItems;
	QList<QGraphicsLineItem*> mLineItems;

	public slots:
		void mouseMove( QMouseEvent *mouseEvent);
		void mousePress( QMouseEvent *mouseEvent);
		void keyPress( QKeyEvent *keyEvent);			
};


#endif // TOOLSEMITRACKING_H