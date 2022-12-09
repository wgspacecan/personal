
// Neural Network

#include <iostream>
#include <vector>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <iomanip>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define version 2.1 // version

#define LYRAND (double)rand()/RAND_MAX

using namespace std;

class node;
class layer;
class network;

//----------------------------------------
class node
{
    friend class layer;
    friend class network;
    
    double mutate_mod = 0.1;
    
public:
    vector <double> inputs;
    vector <double> weights;
    
    void mutate();
};

void node::mutate()
{
    for (int a=0; a<weights.size(); a++) {
        if (LYRAND>0.5) weights.at(a) = weights.at(a) * LYRAND * mutate_mod;
        else weights.at(a) = weights.at(a) * LYRAND * mutate_mod * -1;
    }
}

//----------------------------------------
class layer
{
    friend class node;
    friend class network;
    
public:
    vector <node> nodes;
};

//----------------------------------------
class network
{
    friend class node;
    friend class layer;
    
    bool debug = false;
    int cycle;
    
    vector <double> generate_weights(int);
    void create_network();
    
public:
    vector <double> inputs;
    vector <double> outputs;
    vector <layer> layers;
    vector <int> node_count;
    
    void print_info();
    void weights_info(string);
    
    vector <vector <vector <double> > > parse_input_file(string);
    vector <vector <vector <double> > > export_weights();
    void import_weights(vector <vector <vector <double> > >);
    
    void export_weights_to_file();
    void import_weights_from_file(string);
    
    void setup(vector <int>);
    void set_values(vector <double>);
    void mutate();
    void cycle_network();
};

void network::print_info()
{
    cout << endl;
    cout << "------------------------------" << endl;
    cout << endl;
    cout << "Neural Network" << endl;
    cout << "version " << version << endl;
    cout << endl;
    cout << "Input nodes: " << node_count.at(0) << endl;
    cout << "Output nodes: " << node_count.at(node_count.size()-1) << endl;
    cout << "Hidden layers: " << node_count.size()-2 << endl;
    cout << endl;
    cout << "------------------------------" << endl;
    cout << endl;
}

vector <vector <vector <double> > > network::parse_input_file(string file_name)
{
    if (file_name == "") file_name = "weights.txt";
    ifstream file;
    file.open(file_name, ios::in);
    string line;
    vector <vector <vector <double> > > temp;
    vector <vector <double> > t2;
    int l=0;
    int n=0;
    while (getline(file, line)) {
        int pos = 0;
        string token;
        if (line.empty()) {
            l++;
            temp.push_back(t2);
            t2.clear();
        }
        else {
            vector <double> t1;
            while ((pos = line.find(",")) != string::npos) {
                token = line.substr(0, pos);
                line.erase(0, pos+1);
             t1.push_back(stod(token));
            }
            t2.push_back(t1);
            n++;
        }
    }
    temp.push_back(t2);
    file.close();
    return temp;
}

void network::weights_info(string file_name)
{
    cout << endl;
    cout << "weight file info" << endl;
    vector <vector <vector <double> > > temp = parse_input_file(file_name);
    for (int l=0; l<temp.size(); l++) {
        cout << "nodes: " << temp.at(l).size() << endl;
    }
}

vector <vector <vector <double> > > network::export_weights()
{
    vector <vector <vector <double> > > temp;
    for (int l=0; l<layers.size()-1; l++) { // no weights on output
        vector <vector <double> > t1;
        for (int n=0; n<layers.at(l).nodes.size(); n++) {
            t1.push_back(layers.at(l).nodes.at(n).weights);
        } temp.push_back(t1);
    }
    return temp;
}


void network::import_weights(vector <vector <vector <double> > > given_weights)
{
    for (int l=0; l<layers.size()-1; l++) {
        for (int n=0; n<layers.at(l).nodes.size(); n++) {
            layers.at(l).nodes.at(n).weights = given_weights.at(l).at(n);
        }
    }
}

void network::export_weights_to_file()
{
    vector <vector <vector <double> > > temp = export_weights();
    ofstream file;
    file.open("weights.txt", ios::out | ios::trunc);
    for (int l=0; l<temp.size(); l++) {
        for (int n=0; n<temp.at(l).size(); n++) {
            for (int w=0; w<temp.at(l).at(n).size(); w++) {
                file << setprecision(15) << temp.at(l).at(n).at(w);
                file << ',';
            }
            if (n!=temp.at(l).size()-1) file << '\n';
        }
        if (l!=temp.size()-1) file << "\n\n";
    }
    file.close();
}

void network::import_weights_from_file(string file_name)
{
    vector <vector <vector <double> > > temp = parse_input_file(file_name);
    import_weights(temp);
}

