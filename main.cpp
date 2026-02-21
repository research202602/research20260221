#include <iostream>
#include <vector>
#include <algorithm>
#include <bitset>
#include <cassert>
#include <set>
#include <queue>
#include <functional>

using namespace std;
//k=2,n=3
const int N = 9;  // Total points
const int SUB_SIZE = 3;  // Number of points in each subgraph
const int SUB_COUNT = 3;  // Number of subgraph

// Graph structure
struct Graph {
	bitset<N> adj[N];  

	// Add Edge
	void add_edge(int u, int v) {
		adj[u][v] = adj[v][u] = 1;
	}

	// Check if there is an edge between two points
	bool has_edge(int u, int v) const {
		return adj[u][v];
	}

	// Get neighbors
	vector<int> neighbors(int u) const {
		vector<int> res;
		for (int v = 0; v < N; v++) {
			if (has_edge(u, v)) res.push_back(v);
		}
		return res;
	}
};

// Generate all possible graph structures 216 6*6*6
vector<Graph> generate_all_graphs() {
	vector<Graph> graphs;

	// Enumerate all possible subgraph matches
	// Assume：A(0,1,2), B(3,4,5), C(6,7,8)

	// Generate all matches of A and B
	vector<vector<pair<int, int>>> ab_matches;
	vector<int> a_nodes = {0, 1, 2};
	vector<int> b_nodes = {3, 4, 5};

	do {
		vector<pair<int, int>> match;
		for (int i = 0; i < SUB_SIZE; i++) {//0 1 2 5 4 3
			match.push_back({a_nodes[i], b_nodes[i]});
		}
		ab_matches.push_back(match);
	} while (next_permutation(b_nodes.begin(), b_nodes.end()));

	// Generate all matches of B C
	vector<vector<pair<int, int>>> bc_matches;
	vector<int> c_nodes = {6, 7, 8};

	do {
		vector<pair<int, int>> match;
		for (int i = 0; i < SUB_SIZE; i++) {
			match.push_back({b_nodes[i], c_nodes[i]});//3 4 5
		}
		bc_matches.push_back(match);
	} while (next_permutation(c_nodes.begin(), c_nodes.end()));//Permutation

	// Generate all matches of A C
	vector<vector<pair<int, int>>> ac_matches;

	do {
		vector<pair<int, int>> match;
		for (int i = 0; i < SUB_SIZE; i++) {
			match.push_back({a_nodes[i], c_nodes[i]});
		}
		ac_matches.push_back(match);
	} while (next_permutation(c_nodes.begin(), c_nodes.end()));

	// Generate all combinations
	for (const auto& ab_match : ab_matches) {
		for (const auto& bc_match : bc_matches) {
			for (const auto& ac_match : ac_matches) {
				Graph g;

				// Add edges of a complete graph inside the subgraph
				for (int sub = 0; sub < SUB_COUNT; sub++) {
					int start = sub * SUB_SIZE;
					for (int i = 0; i < SUB_SIZE; i++) {
						for (int j = i + 1; j < SUB_SIZE; j++) {
							g.add_edge(start + i, start + j);
						}
					}
				}

				// Add matching edges between subgraphs
				for (const auto& [u, v] : ab_match) {
					g.add_edge(u, v);
				}
				for (const auto& [u, v] : bc_match) {
					g.add_edge(u, v);
				}
				for (const auto& [u, v] : ac_match) {
					g.add_edge(u, v);
				}

				graphs.push_back(g);
			}
		}
	}

	return graphs;
}


