#include "define.h"
#include "TextExtractor.h"

using namespace cv;

int main(int argc, char **argv) {
	CTextExtractor *ext = new CTextExtractor();
	//vector<TextPatch> ret;
	string ret;

	if (argc < 2) {
		fprintf(stderr, "No input file!\n");
		return 1;
	}

	int val = ext->imageToJson(string(argv[1]), ret);

	/*
	printf("tot lines: %d\n", ret.size());
	for (int i = 0; i < ret.size(); ++i) {
		printf("line %d:\n", i + 1);
		printf("%s\n", ret[i].X.c_str());
	}
	*/
	
	if (val == -1) {
		printf("OCR Failure!\n");
		return -1;
	}

	//printf("Avg conf = %d\n", val);
	printf("%s\n", ret.c_str());

	if (ext != NULL) {
		delete ext;
		ext = NULL;
	}

	return 0;
}
