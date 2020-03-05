#include <iostream>
#include <vector>
#include <map>
#include <queue>
#define rep(i, n) for(i = 0; i < n; i++)
using namespace std;
typedef pair<int, int> P;
typedef vector<int> Edges;
typedef vector<Edges> Graph;

int N;
map<P, int> toId;
Graph g;

//���W(y1, x1)�ƑΉ������ --edge--> ���W(y2, x2)�ƑΉ������
void connect(Graph &g, int y1, int x1, int y2, int x2) {
	int u = toId[P(y1, x1)];
	int v = toId[P(y2, x2)];
	g[u].push_back(v);
}

//�J�ڊ֐� (�}�Xi�ɂ̓R�C��p[i]���u����Ă���j
vector<vector<int>> generateMoves(vector<int> p) {
	int i, j;
	vector<vector<int>> ret;
	
	rep(i, g.size()) {
		rep(j, g[i].size()) {
			int u = i;
			int v = g[i][j];
			
			if (p[u] != 0 && p[v] != 0) continue;	//�ǂ��炩�ɂ�0���u����Ă���
			swap(p[u], p[v]);	//����ւ� (�􉽓I�ɂ̓p�^���Ɛ܂�Ԃ��j
			ret.push_back(p);
			swap(p[u], p[v]);	//���ɖ߂��Ă���
		}
	}
	return ret;
}

//�S��.
void solve(map<vector<int>, int> &minCost) {
	int i;
	vector<int> perm;
	rep(i, N * N) perm.push_back(i);
	
	queue<vector<int>> que;
	que.push(perm);
	minCost[perm] = 0;
	
	while (!que.empty()) {
		vector<int> p = que.front();
		que.pop();
		
		vector<vector<int>> nextPs = generateMoves(p);
		
		for (vector<int> nextP : nextPs) {
			if (minCost.find(nextP) == minCost.end()) {
				que.push(nextP);
				minCost[nextP] = minCost[p] + 1;
			}
		}
	}
}

void printArray(vector<int> p, bool endFlag = true) {
	int i;
	cout << "[";
	rep(i, p.size()) {
		cout << p[i];
		if (i + 1 < p.size()) cout << ", ";
	}
	cout << "]";
	if (endFlag) cout << endl;
}

//�F��ȏ����o��
void printInfos(map<vector<int>, int> &minCost) {
	int maxCost = -1;
	int cnts[1000] = {0};
	
	for (map<vector<int>, int>::iterator it = minCost.begin(); it != minCost.end(); it++) {
		vector<int> p = it->first;
		printArray(p, false);
		cout << ", cost = " << it->second << endl;
		cnts[it->second]++;
		maxCost = max(maxCost, it->second);
	}
	cout << "�����\�Ȍ� = " << minCost.size() << endl;
	cout << "�ő�萔 = " << maxCost << endl;
	
	int i;
	rep(i, maxCost + 1) {
		cout << "�ŒZ" << i << "[��]�Ŋ�������p�^�[���̐� = " << cnts[i] << endl;
	}
}

int main() {
	int y, x;
	
	cin >> N;
	
	int id = 0;
	rep(y, N) rep(x, 2 * y + 1) toId[P(y, x)] = id++;	//���W -> �ԍ�
	
	g.resize(N * N);
	for (y = 1; y < N; y++) {
		for (x = 1; x < 2 * y; x += 2) { //��
			connect(g, y, x, y - 1, x - 1);
			connect(g, y, x, y, x - 1);
			connect(g, y, x, y, x + 1);
		}
	}
	
	//�S�񋓂���
	map<vector<int>, int> minCost;
	solve(minCost);
	
	//�F��ȏ����o��
	printInfos(minCost);
	return 0;
}