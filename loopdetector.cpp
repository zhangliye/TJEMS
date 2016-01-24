#include "loopdetector.h"
#include "videoviewbase.h"
#include "mainwindow.h"

LoopDetector::LoopDetector(VideoViewBase *view)
	: QObject( (QObject*)view )
{
	mView = view;
	mPolygon = NULL;

	mSameCorThreshold = 0.8;	
	mCurDif_rate = -1;
}

LoopDetector::~LoopDetector()
{

}

void LoopDetector::setShape( QVector<QPointF> &points )
{
	//# remove the old one
	if (mPolygon != NULL)
		mView->scene()->removeItem( mPolygon );
	

	//# insert new polygon
	//#
	mPolygon = new QGraphicsPolygonItem( QPolygonF(points) );
	mPolygon->setPen( QPen(Qt::red) );                            //# detector color
	mView->scene()->addItem( mPolygon );
}

void LoopDetector::removeShape()
{
//# remove the old one
	if (mPolygon != NULL){
		mView->scene()->removeItem( mPolygon );
		delete mPolygon;
		mPolygon = NULL; 
	}
}

void LoopDetector::updateDetectorMask( const Mat &mat)
{
	// set detector background 
	//
	qreal xMin;
	qreal xMax;
	qreal yMin;
	qreal yMax;
	mPolygon->boundingRect().getCoords(&xMin, &yMin, &xMax, &yMax);

	if( xMin<0 )
		xMin = 0;
	if( yMin<0 )
		yMin = 0;
	if ( xMax>= mView->mCurrentFrame.cols )
		xMax = mView->mCurrentFrame.cols;
	if ( yMax>= mView->mCurrentFrame.rows )
		yMax = mView->mCurrentFrame.rows;

	// create small matrix to reture
	Mat subM = mat( Range(yMin, yMax), Range(xMin, xMax) ).clone(); //row, col	
		
	vector<Point> contSingle;
	QPointF pt;
	int x;
	int y;
	for (int i=0; i< mPolygon->polygon().size(); ++i)
	{
		pt = mPolygon->polygon().at(i);
		x = pt.x()-xMin >= 0 ? pt.x()-xMin : 0;
		y = pt.y()-yMin >= 0 ? pt.y()-yMin : 0;
		contSingle.push_back( Point(x, y) );	
	}

	vector<vector<Point>> contours;
	contours.push_back( contSingle );

	// create zero matrix and mark mask
	mMask = Mat( Mat::zeros(subM.size(), subM.type()) );
	fillPoly( mMask, contours, cv::Scalar(256,256,256));
	imwrite("d:/mask.PNG", mMask);
}

int LoopDetector::detectBicycle( const Mat &mat )
{
	Mat detectionMat = getDetectionArea( mat );

	double dif_rate = compareMat( mBackGroundMat, detectionMat, mMask);     //  0 -- totally different; 1 -- the same 
	mCurDif_rate = dif_rate;
	//double base_base2 = hist2( mBackGroundMat, detectionMat );     //  1 quite high
	
	// Mat subM = abs(mBackGroundMat - detectionMat);
	// Scalar m = cv::mean( subM );
		
	//mView->mMainWind->log( QString("same rate: %1, hist2--%2, ---%3").arg(abs(dif_rate)).arg(abs(base_base2)).arg(m[0]) );
	
	mView->mMainWind->mMatWnd->showMatUp( mBackGroundMat );
	mView->mMainWind->mMatWnd->showMatLow( detectionMat );

	if ( dif_rate < mSameCorThreshold ){  // quit different, bike emerged
		return(1);
	} else {
		return( -1 );
	}

	return(-1);
}

