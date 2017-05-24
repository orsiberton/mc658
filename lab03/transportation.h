/*******************************************************
 * MC658 - Projeto e Analise de Algoritmo III - 1s2017
 * Prof: Flavio Keidi Miyazawa
 * PED: Edson Ticona Zegarra
 ******************************************************/
#ifndef TRANSPORTATION_H
#define TRANSPORTATION_H
#include<string>
#include<algorithm>
#include<iostream>
#include<lemon/list_graph.h>
#include<lemon/graph_to_eps.h>
#include<gurobi_c++.h>
#include"mygraphlib.h"
using namespace lemon;
int read_transportation(std::string);
int show_usage();
int show_input();
int make_eps_graph(ListBpGraph::EdgeMap<int>& color, std::string name);
bool pl(ListBpGraph &g, ListBpGraph::EdgeMap<int> &c, ListBpGraph::NodeMap<int> &v, ListBpGraph::EdgeMap<int> &sol, int tMax);
bool is_feasible_solution(int& cost, int verbose);
void show_graph_mygraphlib(std::string text);
#endif
