/*******************************************************
 * MC658 - Projeto e Analise de Algoritmo III - 1s2017
 * Prof: Flavio Keidi Miyazawa
 * PED: Edson Ticona Zegarra
 ******************************************************/
#include "transportation.h"

///Preencher aqui para facilitar a correcao. 
// Nome1: Bruno Orsi Berton
// RA1: 150573
// Nome2: Fabio Takahashi Tanniguchi
// RA2: 145980
// Gurobi Version: 6.0.2
///
// PL function
///
bool pl(ListBpGraph& g, ListBpGraph::EdgeMap<int> &c, ListBpGraph::NodeMap<int> &v, ListBpGraph::EdgeMap<int> &sol, int tMax){
	// LEMBRETE: terminal azul, roteador vermelho
	
	try {
		GRBEnv env = GRBEnv();
		GRBModel model = GRBModel(env);
		model.set(GRB_IntAttr_ModelSense, GRB_MINIMIZE);
		env.set(GRB_DoubleParam_TimeLimit, tMax);

		int qtdArestas = 0;
		
		for(ListBpGraph::EdgeIt e(g); e != INVALID; ++e){
			qtdArestas++;
		}
		
		// definindo variavel x
		vector<GRBVar> x(qtdArestas); // indica o quanto passa pela aresta
		
		for(ListBpGraph::EdgeIt e(g); e != INVALID; ++e){
			x[g.id(e)] = model.addVar(0, GRB_INFINITY, 0, GRB_INTEGER);
		}
		
		model.update();
		
		// definindo funcao objetivo
		GRBLinExpr exprObj;
		for(ListBpGraph::EdgeIt e(g); e != INVALID; ++e){
			exprObj += x[g.id(e)] * c[e];
		}
		model.setObjective(exprObj, GRB_MINIMIZE);
		model.update();
		
		// definindo restricoes
		
		for(ListBpGraph::NodeIt nodeIt(g); nodeIt != INVALID; ++nodeIt){
			GRBLinExpr expr;
			int i = 0;
			if(g.red(nodeIt)){
				for(ListBpGraph::IncEdgeIt e(g, nodeIt); e != INVALID; ++e){
					ListBpGraph::Edge aresta = e;
					expr += x[g.id(aresta)];
					i++;
				}
				if(i>0){
					int demanda = v[nodeIt];
					model.addConstr(expr ==  demanda); // soma dos trafegos nas arestas que saem do terminal deve ser igual aa demanda do terminal
				}
			}else{
				for(ListBpGraph::IncEdgeIt e(g, nodeIt); e != INVALID; ++e){
					ListBpGraph::Edge aresta = e;
					expr += x[g.id(aresta)];
					i++;
				}
				if(i>0){
					int capacidade = v[nodeIt];
					model.addConstr(expr <= capacidade); // soma dos trafegos nas arestas que chegam ao roteador deve ser menor ou igual aa capacidade do roteador
				}
			}
		}
		
		model.update();

		model.optimize(); // roda o solver
		int status = model.get(GRB_IntAttr_Status);
		
		for(ListBpGraph::EdgeIt e(g); e != INVALID; ++e){
			sol[e] = x[g.id(e)].get(GRB_DoubleAttr_X);
		}
		
		if (status == GRB_OPTIMAL){ // solucao otima
			return true;
		}
		
	}catch(GRBException e) {
		cerr << "Nao foi possivel resolver o PLI." << endl;
		cerr << "Codigo de erro = " << e.getErrorCode() << endl;
		cerr << e.getMessage();
	}
	
	return false;
}
