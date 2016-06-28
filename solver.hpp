#include "nsp.hpp"
#include <vector>
#include <iostream>
#include <algorithm>
#include <ctime>
#include <unistd.h>

#ifndef _SOLVER_HPP_INCLUDED_
#define _SOLVER_HPP_INCLUDED_

using namespace std;

#define D_TYPE vector<vector<NUMBER>>

class Solver {
	private:
		int iterCount;
		int solutionCount;
		int bestScore;
		void printCandidate(vector<NUMBER>&);
		void parseAndPrintCandidate(vector<NUMBER>&);
		bool mfc(D_TYPE::iterator, D_TYPE::iterator);
		void rollbackMfc(D_TYPE::iterator, D_TYPE::iterator);
		bool satisfiesRestrictions(NUMBER, int);
		void search(D_TYPE::iterator, clock_t, pid_t);
		int evaluate();
	public:
		NSP *problem; // Puntero a una instancia de la clase NSP.
		D_TYPE domains; // Lista con todos los dominios.
		vector<NUMBER> candidate; // Solucion parcial.
		D_TYPE solutions; // Contiene todas las soluciones factibles.

		Solver(NSP&);
		void solve();
		
};

#endif