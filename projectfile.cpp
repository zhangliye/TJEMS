#include "projectfile.h"
#include "globaldata.h"
#include "mainwindow.h"

ProjectFile::ProjectFile()
{		
}

ProjectFile::~ProjectFile(void)
{
}

bool ProjectFile::createProject( const QString &prjFileDir, const QString &prjFileName, const QString &prjDescription, const QString &mPrjAuthor )
{
	QString prjFilePath = prjFileDir + "/" + prjFileName + ".prj";	
	mPrjPath = prjFilePath;	

	// if no project dir, create it
	QDir prjDir( prjFileDir );
	if( !prjDir.exists() )
		prjDir.mkdir( prjFileDir );

	// create empty project file
	//
	QFile file( prjFilePath );
	if ( !file.open(QIODevice::WriteOnly | QIODevice::Text) ) 
		return false;

	QString content( "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n" );
	content += "<body> \n";
	content += "\n";

	content += "<!--#################### section 1: project information ##################################-->" + QString("\n");
	content += "<project_name>" + prjFileName + "</project_name>" + "\n";
	content += "<project_description>" + prjDescription + "</project_description>" + "\n";
	content += "<project_author>" + mPrjAuthor + "</project_author>" + "\n";

	content += "\n<!--#################### section 2: video file data ##################################-->" + QString("\n");
	content += QString("<video_path>") + "</video_path>" + "\n";
	content += QString("<current_frame_index>") + "</current_frame_index>" + "\n";
	content += QString("<entry_study_area>") + "</entry_study_area>" + "\n";

	content += "\n<!--#################### section 3: bicycle position data file ##################################-->" + QString("\n");
	content += QString("<entry_trajectory>") + "</entry_trajectory>" + "\n";

	content += "</body>\n";

	// save data
	//
	QTextStream stream( &file );
	stream<<content <<endl;

	file.close();

	// open the new project file
	//
	openPrjectFile(prjFilePath);

	QMessageBox::information(NULL, "Project file created", "Project file created successfully!" );
	return true;
}

bool ProjectFile::openPrjectFile( const QString &prjFilePath )
{
	if (QFile::exists( prjFilePath )){   //open exist file
		// open file
		//
		mPrjPath = prjFilePath;
		QFile prjFile( prjFilePath );
		if (!prjFile.open(QFile::ReadOnly | QFile::Text)) {
			QMessageBox::information(NULL, "File error", "project file do not exist" );
			return false;
		}

		// set file content to QDocument
		//
		try{
			QString errorStr;
			int errorLine;
			int errorColumn;
			if (!mDoc.setContent(&prjFile, false, &errorStr, &errorLine,
				&errorColumn)) {
					QMessageBox::information(NULL, "File format error", "project file format error" );
					return false;
			}

			// parse XML file
			parseProjectFile();  
		} catch(...) {
			QMessageBox::information(NULL, "File format error", "project file format error" );
			prjFile.close();
		}
	}else{   // no project file found
		QMessageBox::information(NULL, "File not exist", "select another file." );
		return false;
	}
	
	return true;
}

