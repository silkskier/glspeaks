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

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & X;
        ar & Class;
    }
};

struct Tree {
    std::vector<Branch> Branches; // Actual branches of the tree
    float Validation;

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & Branches;
        ar & Validation;
    }
};

struct Forest {
    ForestData Data;
    std::vector<Branch> Branches;
    std::vector<Tree> Trees;
    int Features;
    int Classes;
    int LeafSize;
    int MFeatures;
    int NTrees;
    int NSize;
    int MaxDepth;
    std::vector<float> FeatureImportance;

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & Data;
        ar & Branches;
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

bool compare_forests(const DeepForest::Forest& forest1, const DeepForest::Forest& forest2) {
    // Compare forest parameters
    if (forest1.Features != forest2.Features ||
        forest1.Classes != forest2.Classes ||
        forest1.LeafSize != forest2.LeafSize ||
        forest1.MFeatures != forest2.MFeatures ||
        forest1.NTrees != forest2.NTrees ||
        forest1.NSize != forest2.NSize ||
        forest1.MaxDepth != forest2.MaxDepth ||
        forest1.FeatureImportance != forest2.FeatureImportance) {
        return false;
    }

    // Compare tree count
    if (forest1.Trees.size() != forest2.Trees.size()) {
        return false;
    }

    // Compare each tree and its branches
    for (std::size_t i = 0; i < forest1.Trees.size(); ++i) {
        if (forest1.Trees[i].Branches != forest2.Trees[i].Branches ||
            forest1.Trees[i].Validation != forest2.Trees[i].Validation) {
            return false;
        }
    }

    // ... (you can add more comparisons for other parts of the forest if needed)

    return true;
}


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
    std::ofstream ofs(filename, std::ios::binary);
    if (ofs.is_open()) {
        boost::archive::binary_oarchive archive(ofs);
        archive << testForest;
        ofs.close();
        std::cout << "Test Forest saved to " << filename << std::endl;
    } else {
        std::cerr << "Failed to open " << filename << " for writing." << std::endl;
    }

       // Deserialize and read the Forest object from the binary file
    std::ifstream ifs(filename, std::ios::binary);
    if (ifs.is_open()) {
        DeepForest::Forest loadedForest;
        boost::archive::binary_iarchive archive(ifs);
        archive >> loadedForest;
        ifs.close();
        std::cout << "Test Forest loaded from " << filename << std::endl;

        // Compare the loadedForest with the original testForest
        if (compare_forests(testForest, loadedForest)) {
            std::cout << "Loaded forest is the same as the original forest." << std::endl;
        } else {
            std::cerr << "Error: Loaded forest is not the same as the original forest." << std::endl;
        }

        // Now you can access and use the loadedForest object
        // ... (perform actions with loadedForest)
    } else {
        std::cerr << "Failed to open " << filename << " for reading." << std::endl;
        return 1;
    }


    return 0;
}
