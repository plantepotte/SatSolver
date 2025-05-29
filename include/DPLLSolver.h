//
// Created by potteplante on 28.05.25.
//

#ifndef SOLVER_H
#define SOLVER_H
#include <map>
#include <vector>
#include <iostream>
#include <random>
#include <unordered_map>

namespace SatSolver {
    struct FormulaMetadata
    {
        size_t numVariables;
        size_t numClauses;
    };

    class DPLLSolver {
    public:
        DPLLSolver() = default;
        DPLLSolver(const DPLLSolver &) = default;
        DPLLSolver(DPLLSolver &&)  noexcept = default;

        /**
         * Constructor that loads clause-set from a .txt file in the DIMACS format.
         * @param filename path and name (with extension) of DIMACS text-file containing clause-set to operate on.
         */
        explicit DPLLSolver(const std::string &filename);

        /**
         * Constructor that takes in a clause-set as a vector of vectors of ints.
         * The sub-vectors each represents a clause with each int representing a distinct literal.
         * @param clauses a vector of vectors of ints, representing the clause-set to operate on.
         */
        explicit DPLLSolver(const std::vector<std::vector<int>>& clauses);

        ~DPLLSolver() = default;

        DPLLSolver &operator=(const DPLLSolver &) = default;
        DPLLSolver &operator=(DPLLSolver &&) = default;

        /**
         * Solve the set. Returns true if there is a satisfying interpretation,
         * or false if the set is unsatisfiable.
         * If true, the satisfying interpretation can be retrieved
         * with the GetAssignment member-method.
         * @return The satisfiability of the starting clause-set.
         */
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

        /**
         * Returns a wrapper for the number of atomic variables, and clauses in the clause-set.
         * @return A metadata structure.
         */
        [[nodiscard]] const FormulaMetadata& GetMetadata() const { return _metadata; }

        /**
         * Returns the currently stored interpretation as a map
         * where the key is an int representing an atomic variable,
         * and the corresponding value is said variable's currently assigned truth-value.
         * @return A map with the assignment of truth-values.
         */
        [[nodiscard]] const std::map<int, bool>& GetAssignment() const { return _assignment; }

    private:
        std::vector<std::vector<int>> _clauses{};
        std::map<int, bool> _assignment{};

        FormulaMetadata _metadata{};

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
        void GetIterToUnitClause(std::vector<std::vector<int>>::iterator &clauseIter);

        /**
         * My attempt at implementing DPLL for propositional formulas.
         * @return Returns true if satisfiable, or false if unsatisfiable.
         */
        bool DPLL();
    };

} // DPLL

#endif //SOLVER_H
