#ifndef _TEXTEXTRACTOR_H_
#define _TEXTEXTRACTOR_H_

#include "define.h"
#include "TextDetector.h"
#include "OCREngine.h"

using namespace cv;

class CTextExtractor {
public:
	CTextExtractor();
	~CTextExtractor();

	int imageToString(const string &srcImg, vector<TextPatch> &ret);
	int imageToJson(const string &srcImg, string &ret);

private:
	CTextDetector *det;
	COCREngine *ocr;

	int strSplit(const string &str, char ch, vector<string> &ret) const;
	string getFileName(const string &str) const;
	void showResult(const Mat &img, const string &txt, int conf) const;
	int getIndex(const string &str, char ch, vector<int> &ret) const;
	void strProc(const string &str, map<string, string> &mp, set<string> &st) const;
	bool getTel(const string &str, string &tel) const;
	bool getMob(const string &str, string &mob) const; 
	void dataToJson(const map<string, string> &mp, const set<string> &st, string &ret) const;
};

#endif
