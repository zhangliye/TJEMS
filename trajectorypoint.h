#pragma once
#include <QtCore>
#include <QtGui>

class TrajectoryPoint : public QGraphicsEllipseItem
{
public:
	TrajectoryPoint( int bikeId, int bikeType = 1 );
	~TrajectoryPoint(void);

	void setPosition( float x, float y );

	QPointF getPosition(){ return QPointF(); }

	void moveDleta( float deltaX, float deltaY) {}

	// selected - yello; cleared - red
	void setSelected(){ setPen( QPen(Qt::yellow) ); }
	void clearSelected() { setPen( QPen(Qt::red) ); }

	// attributes access API
public:
	void bikeID(int id);
	int bikeID() { return( mBikeID ); }

	void bikeType(int bikeType){ mBikeType= bikeType; }
	int bikeType() { return( mBikeType ); }

private:
	int mBikeID;
	int mBikeType;   // 1 - traditional bike; 2 - electrical bike
};

