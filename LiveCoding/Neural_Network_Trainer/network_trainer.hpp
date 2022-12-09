
#include <iostream>
#include <vector>
#include <cmath>

#include "../Neural_Network/neural_network.hpp"

using namespace std;

class trainer
{
    bool debug = false;
    int max_rounds = 5000;
    vector <int> nodes_per_layer = {4,8,4};
    int population_count = 10;
    double error_tol = 0.1;
    
    vector <network> population;
    vector <double> inputs;
    vector <double> expected_outputs;
    vector <double> errors; // avg error, one for each population
    int rounds;
    double starting_avg_error;
    double best_error;
    double avg_error;
    vector <vector <vector <double> > > best_weights;
    
    void print_info();
    int train_vals(int);
    void generate_values();
    void check_pop_error();
    void prune();
    void populate();
    void cycle_network();
public:
    void setup();
    void train();
    // note: I am using the code (below) from the main.cpp file that ran the neural network before
};

void trainer::print_info()
{
    cout << endl;
    cout << "starting avg error: " << starting_avg_error << endl;
    cout << "avg pop error: " << avg_error << endl;
    cout << "best error: " << best_error << endl;
}

// this is for x^2
int trainer::train_vals(int in)
{
    return pow(in,2);
}

void trainer::generate_values()
{
    inputs.clear();
    expected_outputs.clear();
    for (int a=0; a<nodes_per_layer.at(0); a++) {
        inputs.push_back(a+1);//+nodes_per_layer.at(0)*rounds);
    }
    int val = nodes_per_layer.at(nodes_per_layer.size()-1);
    for (int a=0; a<val; a++) {
        expected_outputs.push_back(train_vals(a+1));//+val*rounds));
    }
}

void trainer::check_pop_error()
{
    double temp = 0.0;
    for (int p=0; p<population.size(); p++) {
        double e = 0.0;
        for (int i=0; i<population.at(p).outputs.size(); i++) {
            e += expected_outputs.at(i) - population.at(p).outputs.at(i);
        } errors.push_back(e);
        if (abs(e)<best_error) {
            best_error = e;
            best_weights = population.at(p).export_weights();
        }
        temp += e;
    } temp = temp/population.size();
    avg_error = temp;
    if (rounds == 0) starting_avg_error = temp;
}

void trainer::prune()
{
    // remove worst weights (like half)
    double min_val = HUGE_VAL;
    double max_val = -HUGE_VAL;
    for (int a=0; a<errors.size(); a++) {
        if (errors.at(a) > max_val) max_val = errors.at(a);
        if (errors.at(a) < min_val) min_val = errors.at(a);
    }
    double keep_percent = 0.5;
    double keep_error = min_val + keep_percent*(max_val-min_val);
    vector <network> temp;
    for (int p=0; p<population.size(); p++) {
        if (errors.at(p) < keep_error) temp.push_back(population.at(p));
    }
    population.clear();
    population = temp;
}

void trainer::populate()
{
    // refill population with mutation of a random remaining population
    if (debug) cout << "creating populatoin from: " << population.size() << endl;
    while (population.size() < population_count) {
        int i = rand() % population.size();
        vector <vector <vector <double> > > temp = population.at(i).export_weights();
        network net;
        net.setup(nodes_per_layer);
        net.import_weights(temp);
        net.set_values(inputs);
        int run = 0;
        int max_run = 10;
        double new_error = errors.at(i);
        while (new_error < errors.at(i) && run < max_run) {
            net.mutate();
            net.cycle_network();
            double e = 0.0;
            for (int a=0; a<expected_outputs.size(); a++) {
                e = expected_outputs.at(a) - net.outputs.at(a);
            } e = e/expected_outputs.size();
            new_error = e;
            run++;
        }
        population.push_back(net);
    }
}

void trainer::cycle_network()
{
    for (int p=0; p<population.size(); p++) {
        population.at(p).set_values(inputs);
        population.at(p).cycle_network();
    }
}

void trainer::setup()
{
    srand(time(NULL));
    rounds = 0;
    best_error = HUGE_VAL;
    // create starting population
    for (int p=0; p<population_count; p++) {
        network net;
        net.setup(nodes_per_layer);
        population.push_back(net);
    }
}

void trainer::train()
{
    while (abs(best_error) > error_tol && rounds<max_rounds) {
        generate_values();
        cycle_network();
        check_pop_error();
        prune();
        populate();
        rounds++;
    }
    print_info();
}