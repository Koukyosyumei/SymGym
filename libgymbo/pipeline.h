#include <unistd.h>

#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "compiler.h"
#include "gd.h"
#include "parser.h"
#include "tokenizer.h"
#include "type.h"

namespace gymbo {

/**
 * @brief Compiles user input into a program, returning variable counts and the
 * compiled program.
 *
 * This function takes a user-provided input in the form of a character array
 * and performs the following steps:
 * 1. Tokenizes the input, counting occurrences of variables using var_counter.
 * 2. Generates an Abstract Syntax Tree (AST) from the tokenized input.
 * 3. Compiles the AST into a program using gymbo::Node objects.
 *
 * @param user_input A character array representing the user-provided input.
 *
 * @return A std::pair containing:
 *   - First element: An std::unordered_map<std::string, int> representing
 * variable counts.
 *   - Second element: A Prog object representing the compiled program.
 *
 * @see tokenize() Function used for tokenization.
 * @see generate_ast() Function used for AST generation.
 * @see compile_ast() Function used for compiling the AST into a program.
 */
inline std::pair<std::unordered_map<std::string, int>, Prog> gcompile(
    char *user_input) {
    std::unordered_map<std::string, int> var_counter;
    std::vector<gymbo::Node *> code;
    Prog prg;

    Token *token = tokenize(user_input, var_counter);
    generate_ast(token, user_input, code);
    compile_ast(code, prg);

    return std::make_pair(var_counter, prg);
}

/**
 * @brief Symbolically Execute a Program with Gradient Descent Optimization.
 *
 * This function conducts symbolic execution of a given program while
 * simultaneously optimizing the path constraints using the provided gradient
 * descent optimizer, `GDOptimizer`.
 *
 * @param prog The program to symbolically execute.
 * @param optimizer The gradient descent optimizer for parameter optimization.
 * @param state The initial symbolic state of the program.
 * @param target_pcs The set of program counters where gymbo executes
 * path-constraints solving. If this set is empty or contains -1, gymbo solves
 * all path-constraints.
 * @param maxDepth The maximum depth of symbolic exploration.
 * @param maxSAT The maximum number of SAT constraints to collect.
 * @param maxUNSAT The maximum number of UNSAT constraints to collect.
 * @param max_num_trials The maximum number of trials for each gradient descent.
 * @param ignore_memory If set to true, constraints derived from memory will be
 * ignored.
 * @param use_dpll If set to true, use DPLL to decide the initial assignment for
 * each term.
 * @param verbose_level The level of verbosity.
 * @return A set of extracted path constraints and their satisfiability.
 */
inline PathConstraintsTable gexecute(Prog &prg, GDOptimizer &optimizer,
                                     SymState &init,
                                     std::unordered_set<int> &target_pcs,
                                     int max_depth, int maxSAT, int maxUNSAT,
                                     int max_num_trials, bool ignore_memory,
                                     bool use_dpll, int verbose_level) {
    PathConstraintsTable cache_constraints;

    run_gymbo(prg, optimizer, init, target_pcs, cache_constraints, max_depth,
              maxSAT, maxUNSAT, max_num_trials, ignore_memory, use_dpll,
              verbose_level);

    return cache_constraints;
}

}  // namespace gymbo
