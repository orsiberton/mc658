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

vector<int> original_item_index;
vector<int> items;
int best_price;

int partition(vector<int>& P, vector<int>& W, vector<int>& C, int left, int right, float who) {
  for (int i = left; i < right; i++) {
    if (P[i]/(double) W[i] > who) {
      swap(P[i], P[left]);
			swap(W[i], W[left]);
			swap(C[i], C[left]);
			swap(original_item_index[i], original_item_index[left]);
      left ++;
    }
  }
  return left - 1;
}

// quicksort based on price/weight for multiple vectors
void qsort(vector<int>& P, vector<int>& W, vector<int>& C, int left, int right) {
  if (left >= right) return;

  int middle = left + (right - left) / 2;
  swap(P[middle], P[left]);
	swap(W[middle], W[left]);
	swap(C[middle], C[left]);
	swap(original_item_index[middle], original_item_index[left]);
  int midpoint = partition(P, W, C, left + 1, right, P[left]/W[left]);
  swap(P[left], P[midpoint]);
	swap(W[left], W[midpoint]);
	swap(C[left], C[midpoint]);
	swap(original_item_index[left], original_item_index[midpoint]);
  qsort(P, W, C, left, midpoint);
  qsort(P, W, C, midpoint + 1, right);
}

// TODO remover
void printVector(vector<int>& A) {
  for (int i = 0; i < A.size(); i++) {
    cout << A[i] << " ";
  }
  cout << endl;
}

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

void branch_and_bound(int index, int current_weight, int current_value, int max_weight, int d, vector<int> &p, vector<int> &w, vector<int> &c, vector<int> current_items) {
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

	float best_until_now = current_value + ((max_weight - current_weight) * (p[index]/(double) w[index]));

	// this branch can be cut off if its value is not up to the best
	if (best_until_now >= best_price + 1) {
		// solution with item n
		current_items.push_back(index);
		branch_and_bound(index + 1, new_weight, current_value + p[index], max_weight, d, p, w, c, current_items);
		current_items.pop_back();
	}

	// solution without item n
	branch_and_bound(index + 1, current_weight, current_value, max_weight, d, p, w, c, current_items);
}

///
// Bactracking function:
///
bool bt(int n, int d, int B, vector<int> &p, vector<int> &w, vector<int> &c, vector<int> &sol, int t) {
	// TODO t é timeout
	// TODO fazer timeout
	best_price = -1;
	vector<int> temp_items;
	// copy vector for sort price/weight
	vector<int> p_copy(p);
	vector<int> w_copy(w);
	vector<int> c_copy(c);
	bool passed = true;

	// save original index of items to be able to print the right solution
	for (int i = 0; i < p.size(); i++) {
		original_item_index.push_back(i);
	}

	// sort based on price/weight
	qsort(p_copy, w_copy, c_copy, 0, p.size());

	// do the magic
	backtracking(0, 0, 0, B, d, p_copy, w_copy, c_copy, temp_items);

	// save solution
	for (int i = 0; i < items.size(); i++) {
		sol[original_item_index[items[i]]] = 1;
	}

	return passed;
}

///
// Branch and Bound function
///
bool bnb(int n, int d, int B, vector<int> &p, vector<int> &w, vector<int> &c, vector<int> &sol, int t) {
	// TODO t é timeout
	// TODO fazer timeout
	best_price = -1;
	vector<int> temp_items;
	// copy vector for sort price/weight
	vector<int> p_copy(p);
	vector<int> w_copy(w);
	vector<int> c_copy(c);
	bool passed = true;

	// save original index of items to be able to print the right solution
	for (int i = 0; i < p.size(); i++) {
		original_item_index.push_back(i);
	}

	// sort based on price/weight
	qsort(p_copy, w_copy, c_copy, 0, p.size());

	// do the magic
	branch_and_bound(0, 0, 0, B, d, p_copy, w_copy, c_copy, temp_items);

	// save solution
	for (int i = 0; i < items.size(); i++) {
		sol[original_item_index[items[i]]] = 1;
	}

	return passed;
}
