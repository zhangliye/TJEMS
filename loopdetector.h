#ifndef LOOPDETECTOR_H
#define LOOPDETECTOR_H

#include <QObject>
#include "pub.h"

class VideoViewBase;
class LoopDetector : public QObject
{
	Q_OBJECT
public:


public:
	LoopDetector(VideoViewBase *view);
	~LoopDetector();

	void setShape( QVector<QPointF> &points);
	void removeShape();

	// update background matrix, as time elapse background change
	void updateBackGround( const Mat &mat ); 

	/************************************************************************/
	/* function    : return the compare value
	/* parameter   : mat_mask - Mat,mask, 1 to consider
	/* return      : realted rate for CV_COMP_CORREL;  0 -- totally different; 1 -- the same 
	/************************************************************************/
	// 
	double compareMat(const Mat &mat1, const Mat &mat2, const Mat &mat_mask);
	
	double hist2(Mat mBackGroundMat, Mat detectionMat);

	// para: mat - whole image of one frame in video
	// return: 1 - traditional bicycle, 2 - e-bicyle, -1 - None 
	int detectBicycle( const Mat &mat );

	void increaseThreshold( float step = 0.01 );
	void decreaseThreshold( float step = 0.01 );
	
	Mat getDetectionArea( const Mat &mat );
	void updateDetectorMask( const Mat &mat);

public:
	Mat mBackGroundMat;   // background under detector
	Mat mMask;            // mask of detector polygon area
	VideoViewBase *mView;
	QGraphicsPolygonItem *mPolygon;
	float mSameCorThreshold;    // corelation rate, defaul == 0.8, as same background
	float mCurDif_rate;  // current dif_rate, for debugging 
};

#endif // LOOPDETECTOR_H
