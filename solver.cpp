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
			if(this->satisfiesRestrictions(*j, distance(this->domains.begin(), i) - 1)) {
				break; // MFC detiene el podado al primer valor factible
			} else {
				k->push_back(*j); // Guarda el valor en la estructura de respaldo
				j = i->erase(j); // Elimina el valor del dominio actual
			}
		}
		if(i->size() == 0) {
			OK = false; // Si un dominio queda vacío se debe hacer backtrack.
			break;
		} else {
			k++;
		}
	}
	return OK;
}

void Solver::rollbackMfc(D_TYPE::iterator domains, D_TYPE::iterator filteredValues) {
	D_TYPE::iterator k = filteredValues;

	for (D_TYPE::iterator i = domains; i != this->domains.end(); ++i)
	{
		if (k->size() == 0) continue;
		for (vector<NUMBER>::iterator j = k->begin(); j != k->end(); ++j)
		{
			i->push_back(*j); // Inserta el valor filtrado nuevamente al dominio
		}
		sort(i->begin(), i->end());// Ordena los valores del dominio a su forma inicial (secuencial)
		k++;
	}
}

bool Solver::satisfiesRestrictions(NUMBER value, int pos) {
	// Restricciones de Cobertura
	vector<NUMBER> nurseCountPerTurn = vector<NUMBER>(this->problem->S, 0);
	nurseCountPerTurn[value-1]++;
	int targetDay = pos%this->problem->D;
	for (NUMBER i = targetDay; i < this->candidate.size(); i+=this->problem->D)
	{
		nurseCountPerTurn[this->candidate[i] - 1]++;
	}
	int assignmentsLeft = this->problem->N - (int)(this->candidate.size()-targetDay)/this->problem->N;
	for (NUMBER j = 0; j < this->problem->S; ++j)
	{
		if(assignmentsLeft < this->problem->demand[targetDay][j] - nurseCountPerTurn[j]) {
			return false;
		}
	}
	return true;
}

/*	
 *	Realiza la búsqueda de soluciones para el NSP dado. 
 *	Retorna false si no se encontraron solucion, true en caso contrario.
 *
 *	node (D_TYPE::iterator) : posición la lista de dominios. 
 */
bool Solver::search(D_TYPE::iterator node, NUMBER level) {
	if((NUMBER)this->candidate.size() == this->problem->N*this->problem->D) {
		if (this->iterCount<1) {
			this->printCandidate(this->candidate);
		}
		this->iterCount++;
		//vector<NUMBER> tmp = this->candidate;
		//this->solutions.push_back(tmp);
		return false;
	}

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

/*	
 *	Inicia la búsqueda de soluciones desde la raíz.
 */
void Solver::solve() {
	this->search(this->domains.begin(), 0);
}
