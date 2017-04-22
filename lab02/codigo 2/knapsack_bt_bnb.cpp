/*******************************************************
 * MC658 - Projeto e Analise de Algoritmo III - 1s2017
 * Prof: Flavio Keidi Miyazawa
 * PED: Edson Ticona Zegarra
 ******************************************************/
#include "knapsack.h"

///Preencher aqui para facilitar a correcao.
// Nome1: Bruno Orsi Berton
// RA1: 150573
// Nome2:
// RA2:

vector<int> items;
int best_price;

bool is_new_class(int item, vector<int> &c, vector<int> temp) {
	// there is no item in the sack
	if (temp.empty()) {
		return false;
	}

	for(vector<int>::iterator it = temp.begin(); it != temp.end(); it++) {
		if (c[*it] == c[item]) {
			return false;
		}
	}

	return true;
}

void backtracking(int index, int current_weight, int current_value, int max_weight, int d, vector<int> &p, vector<int> &w, vector<int> &c, vector<int> current_items) {
	if (current_weight > max_weight) {
		return;
	}

	if (current_value > best_price) {
		best_price = current_value;
		if (!items.empty()) {
			items.clear();
		}
		// copy vector currentItems to items
		move(current_items.begin(), current_items.end(), back_inserter(items));
	}

	if (index == p.size()) {
			return;
	}

	int new_weight = current_weight + w[index];
	// check if is necessary to add the division
	if (is_new_class(index, c, current_items)) {
		new_weight += d;
	}

	// solution with item n
	current_items.push_back(index);
	backtracking(index + 1, new_weight, current_value + p[index], max_weight, d, p, w, c, current_items);

	// solution without item n
	current_items.pop_back();
	backtracking(index + 1, current_weight, current_value, max_weight, d, p, w, c, current_items);
}

///
// Bactracking function:
///
bool bt(int n, int d, int B, vector<int> &p, vector<int> &w, vector<int> &c, vector<int> &sol, int t) {
	// TODO t é timeout
	best_price = -1;
	vector<int> temp_items;
	backtracking(0, 0, 0, B, d, p, w, c, temp_items);

	for (int i = 0; i < items.size(); i++) {
		sol[items[i]] = 1;
	}

	return true;
}

///
// Branch and Bound function
///
bool bnb(int n, int d, int B, vector<int> &p, vector<int> &w, vector<int> &c, vector<int> &sol, int t) {
	return false;
}
