#include "solver.hpp"

Solver::Solver(NSP &params)
: iterCount(0)
, problem(&params)
, domains(problem->N*problem->D, vector<NUMBER>(problem->S))
, candidate()
, solutions()
{
	for (int i = 0; i < this->problem->N*this->problem->D; ++i)
	{
		for (NUMBER j = 0; j < this->problem->S; ++j)
		{
			this->domains[i][j] = j+1;
		}
	}
}

void Solver::printCandidate(vector<NUMBER>& candidate) {
	for (vector<NUMBER>::iterator it = candidate.begin(); it != candidate.end(); ++it)
	{
		if(distance(candidate.begin(), it)%this->problem->D == 0) {
			cout << endl;
		}
		cout << *it << ' ';
	}
	cout << endl;
}

void Solver::parseAndPrintCandidate(vector<NUMBER>& candidate) {
	for (vector<NUMBER>::iterator it = candidate.begin(); it != candidate.end(); ++it)
	{
		if(distance(candidate.begin(), it)%this->problem->D == 0) {
			cout << endl;
		}
		switch (*it) {
			case 1: 
				cout << "Día\t";
				break;
			case 2:
				cout << "Tarde\t";
				break;
			case 3:
				cout << "Noche\t";
				break;
			default:
				cout << "Libre\t";
		}
	}
}

bool Solver::mfc(D_TYPE::iterator domains, D_TYPE::iterator filteredValues) {
	bool OK = true;
	D_TYPE::iterator k = filteredValues;

	for (D_TYPE::iterator i = domains; i != this->domains.end(); ++i)
	{
		vector<NUMBER>::iterator j = i->begin();
		while (j != i->end())
		{
			this->candidate.push_back(*j);

			if(this->satisfiesRestrictions()) {
				OK = true;
			} else {
				k->push_back(*j);
				j = i->erase(j);
			}

			this->candidate.pop_back();
			if (OK) break; // MFC detiene el podado al primer valor factible.
		}
		if(i->size() == 0) {
			OK = false; // Si un dominio queda vacío se debe hacer backtrack.
			break;
		} else {
			k = next(k);
		}
	}

	return OK;
}

void Solver::rollbackMfc(D_TYPE::iterator domains, D_TYPE::iterator filteredValues) {

}

bool Solver::satisfiesRestrictions() {
	return true;
}

bool Solver::search(D_TYPE::iterator node, NUMBER level) {
	if((NUMBER)this->candidate.size() == this->problem->N*this->problem->D) {
		//this->printCandidate(this->candidate);
		vector<NUMBER> tmp = this->candidate;
		this->solutions.push_back(tmp);
		return false;
	}

	this->iterCount++;

	for (vector<NUMBER>::iterator it = node->begin(); it != node->end(); ++it)
	{	
		// Almacena los valores filtrados de cada dominio
		int remainingDomainsCount = distance(next(node), this->domains.end());
		D_TYPE filteredValues = D_TYPE(remainingDomainsCount, vector<NUMBER>(0));

		this->candidate.push_back(*it); // Instancia la variable

		if (remainingDomainsCount == 0 || this->mfc(next(node), filteredValues.begin())) { // Propaga restricciones
			this->search(next(node), level + 1); // Continua con la búsqueda
		}

		if (remainingDomainsCount != 0) this->rollbackMfc(next(node), filteredValues.begin()); // Restaura los dominios filtrados
		this->candidate.pop_back(); // Remueve la variable instanciada
	}

	return false;
}

void Solver::solve() {
	this->search(this->domains.begin(), 0);
}
