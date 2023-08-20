#include <iostream>
#include <fstream>
#include <vector>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

namespace DeepForest {

struct Branch;

struct Branch {
    int Attribute;
    float Value;
    bool IsLeaf;
    std::vector<float> LeafValue;
    float Gini;
    float GiniGain;
    int Size;
    int Branch0Index; // Index of the first branch
    int Branch1Index; // Index of the second branch
    int Depth;

    Branch() : IsLeaf(true), Branch0Index(-1), Branch1Index(-1) {}

    bool operator==(const Branch& other) const {
    return Attribute == other.Attribute &&
            Value == other.Value &&
            IsLeaf == other.IsLeaf &&
            LeafValue == other.LeafValue &&
            Gini == other.Gini &&
            GiniGain == other.GiniGain &&
            Size == other.Size &&
            Branch0Index == other.Branch0Index &&
            Branch1Index == other.Branch1Index &&
            Depth == other.Depth;
    }

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & Attribute;
        ar & Value;
        ar & IsLeaf;
        ar & LeafValue;
        ar & Gini;
        ar & GiniGain;
        ar & Size;
        ar & Branch0Index;
        ar & Branch1Index;
        ar & Depth;
    }

    template<class Archive>
    void save(Archive& ar, const unsigned int version) const {
        ar & Attribute;
        ar & Value;
        ar & IsLeaf;
        ar & LeafValue;
        ar & Gini;
        ar & GiniGain;
        ar & Size;
        ar & Branch0Index;
        ar & Branch1Index;
        ar & Depth;
    }

    template<class Archive>
    void load(Archive& ar, const unsigned int version) {
        ar & Attribute;
        ar & Value;
        ar & IsLeaf;
        ar & LeafValue;
        ar & Gini;
        ar & GiniGain;
        ar & Size;
        ar & Branch0Index;
        ar & Branch1Index;
        ar & Depth;
    }
};

struct ForestData {
    std::vector<std::vector<float>> X;
    std::vector<int> Class;


    bool operator==(const ForestData& other) const {return X == other.X && Class == other.Class;}
    //for some reason that structs needs to have != operator defined separately - in any other case it doesn't compile
    bool operator!=(const ForestData& other) const {return !(X == other.X && Class == other.Class);}

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & X;
        ar & Class;
    }
};

struct Tree {
    std::vector<Branch> Branches; // Actual branches of the tree
    // Root = Branches[0]
    float Validation;

    bool operator==(const Tree& other) const {
    return Branches == other.Branches && Validation == other.Validation;
    }

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & Branches;
        ar & Validation;
    }
};

struct Forest {
    ForestData Data;
    std::vector<Tree> Trees;
    int Features;
    int Classes;
    int LeafSize;
    int MFeatures;
    int NTrees;
    int NSize;
    int MaxDepth;
    std::vector<float> FeatureImportance;

    void save(const std::string& filename) const {
        std::ofstream ofs(filename, std::ios::binary);
        if (ofs.is_open()) {
            boost::archive::binary_oarchive archive(ofs);
            archive << *this;
            ofs.close();
            std::cout << "Forest saved to " << filename << std::endl;
        } else {
            std::cerr << "Failed to open " << filename << " for writing." << std::endl;
        }
    }

    void load(const std::string& filename) {
        std::ifstream ifs(filename, std::ios::binary);
        if (ifs.is_open()) {
            boost::archive::binary_iarchive archive(ifs);
            archive >> *this;
            ifs.close();
            std::cout << "Forest loaded from " << filename << std::endl;
        } else {
            std::cerr << "Failed to open " << filename << " for reading." << std::endl;
        }
    }

    bool operator==(const Forest& other) const {
        // Compare forest parameters
        if (Data != other.Data ||
            Features != other.Features ||
            Classes != other.Classes ||
            LeafSize != other.LeafSize ||
            MFeatures != other.MFeatures ||
            NTrees != other.NTrees ||
            NSize != other.NSize ||
            MaxDepth != other.MaxDepth ||
            FeatureImportance != other.FeatureImportance ||
            Trees != other.Trees) {
            return false;
        }
    return true;
    }

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & Data;
        ar & Trees;
        ar & Features;
        ar & Classes;
        ar & LeafSize;
        ar & MFeatures;
        ar & NTrees;
        ar & NSize;
        ar & MaxDepth;
        ar & FeatureImportance;
    }
};

} // namespace DeepForest



// test serialization and deserialization


int main() {
    // Create a test Forest object
    DeepForest::Forest testForest;

    DeepForest::Tree tree1;
    DeepForest::Tree tree2;

    // Initialize boolean variables for leaves
    DeepForest::Branch leaf1, leaf2;
    leaf1.IsLeaf = true;
    leaf2.IsLeaf = true;
    tree1.Branches.push_back(leaf1);
    tree1.Branches.push_back(leaf2);

    // Initialize boolean variables for leaves and branches
    DeepForest::Branch branch1, branch2, leaf3, leaf4;
    leaf3.IsLeaf = true;
    leaf4.IsLeaf = true;
    branch1.IsLeaf = false; // This is a branch, not a leaf
    branch2.IsLeaf = false; // This is a branch, not a leaf
    branch2.Branch0Index = tree1.Branches.size();
    branch2.Branch1Index = tree1.Branches.size() + 1;
    tree1.Branches.push_back(leaf3);
    tree1.Branches.push_back(leaf4);
    tree2.Branches.push_back(branch1);
    tree2.Branches.push_back(branch2);

    // ... (set other parameters of testForest)

    // Specify the filename for saving
    std::string filename = "test_forest.rf";

    // Serialize and save the Forest object to the binary file
    // Save the forest using the save method
    testForest.save(filename);

    // Deserialize and read the Forest object from the binary file
    DeepForest::Forest loadedForest;
    loadedForest.load("test_forest.rf");

    // Compare the loadedForest with the original testForest
    if (testForest == loadedForest) {
        std::cout << "Loaded forest is the same as the original forest." << std::endl;
    } else {
        std::cerr << "Error: Loaded forest is not the same as the original forest." << std::endl;
    }

    // Create a new Forest object with one tree containing one leaf
    DeepForest::Forest newForest;

    DeepForest::Tree newTree;
    DeepForest::Branch newLeaf;
    newLeaf.IsLeaf = true;
    newTree.Branches.push_back(newLeaf);

    newForest.Trees.push_back(newTree);

    // Compare the newForest with the loadedForest
    if (newForest == loadedForest) {
        std::cerr << "Error:New forest is the same as the loaded forest." << std::endl;
    } else {
        std::cout << "New forest is not the same as the loaded forest." << std::endl;
    }

    return 0;
}