bool find_two_paths(const Graph& g, int s1, int t1, int s2, int t2,
                    vector<int>& path1, vector<int>& path2) {

	if (s1 == s2 || s1 == t2 || t1 == s2 || t1 == t2) {

		return false;
	}


	vector<vector<int>> all_paths1;


	function<void(int, vector<int>&, bitset<N>&)> dfs1 =
	[&](int u, vector<int>& path, bitset<N>& visited) {
		if (u == t1) {
			all_paths1.push_back(path);
			return;
		}

		for (int v : g.neighbors(u)) {
	
			if (v == s2 || v == t2) continue;

			if (!visited[v]) {
				visited[v] = 1;
				path.push_back(v);
				dfs1(v, path, visited);
				path.pop_back();
				visited[v] = 0;
			}
		}
	};

	vector<int> p1 = {s1};
	bitset<N> vis1;
	vis1[s1] = 1;

	vis1[s2] = 1;
	vis1[t2] = 1;
	dfs1(s1, p1, vis1);


	for (const auto& p1_candidate : all_paths1) {
		bitset<N> covered;
		for (int v : p1_candidate) covered[v] = 1;


		vector<int> remaining_nodes;
		for (int i = 0; i < N; i++) {
			if (!covered[i]) {
				remaining_nodes.push_back(i);
			}
		}


		if (!covered[s2] && !covered[t2]) {

			set<int> remaining_set(remaining_nodes.begin(), remaining_nodes.end());

			vector<vector<int>> all_paths2;
			function<void(int, vector<int>&, bitset<N>&)> dfs2 =
			[&](int u, vector<int>& path, bitset<N>& visited) {
				if (u == t2) {
		
					if (path.size() == remaining_nodes.size()) {
						all_paths2.push_back(path);
					}
					return;
				}

				for (int v : g.neighbors(u)) {
					if (remaining_set.count(v) && !visited[v]) {
						visited[v] = 1;
						path.push_back(v);
						dfs2(v, path, visited);
						path.pop_back();
						visited[v] = 0;
					}
				}
			};

			vector<int> p2 = {s2};
			bitset<N> vis2;
			vis2[s2] = 1;
			dfs2(s2, p2, vis2);

			if (!all_paths2.empty()) {
				path1 = p1_candidate;
				path2 = all_paths2[0];
				return true;
			}
		}
	}

	return false;
}



bool verify_graph(const Graph& g) {

	for (int s1 = 0; s1 < N; s1++) {
		for (int t1 = 0; t1 < N; t1++) {
			if (s1 == t1) continue;
			for (int s2 = 0; s2 < N; s2++) {
				if (s2 == s1 || s2 == t1) continue;
				for (int t2 = 0; t2 < N; t2++) {
					if (t2 == s1 || t2 == t1 || t2 == s2) continue;

					vector<int> path1, path2;

					if (find_two_paths(g, s1, t1, s2, t2, path1, path2)) {
				

					
						cout << "success to found!\n";
						cout << "s1=" << s1 << ", t1=" << t1
						     << ", s2=" << s2 << ", t2=" << t2 << endl;

			
						cout << "Graph edges:\n";
						for (int i = 0; i < N; i++) {
							for (int j = i + 1; j < N; j++) {
								if (g.has_edge(i, j)) {
									cout << i << " - " << j << endl;
								}
							}
						}
						cout << "p1:\n";
						for (int i = 0; i < path1.size() - 1; i++) {
							cout << path1[i] << "->";
						}
						cout << path1[path1.size() - 1];

						cout << endl;
						cout << "p2:\n";
						for (int i = 0; i < path2.size() - 1; i++) {
							cout << path2[i] << "->";
						}
						cout << path2[path2.size() - 1];
						cout << endl;
						continue;  
					}

			
					cout << "Counterexample found!\n";
					cout << "s1=" << s1 << ", t1=" << t1
					     << ", s2=" << s2 << ", t2=" << t2 << endl;

		
					cout << "Graph edges:\n";
					for (int i = 0; i < N; i++) {
						for (int j = i + 1; j < N; j++) {
							if (g.has_edge(i, j)) {
								cout << i << " - " << j << endl;
							}
						}
					}

					return false;
				}
			}
		}
	}
	return true;
}

int main() {
	freopen("2-dpc.out","w",stdout);
	cout << "Generating all possible graphs..." << endl;
	auto graphs = generate_all_graphs();
	cout << "Total graphs: " << graphs.size() << endl;


	bool all_valid = true;

	for (size_t i = 0; i < graphs.size(); i++) {
		cout << "Verifying graph " << i + 1 << "/" << graphs.size() << "... ";

		if (verify_graph(graphs[i])) {
			cout << "Valid" << endl;
		} else {
			cout << "Invalid!" << endl;
			all_valid = false;
			break;
		}
	}

	if (all_valid) {
		cout << "\nThe theorem holds: For all such graphs, for any four distinct points s1, s2, t1, t2," << endl;
		cout << "There are two paths, s1->t1 and s2->t2, that cover all the points." << endl;
	} else {
		cout << "\nThe theorem does not hold: find a counterexample" << endl;
	}

	return 0;
}
