#ifndef TOOLEDITCONTROLPOINT_H
#define TOOLEDITCONTROLPOINT_H

#include "toolimage.h"
#include "videoviewbase.h"

class ToolEditControlPoint : public ToolImage
{
	Q_OBJECT

public:
	ToolEditControlPoint(VideoViewBase *parent);
	~ToolEditControlPoint();

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


#endif // TOOLEDITCONTROLPOINT_H
