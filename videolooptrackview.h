#ifndef VIDEOPROCESSLOOPVIEW_H
#define VIDEOPROCESSLOOPVIEW_H
/************************************************************************/
/*               Video processing window for loop detector, show when in 
/*               loop detector model
/* FileName    : D:\liye_360syn\syn_develop\tjems2\tjems2\videoprocessloopview.h
/* Description : 
/* Author      : Liye Zhang, during study and work in China, USA and Singapore
/* Email       : chinazhangly@126.com 
/* Date        : 2015/01/17
/* Copyright   : all right reserved
/*
/*------------------------------------------------------------------------------
/* Copyright Details:
/*
/* This program is developed as part of the results of Liye Zhang's PhD dissertation
/* from Zhong-Ren Peng's research group, Tongji University, China.
/* All the source code and related documents can only be copied or modified with the
/* permission of Liye Zhang and Prof. Zhong-Ren Peng's group.
/*
/* It is mainly developed during 2011-2015 in Univeristy of Florida, USA and Tongji
/* University, and University of Singapore.
/*                                                              
/************************************************************************/
#include "videoviewbase.h"
class VideoLoopTrackView : public VideoViewBase
{
	Q_OBJECT

public:
	VideoLoopTrackView(QWidget *parent);
	~VideoLoopTrackView();

public slots:
	/************************************************************************/
	/* function    : for time auto invoce action or single step video processing
	/* parameter   : customStep- custom frame sampling step; 
	/*               ifShowCorelateInf - if show rate value in main window log
	/************************************************************************/
	void actNextFrame(int customStep=-1, bool ifShowCorelateInf=false );	
	
	void actStartProcessing();
	void actPauseProcessing();
	void actEndProcessing();		

public:
	// data for loop detector
	// 
	QList<LoopDetector*> mLoopDetectors;
	LoopDetector *mCurLoopDetector;    // current running detector	

	bool mDetectionModel;     // detection model, detector worked only when in detection model
};

#endif // VIDEOPROCESSLOOPVIEW_H
