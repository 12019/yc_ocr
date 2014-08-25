#ifndef _TEXTDETECTOR_H_
#define _TEXTDETECTOR_H_

#include <opencv2/opencv.hpp>

#include "define.h"
#include "Graph.h"

using namespace cv;

class CTextDetector {
public:
	CTextDetector();
	CTextDetector(double height);
	~CTextDetector();

	bool textDetect(const string &imgStr, vector<Rect> &ret);
	bool textDetect(const string &imgStr, vector<Mat> &ret);

private:
	Mat colorImg, grayImg;
	double m_height;
	MSER ms;
	vector< vector<Point> > regions;
	vector<Rect> regionRect;
	vector< vector<int> > textRegions;

	CGraph graph;

	bool loadImg(const string &imgStr);
	bool checkArea(const Rect &rect) const;
	void uniqArea(vector<Rect> &vec) const;
	bool isConnected(const Rect &a, const Rect &b) const;
};

#endif
