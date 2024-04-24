//
//  main.cpp
//  BPTree
//
//  Name: Zhang Dong
//  UFID: 69633923
//  Email: zdong@ufl.edu
//

#include "BPTree.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <queue>

using namespace std;
//  Define command in SampleInput.txt for implement
const string Initialize ("Initialize");
const string Insert ("Insert");
const string Delete ("Delete");
const string Search ("Search");
const string Output ("Output.txt");



int main (int argc, char **argv)
{
    //  If not select input file, ERROR
    // if (argc != 2) {
    //     cout << "ERROR! No Input file!" << endl;
    //     exit (-1);
    // }
    
    ifstream in (argv[1]);
    ofstream out (Output.c_str ());
    
    string line;
    string command;
    string data;
    
    int i = 0;

    // while (!in.eof ()) {
    //     //  Get line of input file as string to compare with command defined
    //     i++;
    //     getline (in, line);
        
    //     command = line.substr(0, line.find('('));
    //     data = line.substr(line.find('(') + 1, line.find(')') - line.find('(') - 1);

    //     if (command.compare (Initialize) == 0) {
    //         //  Intialize operation
    //         int order = stoi(data);
    //         tree.Initialize (order);
    //     } else if (command.compare (Insert) == 0) {
    //         //  Insert operation
    //         int key = stoi (data.substr(0, data.find(',')));
    //         float val = stof (data.substr(data.find(',') + 1, data.length() - data.find(',')));
    //         tree.Insert (key, val);
    //     } else if (command.compare (Delete) == 0) {
    //         //  Delete operation
    //         int key = stoi (data);
    //         tree.Delete (key);
    //     } else if (command.compare (Search) == 0) {
    //         //  Search operation for two keys
    //         if (data.find(',') != string::npos) {
    //             int num;
    //             bool *results;
    //             float *vals;
    //             int key1 = stoi (data.substr(0, data.find(',')));
    //             int key2 = stoi (data.substr(data.find(',') + 1, data.length() - data.find(',')));
    //             results = tree.Search (key1, key2, vals, num);
    //             //  can not find val
    //             if (!results[0]) {
    //                 out << "";
    //             }
                
    //             for (int i = 0; i < num; i++) {
    //                 out << vals[i];
    //                 if (i != num - 1) out << ", ";
    //             }
                
    //             if (!results[1]) {
    //                 out << "";
    //             }
    //             out << endl;
    //         } else {
    //             //  Search operation for one key
    //             float val;
    //             int key = stoi (data);
            
    //             if (tree.Search (key, val)) {
    //                 out << val << endl;  
    //             } else { 
    //                 out << "" << endl; 
    //             }
    //         }
    //     } else {
    //         //  Command error
    //         out << "ERROR! Wrong input of command! " << i << endl;  
    //     }  
    // }
    // tree.Initialize(3);
    // tree.Insert(1, 4);
    // tree.Insert(5, 3);
    // tree.Insert(6, 2);
    // tree.Insert(8, 1);
    // tree.printTreeStructure();

    BPTree<double> tree;
    tree.Initialize(3);
    tree.Insert(1.0, 8.000);
    tree.Insert(2.0, 2.000);
    tree.Insert(3.0, 7.000);
    tree.Insert(4.0, 1.000);
    tree.Insert(5.0, 4.000);
    tree.Insert(6.0, 5.000);
    float value;
    bool found = tree.Search(6.0, value);
    if (found) {
        cout << "Value found: " << value << endl;
    } else {
        cout << "Value not found." << endl;
    }
    tree.printTreeStructure();
    // Perform level order traversal and print the structure of the B+ tree
    in.close ();
    out.close ();
    return 0;
}