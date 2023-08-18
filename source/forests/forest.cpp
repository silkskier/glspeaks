/*
    A simple deep forest (Kevin Miller, Chris Hettinger, Jeffrey Humpherys, Tyler Jarvis, David Kartchner - Forward Thinking: Building Deep Random Forests) building C++ library.
    Heavily inspired by https://github.com/malaschitz/randomForest
*/



#include <iostream>
#include <fstream>
#include <vector>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

namespace DeepForest {

// ForestData contains database
struct ForestData {
    std::vector<std::vector<double>> X; // All data are float64 numbers
    std::vector<int> Class; // Result should be int numbers 0,1,2,..
};

// Branch is tree structure of branches
struct Branch {
    int Attribute;
    double Value;
    bool IsLeaf;
    std::vector<double> LeafValue;
    double Gini;
    double GiniGain;
    int Size;
    Branch* Branch0;
    Branch* Branch1;
    int Depth;
};

// Tree is one random tree in forest with Branch and validation number
struct Tree {
    Branch Root;
    double Validation;
};

//Forest is base class for whole forest with database, properties of Forest and trees.
struct Forest {
    ForestData Data; // database for calculate trees
    std::vector<Tree> Trees;    // all generated trees
    int Features;   // number of attributes
    int Classes;    // number of classes
    int LeafSize;   // leaf size
    int MFeatures;  // attributes for choose proper split
    int NTrees;     // number of trees
    int NSize;      // len of data
    int MaxDepth;   // max depth of forest
    std::vector<double> FeatureImportance;  // stats of FeatureImportance
};





}
