#include "OCREngine.h"

COCREngine::COCREngine() {
	chiApi = new tesseract::TessBaseAPI();
	engApi = new tesseract::TessBaseAPI();
}

COCREngine::~COCREngine() {
	if (chiApi != NULL) {
		//chiApi->End();
		delete chiApi;
		chiApi = NULL;
	}
	if (engApi != NULL) {
		//engApi->End();
		delete engApi;
		engApi = NULL;
	}
}


bool COCREngine::init() {
	if (chiApi->Init(NULL, "chi_print")) {
		fprintf(stderr, "Could not initialize tesseract chi_sim.\n");
		return false;
	}
	if (engApi->Init(NULL, "eng")) {
		fprintf(stderr, "Could not initialize tesseract eng.\n");
		return false;
	}

	return true;
}

int COCREngine::getTextLine(const string &srcImg, string &ret) {
	Pix *img = pixRead(srcImg.c_str());
	char *chiText, *engText;
	int chiConf, engConf;
	
	chiApi->SetImage(img);
	engApi->SetImage(img);
	
	chiText = chiApi->GetUTF8Text();
	chiConf = chiApi->MeanTextConf();

	engText = engApi->GetUTF8Text();
	engConf = engApi->MeanTextConf();

	string chiStr = chiText;
	string engStr = engText;
	

	//clearText(chiStr);
	//clearText(engStr);

	mergeText(chiStr, engStr, ret);

	//if (chiConf >= engConf) {
	//	ret = string(chiText);
	//} else {
	//	ret = string(engText);
	//}
	//ret = string(chiText);

	delete[] chiText;
	delete[] engText;

	chiApi->End();
	engApi->End();

	//return max(chiConf, engConf);
	return chiConf;
}

int COCREngine::getText(const string &srcImg, vector<string> &ret) {
	Pix *img = pixRead(srcImg.c_str());
	char *chiText, *engText;
	int i, chiConf, engConf;

	chiApi->SetImage(img);
	engApi->SetImage(img);

	Boxa *boxes = chiApi->GetComponentImages(tesseract::RIL_TEXTLINE, true, NULL, NULL);
	printf("Found %d textline image components\n", boxes->n);
	for (i = 0; i < boxes->n; ++i) {
		BOX *box = boxaGetBox(boxes, i, L_CLONE);
		chiApi->SetRectangle(box->x, box->y, box->w, box->h);
		engApi->SetRectangle(box->x, box->y, box->w, box->h);
		
		chiText = chiApi->GetUTF8Text();
		chiConf = chiApi->MeanTextConf();
		
		engText = engApi->GetUTF8Text();
		engConf = engApi->MeanTextConf();
		
		printf("chi = %s, conf = %d\neng = %s, conf = %d\n", chiText, chiConf, engText, engConf);
	}

	return 0;
}

void COCREngine::clearText(string &str) const {
	int i;

	for (i = str.length() - 1; i >= 0; --i) {

		//printf("i = %d, str[i] = %02X, tar = %02X\n", i, str[i] & 0xFF, '\n');

		if (str[i] != '\n'); break;
	}

	str = str.substr(0, i + 1);
}

void COCREngine::mergeText(const string &chiTxt, const string &engTxt, string &ret) const {
	int n = chiTxt.length(), m = engTxt.length(), p, q, i, j;
	
	//printf("chi = %s\n", chiTxt.c_str());
	//printf("eng = %s\n", engTxt.c_str());

	ret = chiTxt;
}