bool ProjectFile::parseProjectFile()
{
	QDomElement root = mDoc.documentElement();
	if (root.tagName() != "body")
		return false;
	
	// project name, description and author
	//
	QDomElement &project_name  = root.firstChildElement("project_name");
	if ( !project_name.isNull() ){
		mPrjName = project_name.text();
	}else{
		mPrjName = "";
	} 

	QDomElement &project_description  = root.firstChildElement("project_description");
	if ( !project_description.isNull() ){
		mPrjDescrition = project_description.text();
	}else{
		mPrjDescrition = "";
	}	

	QDomElement &project_author  = root.firstChildElement("project_author");
	if ( !project_author.isNull() ){
		mPrjAuthor = project_author.text();
	}else{
		mPrjAuthor = "";
	}	

	// XML video data
	//
	QDomElement &video_path  = root.firstChildElement("video_path");
	if ( !video_path.isNull() ){
		mVideoPath = video_path.text();
	}else{
		mVideoPath = "";
	}	

	QDomElement &current_frame_index  = root.firstChildElement("current_frame_index");
	if ( !current_frame_index.isNull() ){
		mCurrentFrameIndex = current_frame_index.text();
	}else{
		mCurrentFrameIndex = "";
	}

	// read study area points
	//
	QDomElement &entry_study_area  = root.firstChildElement("entry_study_area");

	// read first point
	QDomElement point = entry_study_area.firstChildElement("point");
	QString strPt = point.text();
	int x = getX( strPt );
	int y = getY( strPt );
	if ( x!=-1 && y!=-1 )
		mStudyAreaPoints.push_back( QPoint(x, y) );	

	// read left points
	//
	while (true)
	{
		point = point.nextSiblingElement();
		if (point.isNull())
		{
			break;
		} else {
			strPt = point.text();
			x = getX( strPt );
			y = getY( strPt );
			if ( x!=-1 && y!=-1 )
				mStudyAreaPoints.push_back( QPoint(x, y) );
		}
	}

	// XML trajectory data
	//
	QDomElement &entry_trajectory  = root.firstChildElement("entry_trajectory");
	QDomElement &entry_trj_row = entry_trajectory.firstChildElement("entry_trj_row");
	while (true)
	{
		if ( entry_trj_row.isNull() ){
			break;
		}else{
			//  read points
			//
			QDomElement &time_obj = entry_trj_row.firstChildElement("time");
			QDomElement &vehicle_id = entry_trj_row.firstChildElement("vehicle_id");
			QDomElement &bike_type = entry_trj_row.firstChildElement("vehicle_type");
			QDomElement &x_pos = entry_trj_row.firstChildElement("x_pos");
			QDomElement &y_pos = entry_trj_row.firstChildElement("y_pos");
			QDomElement &x_text = entry_trj_row.firstChildElement("x_text");
			QDomElement &y_text = entry_trj_row.firstChildElement("y_text");
			QDomElement &label_pt = entry_trj_row.firstChildElement("label_pt");

			int mTimeInstance = time_obj.text().toInt();
			int mVehicleID = vehicle_id.text().toInt();
			int mVehicleType = bike_type.text().toInt();			

			float mXpos1 = x_pos.text().toFloat() ;
			float mYpos1 = y_pos.text().toFloat() ;
			float mXtext1 = x_text.text().toFloat() ;
			float mYtext1 = y_text.text().toFloat() ;

			int mXpos = int( x_pos.text().toFloat() );
			int mYpos = int( y_pos.text().toFloat() );
			int mXtext = int( x_text.text().toFloat() );
			int mYtext = int( y_text.text().toFloat() );
			QString mLabelPt = label_pt.text();
			writeOneRow(mTimeInstance, mVehicleID, mVehicleType, mXpos, mYpos, mXtext, mYtext, mLabelPt);
		}

		entry_trj_row = entry_trj_row.nextSiblingElement();    // move to next row
	}

	return true;
}

