#include "Graph.h"


CGraph::CGraph() {
}


CGraph::~CGraph() {
}

void CGraph::init(int n) {
	head.resize(n);
	edge.clear();
	nV = n; nE = 0;
	fill(head.begin(), head.end(), -1);
}

void CGraph::addEdge(int a, int b) {
	edge.push_back(Edge(b, head[a])); head[a] = nE++;
	edge.push_back(Edge(a, head[b])); head[b] = nE++;
}

int CGraph::connect() {
	int i, ret = 0;

	idx.resize(nV);
	fill(idx.begin(), idx.end(), -1);
	for (i = 0; i < nV; ++i) {
		if (idx[i] == -1) dfs(i, ret++);
	}

	return ret;
}

void CGraph::dfs(int x, int id) {
	int i;
	
	idx[x] = id;
	for (i = head[x]; ~i; i = edge[i].nxt) {
		if (idx[edge[i].ed] == -1) dfs(edge[i].ed, id);
	}
}