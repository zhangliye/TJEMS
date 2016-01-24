#pragma once
#include <QtCore>
#include <QtXml\QDomDocument>
#include <QtXml\QDomElement>
#include <QtXml\QDomNode>
#include <QtGui>
#include "trajectorypoint.h"
#include <iostream>
using namespace std;

#include "videoviewbase.h"

class ProjectFile
{
public:
	ProjectFile(); 
	~ProjectFile( void );

	// prjFilePath - project file pull path
	bool openPrjectFile( const QString &prjFilePath );

	// prjFileDir - project saving real directory; prjFileName - project directory
	bool createProject( const QString &prjFileDir, const QString &prjFileName, const QString &prjDescription, const QString &mPrjAuthor );
	
	bool parseProjectFile();

	bool saveProjectFile();

	void setStudyArea( const QPolygon &points );
	
	// get x, y from "34,45"
	int getX(const QString & row);
	int getY(const QString & row);

	// push one row ton data structure in main window
	void writeOneRow(int mTimeInstance, int mVehicleID, int vehicleType, int mXpos, int mYpos, int mXtext, int mYtext, const QString &mLabelPt);

	// add items to scene for showing
	void addItemsToView( VideoViewBase* view );

	// the the nearest point
	// x,y - position in scene coordinates; cur_time - current frame index
	QPair<TrajectoryPoint*, QGraphicsTextItem*> nearestPoint( float x, float y, int cur_time);

private:
	QString mTemplateContent;

public:
	QString mPrjPath;
	QString mPrjDir;

	// project XML contents
	//
	QString mPrjName;
	QString mPrjDescrition;
	QString mPrjAuthor;

	// XML video data
	//
	QString mVideoPath; 
	QString mCurrentFrameIndex; 
	QString mEntryStudyArea;
	vector<QPoint> mStudyAreaPoints;
	
	QDomDocument mDoc;  // XML contents

	// trajectory  data
	//
	QMap<int, QMap<int, QPair<TrajectoryPoint*, QGraphicsTextItem*>> > mTrajectory;    //{FrameIndex: {VehilcleID:QPoint, ...}}
};

