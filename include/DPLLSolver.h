//
// Created by potteplante on 28.05.25.
//

#ifndef SOLVER_H
#define SOLVER_H
#include <map>
#include <vector>
#include <iostream>
#include <random>

namespace SatSolver {
    struct formulaMetadata
    {
        int numVariables;
        int numClauses;
    };

    class DPLLSolver {
    public:
        DPLLSolver() = default;
        DPLLSolver(const DPLLSolver &) = default;
        DPLLSolver(DPLLSolver &&)  noexcept = default;

        explicit DPLLSolver(const std::string &filename);
        explicit DPLLSolver(const std::vector<std::vector<int>>& clauses);

        ~DPLLSolver() = default;

        DPLLSolver &operator=(const DPLLSolver &) = default;
        DPLLSolver &operator=(DPLLSolver &&) = default;

        bool Solve();

        /**
         * Get text representation of clause set in DIMACS format
         * @param os output stream to insert into
         * @param satSolver satisfiability solver object
         * @return modified output stream
         */
        friend std::ostream& operator<<(std::ostream& os, const DPLLSolver& satSolver);

        /**
        * Function to read clause-set from text-file in DIMACS format.
        * @param inFileName
        * @return number of literals and clauses in a struct
        */
        bool ClauseSetFromFile(const std::string& inFileName);

        const formulaMetadata& GetMetadata() const { return _metadata; }
        const std::map<int, bool>& GetAssignment() const { return _assignment; }

    private:
        std::vector<std::vector<int>> _clauses{};
        std::map<int, bool> _assignment{};

        formulaMetadata _metadata{};

        std::minstd_rand _generator{std::random_device{}()};

        /**
         * Performs unit propagation of the given literal on the given clause set.
         * The atomic formula corresponding to the given literal is assigned the corresponding truth-value.
         * @param litToPropagate literal to propagate
         */
        void UnitPropagation(int litToPropagate);

        /**
         * Function that finds a unit clause in the given clause-set.
         * @return iterator that points to first found unit-clause.
         */
        std::vector<std::vector<int>>::iterator GetIterToUnitClause();

        /**
         * My attempt at implementing DPLL for propositional formulas.
         * @return Returns true if satisfiable, or false if unsatisfiable.
         */
        bool DPLL();
    };

} // DPLL

#endif //SOLVER_H
