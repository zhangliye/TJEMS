#ifndef TOOLCONTROLPOINT_H
#define TOOLCONTROLPOINT_H

#include "toolimage.h"
#include "videoviewbase.h"
#include "trajectorypoint.h"

class ToolControlPoint : public ToolImage
{
	Q_OBJECT

public:
	ToolControlPoint(VideoViewBase *parent);
	~ToolControlPoint();

public:
	QVector<QPointF> mBoundaryPts;       //#[(x1, y1), ...]  points of detector boundary

	// temporal drawing
	//
	QPoint mPrePoint;

	// bicycle loop detector area  	
	QList<QGraphicsEllipseItem*> mPointItems;
	QList<QGraphicsLineItem*> mLineItems;

	public slots:
		void mousePress( QMouseEvent *mouseEvent);
		void keyPress( QKeyEvent *keyEvent);	
};

#endif // TOOLCONTROLPOINT_H
