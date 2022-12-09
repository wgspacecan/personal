
/*
TODO:
    - import network - add output node count to weights info or a network output
    - create trainer
*/

#include <iostream>

#include "neural_network.hpp"

using namespace std;

int max_rounds = 4;
vector <int> nodes_per_layer = {4,8,4};

vector <double> inputs;
vector <double> outputs;
vector <double> expected_outputs;

// this is for x^2
int train_vals(int in)
{
    return pow(in,2);
}

void generate_values()
{
    inputs.clear();
    expected_outputs.clear();
    for (int a=0; a<nodes_per_layer.at(0); a++) {
        inputs.push_back(a+1);
    }
    for (int a=0; a<nodes_per_layer.at(nodes_per_layer.size()-1); a++) {
        expected_outputs.push_back(train_vals(a+1));
    }
}

void print_error()
{
    double avg_error = 0;
    for (int a=0; a<outputs.size(); a++) {
        cout << "- out " << setprecision(15) << a << ": " << outputs.at(a) << endl;
        avg_error += expected_outputs.at(a) - outputs.at(a);
    } avg_error = avg_error/outputs.size();
    cout << "Average Error: " << avg_error << endl;
    cout << endl;
}

int main()
{
    srand(time(NULL));
    
    network net;
    net.setup(nodes_per_layer);
    //net.weights_info("weights.txt");
    net.print_info();
    generate_values();
    net.set_values(inputs);
    //net.import_weights_from_file("");
    
    int run = 1;
    while (run <= max_rounds) {
        net.mutate();
        cout << "cycle: " << run << endl;
        net.cycle_network();
        outputs = net.outputs;
        //net.export_weights_to_file();
        print_error();
        run++;
    }
    
    return 0;
}