bool ProjectFile::saveProjectFile()
{
	// if project file, remove it
	// 
	if ( QFile::exists(mPrjPath) )
	{
		QFile fileOld( mPrjPath );
		fileOld.remove( mPrjPath );
	}
	
	// create empty project file
	// 
	QFile file( mPrjPath );
	if ( !file.open(QIODevice::WriteOnly | QIODevice::Text) ) 
		return false;

	QString content( "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n" );
	content += "<body> \n";
	content += "\n";

	content += "<!--#################### section 1: project information ##################################-->" + QString("\n");
	content += "<project_name>" + mPrjName + "</project_name>" + "\n";
	content += "<project_description>" + mPrjDescrition + "</project_description>" + "\n";
	content += "<project_author>" + mPrjAuthor + "</project_author>" + "\n";

	content += "\n<!--#################### section 2: video file data ##################################-->" + QString("\n");
	content += QString("<video_path>") + mVideoPath + "</video_path>" + "\n";
	content += QString("<current_frame_index>") + mCurrentFrameIndex + "</current_frame_index>" + "\n";
	content += QString("<entry_study_area>") + "\n";

	// save study area points
	//
	int x;
	int y;
	QString row;
	for (std::vector<QPoint>::iterator it = mStudyAreaPoints.begin() ; it != mStudyAreaPoints.end(); ++it){
		x = (*it).x();
		y = (*it).y();
		row = QString("<point>%1,%2</point>")
			.arg(x).arg(y);
		content += row + + "\n";
	}
	content += QString("</entry_study_area>") + "\n";

	content += "\n<!--#################### section 3: bicycle position data file ##################################-->" + QString("\n");
	content += QString("<entry_trajectory>") + "\n";
	QMapIterator<int, QMap<int, QPair<TrajectoryPoint*, QGraphicsTextItem*>> > i(mTrajectory);
	QString strRow;
	while ( i.hasNext() ) {
		i.next();  // move next
		int time = i.key();
		QMap<int, QPair<TrajectoryPoint*, QGraphicsTextItem*>> ptsMap = i.value();

		QMapIterator<int, QPair<TrajectoryPoint*, QGraphicsTextItem*>> j( ptsMap );
		while ( j.hasNext() )
		{
			j.next();  // move next
						
			QPair<TrajectoryPoint*, QGraphicsTextItem*> pt_text = j.value();
			TrajectoryPoint* pt = pt_text.first;
			QGraphicsTextItem* text = pt_text.second;
			
			int veicleID = pt->bikeID();
			int veicleType = pt->bikeType();
			QRectF rect = pt->rect();
			float x1 = rect.x();
			float y1 = rect.y();
			float x2 = text->pos().x();
			float y2 = text->pos().y();
			QString label = text->toPlainText();

			// save one row
			//
			content += QString("<entry_trj_row>") + "\n";

			content += QString("<time>%1</time>").arg( time ) + "\n";
			content += QString("<vehicle_id>%1</vehicle_id>").arg( veicleID ) + "\n";
			content += QString("<vehicle_type>%1</vehicle_type>").arg( veicleType ) + "\n";
			content += QString("<x_pos>%1</x_pos>").arg(x1) + "\n";
			content += QString("<y_pos>%1</y_pos>").arg(y1) + "\n";
			content += QString("<x_text>%1</x_text>").arg(x2) + "\n";
			content += QString("<y_text>%1</y_text>").arg(y2) + "\n";
			content += QString("<label_pt>%1</label_pt>").arg( label ) + "\n";

			content += QString("</entry_trj_row>") + "\n" + "\n";
		}		
	}

	content += QString("</entry_trajectory>") + "\n";	

	content += "</body>\n";

	// save data
	//
	QTextStream stream( &file );
	stream<< content <<endl;
	file.close();
	
	return true;
}

void ProjectFile::setStudyArea( const QPolygon &points )
{
	QPointF pt;
	int x;
	int y;
	for (int i=0; i< points.size(); ++i)
	{
		pt = points.at(i);
		x = pt.x();
		y = pt.y();
		mStudyAreaPoints.push_back( QPoint(x, y) );	
	}
}

int ProjectFile::getX( const QString & row )
{
	QStringList pts = row.split(",");
	if ( pts.length() <= 1)
		return -1;

	int x = pts.at(0).toInt();
	return(x);
}

int ProjectFile::getY( const QString & row )
{
	QStringList pts = row.split(",");
	if ( pts.length() <= 1)
		return -1;

	int y = pts.at(1).toInt();
	return(y);
}

