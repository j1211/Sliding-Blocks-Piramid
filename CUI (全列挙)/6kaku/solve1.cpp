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
	rep(i, g.size()) perm.push_back(i);
	
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
		cnts[it->second]++;
		maxCost = max(maxCost, it->second);
	}
	cout << "�����\�Ȍ� = " << minCost.size() << endl;
	cout << "�ő�萔 = " << maxCost << endl;
	
	for (map<vector<int>, int>::iterator it = minCost.begin(); it != minCost.end(); it++) {
		if (it->second == maxCost) {
			printArray(it->first, false);
			cout << ", cost = " << it->second << endl;
			break;
		}
	}
	
	int i;
	rep(i, maxCost + 1) {
		cout << "�ŒZ" << i << "[��]�Ŋ�������p�^�[���̐� = " << cnts[i] << endl;
	}
}

int main() {
	int y, x;
	
	cin >> N;
	
	int id = 0;
	rep(y, N) rep(x, y + 1) toId[P(y, x)] = id++;	//���W -> �ԍ�
	
	g.resize(id);
	rep(y, N - 1) {
		rep(x, y + 1) {
			connect(g, y, x, y + 1, x);
			connect(g, y + 1, x, y + 1, x + 1);
			connect(g, y + 1, x + 1, y, x);
		}
	}
	
	//�S�񋓂���
	map<vector<int>, int> minCost;
	solve(minCost);
	
	//�F��ȏ����o��
	printInfos(minCost);
	return 0;
}