vector <double> network::generate_weights(int nodes_next)
{
    vector <double> t;
    for (int a=0; a<nodes_next; a++) {
        if(LYRAND>0.5)  t.push_back(LYRAND);
        else t.push_back(-LYRAND);
    }
    return t;
}

void network::create_network()
{
    // create input layers
    layer li;
    for (int i=0; i<node_count.at(0); i++) {
        node n;
        li.nodes.push_back(n); // add nodes to layer
    } layers.push_back(li); // add layer to vector of layers
    
    // create hidden layers
    for (int l=1; l<node_count.size()-1; l++) {
        layer lh;
        for (int h=0; h<node_count.at(l); h++) {
            node n;
            lh.nodes.push_back(n);
        } layers.push_back(lh);
    }
    // create output layers
    layer lo;
    for (int o=0; o<node_count.at(node_count.size()-1); o++) {
        node n;
        lo.nodes.push_back(n); // add nodes to layer
    } layers.push_back(lo); // add layer to vector of layers
    
    // create weights
    for (int l=0; l<layers.size()-1; l++) {
        for (int n=0; n<layers.at(l).nodes.size(); n++) {
            layers.at(l).nodes.at(n).weights = generate_weights(node_count.at(l+1));
        }
    }
}

void network::setup(vector <int> nc)
{
    outputs.clear();
    inputs.clear();
    layers.clear();
    node_count.clear();
    
    node_count = nc;
    cycle = 1;
    create_network();
}

void network::set_values(vector <double> in)
{
    inputs.clear();
    inputs = in;
}

void network::mutate()
{
    for (int l=0; l<layers.size(); l++) {
        for (int n=0; n<layers.at(l).nodes.size(); n++) {
            if (LYRAND>0.5) layers.at(l).nodes.at(n).mutate();
        }
    }
}

void network::cycle_network()
{
    if (debug) cout << "cycle starting: " << cycle << endl;
    vector <vector <double> > temp_inputs;
    vector <vector <double> > temp_outputs;
    vector <double> temp_input;
    vector <double> temp_output;
    vector <vector <double> > temp_weights;
    // Iterate through each layer
    for (int l=0; l<layers.size(); l++) {
        // - get inputs from current layer's nodes except for input layer which gets its inputs from 'inputs'
        if (l==0) {
            temp_input = inputs;
        }
        else {
            for (int n=0; n<layers.at(l).nodes.size(); n++) {
                temp_inputs.push_back(layers.at(l).nodes.at(n).inputs);
            }
        }
        if (debug) cout << "got intputs" << endl;
        
        if (l!=layers.size()-1) {
            for (int n=0; n<layers.at(l).nodes.size(); n++) {
                temp_weights.push_back(layers.at(l).nodes.at(n).weights);
            }
        }
        if (debug) cout << "got weights" << endl;
        
        // - sum inputs except for input layer, its inputs are doubles, the rest are vectors of doubles
        if (l!=0) {
            for (int n=0; n<temp_inputs.size(); n++) {
                temp_input.push_back(0.0);
                for (int i=0; i<temp_inputs.at(n).size(); i++) {
                    temp_input.at(n) += temp_inputs.at(n).at(i);
                }
            }
        }
        if (debug) cout << "inputs summed" << endl;
        
        // - output layer returns outputs here, last layer
        if (l==layers.size()-1) {
            outputs = temp_input;
        }
        
        // - hidden layers apply sigmoid
        if (l==0) temp_output = temp_input;
        if (l!=0 && l!= layers.size()-1) {
            for (int a=0; a<temp_input.size(); a++) {
                temp_output.push_back(1/(1+exp(-temp_input.at(a))));
            }
        }
        if (debug) cout << "sigmoid applied" << endl;
        
        // - create output vectors and apply weights
        if (l!=layers.size()-1) {
            for (int a=0; a<node_count.at(l+1); a++) {
                // push back every output from every node int the last layer
                vector <double> t;
                for (int b=0; b<temp_output.size(); b++) {
                    t.push_back(temp_output.at(b)*temp_weights.at(b).at(a));
                } temp_outputs.push_back(t);
            }
        }
        if (debug) cout << "output vectors with weights created" << endl;
        
        // - fill next layer nodes with inputs
        if (l!=layers.size()-1) {
            for (int a=0; a<node_count.at(l+1); a++) {
                layers.at(l+1).nodes.at(a).inputs.clear();
                layers.at(l+1).nodes.at(a).inputs = temp_outputs.at(a);
            }
        }
        if (debug) cout << "- layer done" << endl;
        
        temp_inputs.clear();
        temp_outputs.clear();
        temp_input.clear();
        temp_output.clear();
        temp_weights.clear();
    }
    if (debug) cout << "cycle complete\n" << endl;
    cycle++;
}