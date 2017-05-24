/*******************************************************
 * MC658 - Projeto e Analise de Algoritmo III - 1s2017
 * Prof: Flavio Keidi Miyazawa
 * PED: Edson Ticona Zegarra
 ******************************************************/
#include "transportation.h"
#include<fstream>
#include<iostream>
#include<unistd.h>
#include<cstring>
#include<map>
using namespace lemon;
int n1, n2, m;
int tMax;			//Maximum execution time, in seconds
ListBpGraph g;			//Bipartite graph
ListBpGraph::EdgeMap<int> c(g);	//Costs of connecting a terminal with a router
ListBpGraph::NodeMap<int> v(g);	//Velocity requirements/capacities of a terminal/router
ListBpGraph::EdgeMap<int> sol(g);	//Solution
ListBpGraph::NodeMap<std::string> node_names(g);	//Node names
ListBpGraph::NodeMap<double> px(g), py(g);//For drawing the graph
int main(int argc, char* argv[]){
	tMax = 600;
	int verbose = 0;
	std::string input_file;
	int option, i_f = 0;
	if ( argc == 1 ){
		show_usage();
		return 0;
	}
	while ((option = getopt(argc, argv, "t:i:v"))!=-1)
		switch(option){
			case 't':
				tMax=atoi(optarg);
				break;
			case 'i':
				i_f = 1;
				input_file.assign(optarg);
				break;
			case 'v':
				verbose=1;
				break;
			default:
				break;
		}

	if ( i_f == 0 ){
		std::cout << "-i  mandatory argument" << std::endl;
		return 1;
	}

	if ( !read_transportation(input_file) ) return 1;

	if ( verbose )
		show_input();

	pl(g, c, v, sol, tMax);
	int cost = 0;
	if ( verbose == 1 ){
		make_eps_graph(sol, "sol");
		set_pdfreader("evince");
		//set_pdfreader("open");
		//set_pdfreader("xpdf");
		show_graph_mygraphlib(input_file);
	}
	if ( !is_feasible_solution(cost, verbose) )
		std::cout << "Infeasible solution" << std::endl;
	//else 
	//	std::cout << cost << std::endl;

	for ( ListBpGraph::EdgeIt e(g); e!=INVALID; ++e )
		std::cout << node_names[g.u(e)] << " " << node_names[g.v(e)] << " " << sol[e] << std::endl;
	return 0;
}

int read_transportation(string input_file){
	std::ifstream kinput;
	kinput.open(input_file.c_str()); if (!kinput) return 0;
	kinput >> n1 >> n2 >> m;
	//g = ListBpGraph(m,n);	//red, blue
	std::map<string,ListBpGraph::RedNode> r_map;
	std::map<string,ListBpGraph::BlueNode> b_map;
	for ( int i=0; i<n1; i++ ){
		string tmp;
		int r;
		kinput >> tmp >> r;
		ListBpGraph::RedNode rn = g.addRedNode();
		node_names[rn] = tmp;
		v[rn] = r;
		r_map[tmp]=rn;
	}

	for ( int i=0; i<n2; i++ ){
		string tmp;
		int r;
		kinput >> tmp >> r;
		ListBpGraph::BlueNode bn = g.addBlueNode();
		node_names[bn] = tmp;
		v[bn] = r;
		b_map[tmp]=bn;
	}

	for ( int i=0; i<m; i++){
		string red, blue;
		int c_tmp;
		kinput >> red >> blue >> c_tmp;
		ListBpGraph::Edge e = g.addEdge(r_map[red], b_map[blue]);
		c[e] = c_tmp;
	}

	return 1;
}

int show_usage(){
	std::cout << "Uso: ./transportation.e -i <input_file> -t <max_time>"  << std::endl 
	     << "input_time eh o arquivo de entrada e max_time eh o tempo maximo de execucao." << std::endl
	     << "voce pode usar o argumento -v para imprimir a entrada e debug. " << std::endl;
	return 0;
}

int show_input(){
	ListBpGraph::EdgeMap<int> e_color(g,0);
	std::string fname("transportation");
	make_eps_graph(e_color, fname);
	return 0;
}

