#ifndef TOOLSTUDYAREA_H
#define TOOLSTUDYAREA_H
/************************************************************************/
/*
/* FileName    : toolstudyarea.h
/* Description : 
/* Author      : Liye Zhang, during study and work in China, USA and Singapore
/* Email       : chinazhangly@126.com 
/* Date        : 2015/01/24
/* Copyright   : all right reserved
/*
/*------------------------------------------------------------------------------
/* Copyright Details:
/*
/* This program is developed as part of the results of Liye Zhang's PhD dissertation
/* from Zhong-Ren Peng's research group, Tongji University, China.
/* All the source code and related documents can only be copied or modified with the
/* permission of Liye Zhang.
/*
/* It is mainly developed during 2011-2015 in Univeristy of Florida, USA and Tongji
/* University, and University of Singapore.
/*                                                              
/************************************************************************/
#include "toolimage.h"
#include "videoviewbase.h"

class ToolStudyArea : public ToolImage
{
	Q_OBJECT

public:
	ToolStudyArea(VideoViewBase *parent);
	~ToolStudyArea();
	void clearTempPointLine();
	bool valid();

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

#endif // TOOLSTUDYAREA_H
