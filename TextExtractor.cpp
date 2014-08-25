#include "TextExtractor.h"


CTextExtractor::CTextExtractor() {
	det = new CTextDetector();
	ocr = new COCREngine();
}

CTextExtractor::~CTextExtractor() {
	if (det != NULL) {
		delete det;
		det = NULL;
	}
	if (ocr != NULL) {
		delete ocr;
		ocr = NULL;
	}
}

int CTextExtractor::imageToString(const string &imgSrc, vector<TextPatch> &ret) {
	vector<Mat> imgPatch;
	vector<string> imgList, txtList;
	string curLine;
	int curVal, i, j, avg = 0, cnt = 0;
	char buff[MAX_BUFF];

	if (!det->textDetect(imgSrc, imgPatch)) {
		fprintf(stderr, "Fail to detect text from imag %s!\n", imgSrc.c_str());
		return -1;
	}
	
	ret.clear();
	string fname = getFileName(imgSrc);
	for (i = 0; i < imgPatch.size(); ++i) {
		if (!ocr->init()) {
			fprintf(stderr, "OCREngine init failure!\n");
			continue;
		}
		
		sprintf(buff, "%s_rslt_%d.jpg", fname.c_str(), i);
		imgList.push_back(string(buff));
		imwrite(buff, imgPatch[i]);
		curVal = ocr->getTextLine(imgList.back(), curLine);
		
		strSplit(curLine, '\n', txtList);
	
		//printf("size = %d\n", txtList.size());

		for (j = 0; j < txtList.size(); ++j) {
			if (txtList[j].length() > 5 && curVal >= LIM_CONF) {
				ret.push_back(make_pair(txtList[j], curVal));
				avg += curVal; ++cnt;
			}
		}
/*
		if (curLine.length() > 5 && curVal >= LIM_CONF) {
			ret.push_back(make_pair(curLine, curVal));
			avg += curVal;
		}
*/		
		//showResult(imgPatch[i], curLine, curVal);
	}

	for (i = 0; i < imgList.size(); ++i) {
		remove(imgList[i].c_str());
	}
	
	return ret.size() == 0 ? -1 : avg / ret.size();
}

int CTextExtractor::imageToJson(const string &imgSrc, string &ret) {
	vector<TextPatch> txt;
	vector<int> pos;
	map<string, string> mp;
	set<string> st;
	int i, avg = imageToString(imgSrc, txt);
	
	if (avg == -1) return -1;
	
	mp.clear(); st.clear();
	for (i = 0; i < txt.size(); ++i) {
		strProc(txt[i].X, mp, st);
	}
	dataToJson(mp, st, ret);

	return avg;
}

int CTextExtractor::strSplit(const string &str, char ch, vector<string> &ret) const {
	string cur;
	int i;
	
	ret.clear();
	for (i = 0; i < str.length(); ++i) {
		if (str[i] == ch) {
			if (cur.length() > 0) ret.push_back(cur);
			cur.clear();
		} else {
			cur.push_back(str[i]);
		}
	}
	if (cur.length() > 0) ret.push_back(cur);
	
	return ret.size();
}

string CTextExtractor::getFileName(const string &str) const {
	string ret;
	for (int i = 0; i < str.length() && str[i] != '.'; ++i) {
		ret.push_back(str[i]);
	}
	return ret;
}

void CTextExtractor::showResult(const Mat &img, const string &txt, int conf) const {
	imshow("ImgPatch", img);
	printf("*************************************************\n");
	printf("Text get from image:\n%s\n", txt.c_str());
	printf("conf val = %d\n", conf);
	printf("*************************************************\n\n");
	waitKey(0);
}

int CTextExtractor::getIndex(const string &str, char ch, vector<int> &ret) const {
	int i;

	ret.clear();
	for (i = 0; i < str.length(); ++i) {
		if (str[i] == ch) ret.push_back(i);
	}
	
	return ret.size();
}

void CTextExtractor::strProc(const string &str, map<string, string> &mp, set<string> &st) const {
	int idx = str.find(":");
	map<string, string>::iterator it;
	string key, val;

	if (idx != string::npos) {
		key = str.substr(0, idx);
		val = str.substr(idx + 1);
		it = mp.find(key);
		if (it == mp.end()) mp.insert(make_pair(key, val));
		else it->Y += ", " + val;
	} else {
		st.insert(str);
	}
}

void CTextExtractor::dataToJson(const map<string, string> &mp, const set<string> &st, string &ret) const {
	map<string, string>::const_iterator mit;
	set<string>::const_iterator sit;
	int cnt = 0;
	char buff[MAX_BUFF];

	ret = "{";
	for (mit = mp.begin(); mit != mp.end(); ++mit) {
		if (mit != mp.begin()) ret += " ,";
		ret += " \"" + mit->X + "\"\:\"" + mit->Y + "\"";
	}
	for (sit = st.begin(); sit != st.end(); ++sit) {
		if (mp.size() > 0 || sit != st.begin()) ret += ", ";
		sprintf(buff, "item%02d", ++cnt);
		ret += " \"" + string(buff) + "\"\:\"" + *sit + "\"";
	}

	ret += " }";
}
