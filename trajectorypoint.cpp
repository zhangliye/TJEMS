#include "trajectorypoint.h"

TrajectoryPoint::TrajectoryPoint( int bikeId, int bikeType )
{
	mBikeID = bikeId;
	mBikeType = bikeType;
}

TrajectoryPoint::~TrajectoryPoint( void )
{
}

void TrajectoryPoint::setPosition( float x, float y )
{
	setRect(x, y, 1, 1);
}

void TrajectoryPoint::bikeID( int id )
{
	mBikeID= id;


}
