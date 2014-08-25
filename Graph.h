#ifndef _GRAPH_H_
#define _GRAPH_H_

#include "define.h"

class CGraph {
public:
	CGraph();
	~CGraph();

	void init(int n);
	void addEdge(int a, int b);
	int connect();

	int getIndex(int x) const {
		return idx[x];
	}

private:
	struct Edge {
		int ed, nxt;
		
		Edge() {}
		Edge(int _ed, int _nxt) : ed(_ed), nxt(_nxt) {}
	};

	vector<Edge> edge;
	vector<int>  head;
	vector<int>  idx;
	int nV, nE, cnt;

	void dfs(int x, int id);
};

#endif

