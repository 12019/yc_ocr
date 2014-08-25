#include "TextDetector.h"


CTextDetector::CTextDetector() : m_height(1024.0) {
}

CTextDetector::CTextDetector(double height) : m_height(height) {

}

CTextDetector::~CTextDetector() {
}

bool CTextDetector::textDetect(const string &imgStr, vector<Rect> &ret) {
	int n, m, i, j;
	Rect cur;
	
	if (!loadImg(imgStr)) return false;

	ms(grayImg, regions, Mat());
	regionRect.clear();
	for (vector< vector<Point> >::iterator it = regions.begin(); it != regions.end(); ++it) {
		cur = boundingRect(*it);
		if (checkArea(cur)) regionRect.push_back(cur);
	}

	uniqArea(regionRect);

	//for (auto &it : regionRect) {
	//	rectangle(grayImg, it, Scalar(255));
	//}
	//printf("%d\n", ret.size());

	//imshow("mser", grayImg);
	
	n = regionRect.size();
	graph.init(n);
	for (i = 0; i < n; ++i) {
		for (j = i + 1; j < n; ++j) {
			if (isConnected(regionRect[i], regionRect[j])) graph.addEdge(i, j);
		}
	}

	m = graph.connect();
	textRegions.resize(m);
	for (vector< vector<int> >::iterator it = textRegions.begin(); it != textRegions.end(); ++it) {
		it->clear();
	}
	for (i = 0; i < n; ++i) textRegions[graph.getIndex(i)].push_back(i);

	ret.clear();
	for (vector< vector<int> >::iterator it = textRegions.begin(); it != textRegions.end(); ++it) {
		if (it->size() < LIM_REG) continue;
		cur = regionRect[(*it)[0]];
		for (i = 1; i < it->size(); ++i) cur |= regionRect[(*it)[i]];
		ret.push_back(cur);
	}
	uniqArea(ret);

	//for (vector<Rect>::iterator it = ret.begin(); it != ret.end(); ++it) {
	//	rectangle(grayImg, *it, Scalar(255));
	//}
	//printf("%d\n", ret.size());

	//imshow("mser", grayImg);
	//waitKey(0);

	return true;
}

bool CTextDetector::textDetect(const string &imgStr, vector<Mat> &ret) {
	vector<Rect> allRect;
	vector<Rect>::iterator it;
	Mat curImg, roiImg, binImg;
	int blockSize = 25;
	int constValue = 10;

	if (!textDetect(imgStr, allRect)) return false;

	ret.clear();
	for (it = allRect.begin(); it != allRect.end(); ++it) {
		grayImg(*it).copyTo(curImg);
		//equalizeHist(curImg, roiImg);
		//threshold(roiImg, binImg, GRAY_THRESH, 255, THRESH_BINARY); 
		//adaptiveThreshold(curImg, binImg, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, blockSize, constValue);
		
		ret.push_back(curImg);
	}

	return true;
}

bool CTextDetector::loadImg(const string &imgStr) {
	Mat srcImg = imread(imgStr);
	double scale;

	if (!srcImg.data) return false;
	scale = m_height / (double)srcImg.cols;
	resize(srcImg, colorImg, Size(), scale, scale);
	cvtColor(colorImg, grayImg, CV_BGR2GRAY);
	//blur(tmpImg, grayImg, Size(3, 3));
	
	return true;
}

bool CTextDetector::checkArea(const Rect &rect) const {
	if (rect.x == 0 || rect.x + rect.width  == grayImg.cols) return false;
	if (rect.y == 0 || rect.y + rect.height == grayImg.rows) return false;
	if ((double)rect.height / (double)rect.width > LIM_H2W) return false;
	if ((double)rect.width / (double)rect.height > LIM_W2H) return false;
	if (rect.area() > grayImg.rows * grayImg.cols * LIM_AREA) return false;
	return true;
}

void CTextDetector::uniqArea(vector<Rect> &vec) const {
	vector<Rect> cur;
	int i, j;
	bool tag;

	for (i = 0; i < vec.size(); ++i) {
		for (tag = true, j = 0; j < vec.size(); ++j) {
			if (vec[i] == vec[j]) continue;
			if ((vec[i] | vec[j]) == vec[j]) {
				tag = false;
				break;
			}
		}
		if (tag) cur.push_back(vec[i]);
	}
	vec = cur;
}

bool CTextDetector::isConnected(const Rect &a, const Rect &b) const {
	if ((a | b) == a || (a | b) == b) return true;
	return abs(a.y - b.y) <= a.height && abs(a.y - b.y) <= b.height && abs(a.x - b.x) <= LIM_WID;
}