void show_graph_mygraphlib(std::string text){
	ListGraph gtmp;
	ListGraph::NodeMap<std::string> v_name(gtmp);
	ListGraph::NodeMap<std::string> v_label(gtmp);
	ListGraph::EdgeMap<std::string> e_name(gtmp);
	ListGraph::NodeMap<int> v_color(gtmp);
	ListGraph::EdgeMap<int> e_color(gtmp);
	map<ListBpGraph::Node, ListGraph::Node> ref;
	for ( ListBpGraph::RedNodeIt nr(g); nr!=INVALID; ++nr ) {
		ListGraph::Node n = gtmp.addNode();
		std::string label;
		label = node_names[nr] + "/" + std::to_string(v[nr]);
		v_name[n] = node_names[nr];
		v_label[n] = label;
		v_color[n]=3;
		ref[nr]=n;
	}
	for ( ListBpGraph::BlueNodeIt nb(g); nb!=INVALID; ++nb ) {
		ListGraph::Node n = gtmp.addNode();
		std::string label;
	       	label = node_names[nb] + "/" + std::to_string(v[nb]); 
		v_name[n] = node_names[nb];
		v_label[n] = label;
		v_color[n]=5;
		ref[nb]=n;
	}
	for ( ListBpGraph::EdgeIt eb(g); eb!=INVALID; ++eb ){

		ListGraph::Edge e = gtmp.addEdge( ref[g.u(eb)], ref[g.v(eb)] );
		//e_name[e] = sol[eb] + "-" + c[eb]; 
		e_name[e] = std::to_string(sol[eb]) + "/" + std::to_string(c[eb]); 
		if ( sol[eb] > 0 )
			e_color[e] = 4;
		else
			e_color[e] = 2;
	}
	/*GraphCopy<ListBpGraph, ListGraph> cg(g, gtmp);
	cg.run();*/

	//ViewListGraph(gtmp, v_name, e_name, v_color, e_color, text);
	ViewListGraph(gtmp, v_name, v_label, e_name, v_color, e_color, text);
}

int make_eps_graph(ListBpGraph::EdgeMap<int>& e_color, std::string name){
//	cout << "Input file: " << input_file;
	//std::cout << g.redNum() << " " << g.blueNum() << std::endl;
	ListGraph gtmp;
	ListGraph::NodeMap<int> sizes(gtmp);
	ListGraph::NodeMap<dim2::Point<double> >coords(gtmp);
	ListGraph::NodeMap<int> vcolor(gtmp);
	ListGraph::NodeMap<std::string> texts(gtmp);
	ListGraph::EdgeMap<int> ec(gtmp);

	map<ListBpGraph::Node, ListGraph::Node> ref;

	int x=0, y=0;
	for(ListBpGraph::RedNodeIt node(g); node!=INVALID; ++node){
		ListGraph::Node n = gtmp.addNode();
		coords[n]=dim2::Point<double>(x++,y);
		vcolor[n]=3;
		sizes[n]=2.5;
		texts[n]= node_names[node] + "/" + std::to_string(v[node]);
		ref[node] = n;
	}
	x=0; y=4;
	for(ListBpGraph::BlueNodeIt node(g); node!=INVALID; ++node){
		ListGraph::Node n = gtmp.addNode();
		coords[n]=dim2::Point<double>(x++,y);
		vcolor[n]=1;
		sizes[n]=2.5;
		texts[n]= node_names[node] + "/" + std::to_string(v[node]);
		ref[node] = n;
	}

	for(ListBpGraph::EdgeIt edge(g); edge!=INVALID; ++edge){
		ListGraph::Edge e = gtmp.addEdge(ref[g.u(edge)], ref[g.v(edge)]);
		
		double xt, yt;
		xt = ( coords[ref[g.u(edge)]].x + coords[ref[g.v(edge)]].x ) / 2;
		yt = ( coords[ref[g.u(edge)]].y + coords[ref[g.v(edge)]].y ) / 2;

		ListGraph::Node n = gtmp.addNode();
		coords[n] = dim2::Point<double>(xt, yt);
		vcolor[n] = 0;
		sizes[n] = 2;
		if ( e_color[edge] == 0 ){
			ec[e] = 0;
			texts[n] = "";
		}
		else{
			ec[e] = 2;
			texts[n] = std::to_string(sol[edge])+"/"+std::to_string(c[edge]);
		}
	}
		//ec[e]=(e_color[e]==0)?0:1;
	Palette palette;
	graphToEps(gtmp, name+".eps").nodeSizes(sizes).coords(coords).
		//nodeTexts(id).nodeTextSize(0.05).
		nodeTexts(texts).nodeTextSize(0.05).
		//nodeTexts(v).nodeTextSize(0.05).
		nodeColors(composeMap(palette, vcolor)).
		distantBWNodeTexts().
		//distantColorNodeTexts().
		//edgeColors(composeMap(palette, e_color)).
		edgeColors(composeMap(palette, ec)).
		scaleToA4().title(name).run();
	return 0;
}

///
// returns true if sol is feasible
bool is_feasible_solution(int & cost, int verbose){
	//For each terminal/router in the graph, I have to check if its requirement/capacity is fulfilled
	for ( ListBpGraph::NodeIt node(g); node!=INVALID; ++node ){
		int sum = 0;
		//Sum all the incident edges
		for ( ListBpGraph::IncEdgeIt e(g, node); e!=INVALID; ++e ){
			//if ( verbose  && sol[e]>0 )
			//	std::cout << g.id(g.u(e)) << " " << g.id(g.v(e)) << ":" << c[e] << "*" << sol[e] << std::endl;
			sum += sol[e];
			cost += sol[e]*c[e];
		}
		if ( sum != v[node] )
			return false;
	}
	cost/=2;
	if ( verbose )
		for ( ListBpGraph::EdgeIt e(g); e!=INVALID; ++e ){
			if ( sol[e] > 0 )
				std::cout << g.id(g.u(e)) << " " << g.id(g.v(e)) << ":" << c[e] << "*" << sol[e] << std::endl;
		}
	return true;
}
