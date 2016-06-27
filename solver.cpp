#include "solver.hpp"

Solver::Solver(NSP &params)
: iterCount(0)
, solutionCount(0)
, bestScore(0)
, problem(&params)
, domains(problem->N*problem->D, vector<NUMBER>(problem->S))
, candidate()
, solutions()
{
	for (int i = 0; i < this->problem->N*this->problem->D; ++i)
	{
		for (NUMBER j = 0; j < this->problem->S; ++j)
		{
			this->domains[i][j] = this->problem->S - j;
		}
	}
}

void Solver::printCandidate(vector<NUMBER>& candidate) {
	for (vector<NUMBER>::iterator it = candidate.begin(); it != candidate.end(); ++it)
	{
		if(distance(candidate.begin(), it)%this->problem->N == 0) {
			cout << endl;
		}
		cout << *it << ' ';
	}
	cout << endl;
}

void Solver::parseAndPrintCandidate(vector<NUMBER>& candidate) {
	for (vector<NUMBER>::iterator it = candidate.begin(); it != candidate.end(); ++it)
	{
		if(distance(candidate.begin(), it)%this->problem->N == 0) {
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
			if(this->satisfiesRestrictions(*j, distance(this->domains.begin(), i))) {
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
			k = next(k);
		}
	}
	return OK;
}

void Solver::rollbackMfc(D_TYPE::iterator domains, D_TYPE::iterator filteredValues) {

	D_TYPE::iterator k = filteredValues;
	for (D_TYPE::iterator i = domains; i != this->domains.end(); ++i)
	{
		if (k->size() == 0) {
			k = next(k);
			continue;
		}
		for (vector<NUMBER>::iterator j = k->begin(); j != k->end(); ++j)
		{
			i->push_back(*j); // Inserta el valor filtrado nuevamente al dominio
		}
		//sort(i->begin(), i->end(), desc_comparator);// Ordena los valores del dominio a su forma inicial (secuencial)
		k = next(k);
	}
}

bool Solver::satisfiesRestrictions(NUMBER value, int pos) {

	// Restricciones extra.
	int targetNurse = pos%this->problem->N;
	int workingShifts, assignmentsLeft;
	if(value == this->problem->S) {
		workingShifts = 0;
	} else {
		workingShifts = 1;
	}
	assignmentsLeft = this->problem->D - 1;
	int consecCount = 0;

	vector<NUMBER> consecSameShift = vector<NUMBER>(this->problem->S, 0);
	vector<NUMBER> sameShiftCount = vector<NUMBER>(this->problem->S, 0);
	for (NUMBER i = targetNurse; i < this->candidate.size(); i+=this->problem->N)
	{
		assignmentsLeft--;
		if(this->candidate[i] != this->problem->S) {
			workingShifts++; // Contar dias trabajados.
			consecCount++; // Contar dias consecutivos trabajados.
		} else {
			consecCount = 0; // Cuando ocurre un libre se reincia el contador.
		}

	}
	if(consecCount != 0) { // Si el ultimo dia fue libre, no Revisa.
		if(value != this->problem->S) {
			consecCount++;
		} else {
			if(assignmentsLeft < this->problem->minConsec) {
				return false;
			}
		}
		// Revisa min/max dias consecutivos trabajados.
		if(consecCount != 0 && (this->problem->minConsec > consecCount || consecCount > this->problem->maxConsec)) {
			return false;
		}
	}
	// Revisa min/max días trabajados.
	if (workingShifts > this->problem->maxAssign || assignmentsLeft < this->problem->minAssign - workingShifts) {
		return false;
	}

	// Restriccion de Cobertura
	vector<NUMBER> nurseCountPerShift = vector<NUMBER>(this->problem->S, 0);
	nurseCountPerShift[value-1]++;
	int targetDay = pos/this->problem->N;
	int startingPos = targetDay*this->problem->N;
	assignmentsLeft = this->problem->N - 1;
	for (NUMBER i = startingPos; i < startingPos+this->problem->N && i < this->candidate.size(); ++i)
	{
		assignmentsLeft--;
		nurseCountPerShift[this->candidate[i] - 1]++;
	}
	for (NUMBER j = 0; j < this->problem->S; ++j)
	{	
		// Revisa si se cumple la cobertura minima.
		if(assignmentsLeft < this->problem->demand[targetDay][j] - nurseCountPerShift[j]) {
			return false;
		}
	}
	// Si se cumplen todas las restricciones, retornar true.
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
		//cout << this->iterCount << " Score: " << this->evaluate(this->candidate) << endl;
		this->printCandidate(this->candidate);
		this->solutionCount++;
		int score = this->evaluate();
		if(score > this->bestScore) bestScore = score;
		//vector<NUMBER> tmp = this->candidate;
		//this->solutions.push_back(tmp);
		return false;
	}
	this->iterCount++;
	bool check = false;
	for (vector<NUMBER>::iterator it = node->begin(); it != node->end(); ++it)
	{	
		// Revisa las restricciones para los valores del dominio no podados por el MFC.
		if(check && !this->satisfiesRestrictions(*it, distance(this->domains.begin(), node))) {
			continue;
		}
		// Almacena los valores filtrados de cada dominio
		int remainingDomainsCount = distance(next(node), this->domains.end());
		D_TYPE filteredValues = D_TYPE(remainingDomainsCount, vector<NUMBER>(0));
		this->candidate.push_back(*it); // Instancia la variable
		if (remainingDomainsCount == 0 || this->mfc(next(node), filteredValues.begin())) { // Propaga restricciones
			this->search(next(node), level + 1); // Continua con la búsqueda		
		}
		if (remainingDomainsCount != 0) this->rollbackMfc(next(node), filteredValues.begin()); // Restaura los dominios filtrados
		this->candidate.pop_back(); // Remueve la variable instanciada
		check = true;
	}
	return false;
}

int Solver::evaluate() {
	int score = 0;
	for (int i = 0; i < this->problem->N; ++i)
	{
		for (int j = 0; j < this->problem->D; ++j)
		{
			score += (this->problem->S - this->problem->preferences[i][j*this->problem->S+this->candidate[i+j*this->problem->N]-1]);
		}
	}
	return score;
}

/*	
 *	Inicia la búsqueda de soluciones desde la raíz.
 */
void Solver::solve() {
	this->search(this->domains.begin(), 0);
	cout << endl << "Soluciones encontradas: " << this->solutionCount << endl << "Mejor puntaje: " << this->bestScore << endl;
}