double LoopDetector::compareMat(const Mat &mat1, const Mat &mat2, const Mat &mat_mask)
{
	Mat mat1_gray;
	cvtColor( mat1, mat1_gray, COLOR_BGR2GRAY );

	Mat mat2_gray;
	cvtColor( mat2, mat2_gray, COLOR_BGR2GRAY );

	/// Using 50 bins for hue and 60 for saturation
	int b_bins = 60;
	int histSize[] = { b_bins };

	// hue varies from 0 to 179, saturation from 0 to 255
	float b_ranges[] = { 0, 256 };

	const float* ranges[] = { b_ranges };

	// Use the o-th and 1-st channels
	int channels[] = { 0 };

	/// Histograms
	MatND hist_background;
	MatND hist_mat;

	/// Calculate the histograms for the HSV images
	vector<Mat> bgr_planes;
	split( mat_mask, bgr_planes );
	
	calcHist( &mat1_gray, 1, channels, bgr_planes[0], hist_background, 1, histSize, ranges, true, false );
	//calcHist( &mat1_gray, 1, channels, Mat(), hist_background, 1, histSize, ranges, true, false );
	normalize( hist_background, hist_background );

	calcHist( &mat2_gray, 1, channels, bgr_planes[0], hist_mat, 1, histSize, ranges, true, false );
	//calcHist( &mat2_gray, 1, channels, Mat(), hist_mat, 1, histSize, ranges, true, false );
	normalize( hist_mat, hist_mat );

	double base_base = compareHist( hist_background, hist_mat, CV_COMP_CORREL );
	return(base_base);
}

double LoopDetector::hist2(Mat mBackGroundMat, Mat detectionMat)
{
	Mat mHsvBackGround;
	cvtColor( mBackGroundMat, mHsvBackGround, COLOR_BGR2HSV );

	Mat mHsvMat;
	cvtColor( detectionMat, mHsvMat, COLOR_BGR2HSV );

	/// Using 50 bins for hue and 60 for saturation
	int b_bins = 60; int g_bins = 60;
	int histSize[] = { b_bins, g_bins };

	// hue varies from 0 to 179, saturation from 0 to 255
	float b_ranges[] = { 0, 179 };
	float g_ranges[] = { 0, 256 };

	const float* ranges[] = { b_ranges, g_ranges};

	// Use the o-th and 1-st channels
	int channels[] = { 0, 1 };

	/// Histograms
	MatND hist_background;
	MatND hist_mat;

	/// Calculate the histograms for the HSV images
	calcHist( &mBackGroundMat, 1, channels, Mat(), hist_background, 2, histSize, ranges, true, false );
	normalize( hist_background, hist_background );

	calcHist( &detectionMat, 1, channels, Mat(), hist_mat, 2, histSize, ranges, true, false );
	normalize( hist_mat, hist_mat );

	double base_base = compareHist( hist_background, hist_mat, CV_COMP_CORREL );
	
	return (base_base);
}

void LoopDetector::updateBackGround( const Mat &mat )
{
	Mat mat_mask;
	mBackGroundMat = getDetectionArea( mat );
	mView->mMainWind->mMatWnd->showMatUp( mBackGroundMat );
}


cv::Mat LoopDetector::getDetectionArea( const Mat &mat )
{
	// set detector background 
	//
	qreal xMin;
	qreal xMax;
	qreal yMin;
	qreal yMax;
	mPolygon->boundingRect().getCoords(&xMin, &yMin, &xMax, &yMax);

	if( xMin<0 )
		xMin = 0;
	if( yMin<0 )
		yMin = 0;
	if ( xMax>= mView->mCurrentFrame.cols )
		xMax = mView->mCurrentFrame.cols;
	if ( yMax>= mView->mCurrentFrame.rows )
		yMax = mView->mCurrentFrame.rows;

	// create small matrix to reture
	Mat subM = mat( Range(yMin, yMax), Range(xMin, xMax) ).clone(); //row, col	
				
	return(subM);
}

void LoopDetector::increaseThreshold( float step /*= 0.01 */ )
{
	if ( mSameCorThreshold + step <= 1 ){
		mSameCorThreshold += step;
		mView->mMainWind->mLabEqualBkThreshold->setText( QString("co-relation threshold: %1").arg(mSameCorThreshold) );
	}
}

void LoopDetector::decreaseThreshold( float step /*= 0.01 */ )
{
	if ( mSameCorThreshold - step > 0){
		mSameCorThreshold -= step;
		mView->mMainWind->mLabEqualBkThreshold->setText( QString("co-relation threshold: %1").arg(mSameCorThreshold) );
	}
}
