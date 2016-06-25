#include "nsp.hpp"
#include <vector>
#include <iostream>
#include <algorithm>

#ifndef _SOLVER_HPP_INCLUDED
#define _SOLVER_HPP_INCLUDED

#define D_TYPE vector<vector<NUMBER>>

class Solver {
	private:
		int iterCount;
		void printCandidate(vector<NUMBER>&);
		void parseAndPrintCandidate(vector<NUMBER>&);
		bool mfc(D_TYPE::iterator, D_TYPE::iterator);
		void rollbackMfc(D_TYPE::iterator, D_TYPE::iterator);
		bool satisfiesRestrictions(NUMBER, int);
		bool search(D_TYPE::iterator, NUMBER level);
	public:
		NSP *problem; // Puntero a una instancia de la clase NSP.
		D_TYPE domains; // Lista con todos los dominios.
		vector<NUMBER> candidate; // Solucion parcial.
		D_TYPE solutions; // Contiene todas las soluciones factibles.

		Solver(NSP&);
		void solve();
		
};

#endif