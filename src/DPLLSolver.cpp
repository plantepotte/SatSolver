//
// Created by potteplante on 28.05.25.
//

#include "../include/DPLLSolver.h"
#include <algorithm>
#include <iostream>
#include <fstream>

namespace SatSolver {
    DPLLSolver::DPLLSolver(const std::string &filename) {
        ClauseSetFromFile(filename);
    }

    DPLLSolver::DPLLSolver(const std::vector<std::vector<int> > &clauses) {
        _clauses = clauses;
        _metadata = FormulaMetadata{_clauses.empty() ? 0 : clauses[0].size(), clauses.size()};
    }

    bool DPLLSolver::Solve() {
        return DPLL();
    }

    bool DPLLSolver::ClauseSetFromFile(const std::string &inFileName) {
        std::ifstream inFile{inFileName};
        if (!inFile.is_open()) {
            std::cout << "Failed to open file: " + inFileName << '\n';
            return false;
        }
        size_t numVariables{}, numClauses{};

        for (int i = 0; i < 2; ++i) {
            std::string parsedWord;
            inFile >> parsedWord; // skip header
        }

        inFile >> numVariables >> numClauses;
        std::cout << "variables: " << numVariables << " clauses: " << numClauses;

        // allocate space for the clauses:
        _clauses = std::vector<std::vector<int> >(numClauses, std::vector<int>{});

        for (int i = 0; i < numClauses; ++i) {
            int parsedLiteral;
            inFile >> parsedLiteral; // read literal from file

            while (parsedLiteral != 0) // if not end-of-clause add literal to clause
            {
                _clauses[i].emplace_back(parsedLiteral);
                inFile >> parsedLiteral; // read next literal
            }
        }
        _metadata = FormulaMetadata{numVariables, numClauses};
        return true;
    }

    void DPLLSolver::UnitPropagation(int litToPropagate) {
        std::erase_if( // removes all clauses c that the predicate function evaluates to true.
            _clauses,
            [litToPropagate](const std::vector<int> &c) // unary predicate function
            {
                // returns true if-clause c contains the given literal, and false otherwise
                return std::ranges::find(c, litToPropagate) != c.end();
            }
        );

        for (auto &clause: _clauses) // unit resolution:
        {
            // removes all instances of the negation of the given literal from each clause
            std::erase(clause, -litToPropagate);
        }

        if (litToPropagate < 0) // if given literal was ~p, then p is assigned false.
        {
            _assignment[-litToPropagate] = false;
        } else // if given literal was p, then p is assigned true.
        {
            _assignment[litToPropagate] = true;
        }
    }

    void DPLLSolver::GetIterToUnitClause(std::vector<std::vector<int> >::iterator &clauseIter) {
        clauseIter = std::ranges::find_if(_clauses, [](const std::vector<int> &c) { return c.size() == 1; });
    }

    bool DPLLSolver::DPLL() {
        if (_clauses.empty()) // empty clause-set is trivially satisfiable.
        {
            return true;
        }

        // retrieve pointer to unit-clause:
        std::vector<std::vector<int> >::iterator clauseIter;
        GetIterToUnitClause(clauseIter);

        while (clauseIter != _clauses.end()) // if unit-clause was found attempt to resolve empty clause, or propagate.
        {
            int currentLit = (*clauseIter)[0]; // literal in first selected unit-clause
            auto negatedIter = std::ranges::find_if(_clauses,
                                                    [currentLit](const std::vector<int> &c) {
                                                        return c.size() == 1 && c[0] == -currentLit;
                                                        // looks for corresponding negated unit clause
                                                    }
            );

            if (negatedIter != _clauses.end()) // if both {p} and {~p} was found, return unsatisfiable.
            {
                return false;
            }

            // if no {p} and {~p} pair was found, choose assignment so unit-clause is true, and propagate through clauses.
            UnitPropagation(currentLit);
            GetIterToUnitClause(clauseIter); // look for new unit-clause.
        }

        if (_clauses.empty()) // empty clause-set is trivially satisfiable.
        {
            return true;
        }

        // sort clauses by number of literals, ascending order
        std::ranges::sort(_clauses,
                          [](const std::vector<int> &c1, const std::vector<int> &c2) { return c1.size() < c2.size(); });

        // perform atomic cut with random literal from clause with the least amount of literals
        std::uniform_int_distribution<size_t> dist{0, _clauses[0].size() - 1};
        const int litToCut = _clauses[0][dist(_generator)];

        const std::vector<std::vector<int> > newClauseSet = _clauses; // copy to continue dpll on
        UnitPropagation(litToCut); // perform unit propagation with cut literal
        if (DPLL()) {
            return true;
        }

        // if previous branch was unsatisfiable, try branch with negation of cut literal instead.
        _clauses = newClauseSet;
        UnitPropagation(-litToCut);

        return DPLL(); // return satisfiability of second branch
    }

    std::ostream &operator<<(std::ostream &os, const DPLLSolver &satSolver) {
        for (const auto &clause: satSolver._clauses) {
            os << '\n';
            for (const int lit: clause) {
                os << lit << " ";
            }
        }
        os << '\n';
        return os;
    }
} // DPLL
