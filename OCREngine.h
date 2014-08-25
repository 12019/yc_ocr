#ifndef _OCRENGINE_H_
#define _OCRENGINE_H_

#include "define.h"
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

class COCREngine {
public:
	COCREngine();
	~COCREngine();

	bool init();
	int getTextLine(const string &srcImg, string &ret);
	int getText(const string &srcImg, vector<string> &ret);


private:
	tesseract::TessBaseAPI *chiApi;
	tesseract::TessBaseAPI *engApi;
	
	void clearText(string &str) const;
	void mergeText(const string &chiTxt, const string &engTxt, string &ret) const;
};

#endif
