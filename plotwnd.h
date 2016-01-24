#ifndef PLOTWND_H
#define PLOTWND_H
/************************************************************************/
/*             show 2d x-y plot
/* FileName    : D:\liye_360syn\syn_develop\tjems2\tjems2\plotwnd.h
/* Description : 
/* Author      : Liye Zhang, during study and work in China, USA and Singapore
/* Email       : chinazhangly@126.com 
/* Date        : 2015/01/14
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

#include "ui_plotwnd.h"
#include "qcustomplot.h"

class PlotWnd : public QDockWidget, public Ui::PlotWnd
{
	Q_OBJECT

public:
	PlotWnd(QWidget *parent);
	~PlotWnd();

	// push x, y data to vector for showing
	void pushData( double x, double y);	
		
	// show the last [width] element
	void plot();

	void setWidth( int width);

	// change the y range
	void setYRange(int start, int end);

private:
	QVector<double> mXAll;
	QVector<double> mYAll;
	QVector<double> mXPlot;   // only contain the resent mWidth value
	QVector<double> mYPlot;
	int mWidth;
};

#endif // PLOTWND_H