void ProjectFile::writeOneRow( int mTimeInstance, int mVehicleID, int vehicleType, int mXpos, int mYpos, int mXtext, int mYtext, const QString &mLabelPt )
{
	// create point item and show
	//
	TrajectoryPoint *pointItem = new TrajectoryPoint(mVehicleID, vehicleType);
	pointItem->setPosition(mXpos, mYpos);
	pointItem->setPen( QPen(Qt::red) );
	pointItem->hide();	

	// create label for ID of vehicle and show
	//
	QGraphicsTextItem *textItem = new QGraphicsTextItem( QString("%1").arg(mVehicleID) );
	//textItem->setPen( QPen(Qt::red) );
	textItem->setPos( QPointF(mXtext, mYtext) );
	QFont font;
	font.setPointSize( 6 );
	textItem->setFont( font );
	textItem->setOpacity( 0.6 );
	textItem->hide();
	//mVideoTrackView->scene()->addItem( textItem );

	// save trajectory point into main window
	//
	int frmIndex = mTimeInstance;
	QPair<TrajectoryPoint*, QGraphicsTextItem*> pair;
	pair.first = pointItem;
	pair.second = textItem;
	if ( !mTrajectory.contains(frmIndex) ) // not found points at time instance
	{
		QMap<int, QPair<TrajectoryPoint*, QGraphicsTextItem*>> timePoints;
		timePoints[ mVehicleID ] = pair;
		mTrajectory[ frmIndex ] = timePoints;
	} else {  // found point at given time instance 
		mTrajectory[ frmIndex ][ mVehicleID ] = pair;
	}
	
}

void ProjectFile::addItemsToView( VideoViewBase* view )
{
	//
	//#########################################
	QMapIterator<int, QMap<int, QPair<TrajectoryPoint*, QGraphicsTextItem*>> > i(mTrajectory);
	while ( i.hasNext() ) {
		i.next();  // move next
		int time = i.key();
		QMap<int, QPair<TrajectoryPoint*, QGraphicsTextItem*>> ptsMap = i.value();

		QMapIterator<int, QPair<TrajectoryPoint*, QGraphicsTextItem*>> j( ptsMap );
		while ( j.hasNext() )
		{
			j.next();  // move next

			int veicleID = j.key();
			QPair<TrajectoryPoint*, QGraphicsTextItem*> pt_text = j.value();
			TrajectoryPoint* pt = (TrajectoryPoint*)pt_text.first;
			QGraphicsTextItem* text = (QGraphicsTextItem*)pt_text.second;

			// add hide items to scene
			view->scene()->addItem( pt );    // show line	
			view->scene()->addItem( text );  // show line	
		}	 
	}
}

QPair<TrajectoryPoint*, QGraphicsTextItem*> ProjectFile::nearestPoint( float x, float y, int cur_time )
{
	float shortestDist = -1;
	float xFound = -1;
	float yFound = -1;
	float distTem;
	QPair<TrajectoryPoint*, QGraphicsTextItem*> result;
	result.first = NULL;
	result.second = NULL;
	QMapIterator<int, QMap<int, QPair<TrajectoryPoint*, QGraphicsTextItem*>> > i(mTrajectory);
	while ( i.hasNext() ) {
		i.next();  // move next

		// if not given time instance, skip
		if ( i.key() != cur_time )
			continue;

		// iterate points at given time instance
		//
		QMap<int, QPair<TrajectoryPoint*, QGraphicsTextItem*>> ptsMap = i.value();
		QMapIterator<int, QPair<TrajectoryPoint*, QGraphicsTextItem*>> j( ptsMap );
		while ( j.hasNext() )
		{
			j.next();  // move next
			
			int veicleID = j.key();
			QPair<TrajectoryPoint*, QGraphicsTextItem*> pt_text = j.value();
			TrajectoryPoint* pt = (TrajectoryPoint*)pt_text.first;
			QGraphicsTextItem* text = (QGraphicsTextItem*)pt_text.second;

			// change all item to origin color, red
			pt->setPen( QPen(Qt::red) );
			
			distTem = pow(pt->rect().x()-x, 2) + pow(pt->rect().y()-y, 2);
			if ( shortestDist<0 ){
				xFound = pt->rect().x();
				yFound = pt->rect().y();
				shortestDist = distTem;
				result.first = pt;
				result.second = text;
				continue;
			}
						
			if ( distTem < shortestDist )
			{
				xFound = pt->rect().x();
				yFound = pt->rect().y();
				shortestDist = distTem;
				result.first = pt;
				result.second = text;
			}
		}		
	} // end while ( i.hasNext() ) {

	return( result );
}


