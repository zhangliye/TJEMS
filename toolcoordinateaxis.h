#ifndef TOOLCOORDINATEAXIS_H
#define TOOLCOORDINATEAXIS_H

#include "toolimage.h"
#include "videoviewbase.h"

class ToolCoordinateAxis : public ToolImage
{
	Q_OBJECT

public:
	ToolCoordinateAxis(VideoViewBase *parent);
	~ToolCoordinateAxis();
	void clearTempPointLine();

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


#endif // TOOLCOORDINATEAXIS_H
