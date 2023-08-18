#include <iostream>
#include <fstream>
#include <vector>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

namespace DeepForest {

struct Branch;

void deserializeBranch(boost::archive::binary_iarchive& ar, Branch*& branch);

struct Branch {
    int Attribute;
    float Value;
    bool IsLeaf;
    std::vector<float> LeafValue;
    float Gini;
    float GiniGain;
    int Size;
    Branch* Branch0;
    Branch* Branch1;
    int Depth;

    Branch() : Branch0(nullptr), Branch1(nullptr) {}

    ~Branch() {
        delete Branch0;
        delete Branch1;
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
        ar & Depth;

        if (!IsLeaf) {
            deserializeBranch(ar, Branch0);
            deserializeBranch(ar, Branch1);
        }
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()
};

void deserializeBranch(boost::archive::binary_iarchive& ar, Branch*& branch) {
    int attribute;
    float value;
    bool isLeaf;
    std::vector<float> leafValue;
    float gini;
    float giniGain;
    int size;
    int depth;
    ar & attribute;
    ar & value;
    ar & isLeaf;
    ar & leafValue;
    ar & gini;
    ar & giniGain;
    ar & size;
    ar & depth;

    branch = new Branch;
    branch->Attribute = attribute;
    branch->Value = value;
    branch->IsLeaf = isLeaf;
    branch->LeafValue = leafValue;
    branch->Gini = gini;
    branch->GiniGain = giniGain;
    branch->Size = size;
    branch->Depth = depth;

    if (!isLeaf) {
        deserializeBranch(ar, branch->Branch0);
        deserializeBranch(ar, branch->Branch1);
    }
}

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
    Branch Root;
    float Validation;

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & Root;
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
    // Create an empty Forest object
    DeepForest::Forest emptyForest;

    // Specify the filename for saving
    std::string filename = "empty_forest.rf";

    // Serialize and save the Forest object to the binary file
    std::ofstream ofs(filename, std::ios::binary);
    if (ofs.is_open()) {
        boost::archive::binary_oarchive archive(ofs);
        archive << emptyForest;
        ofs.close();
        std::cout << "Empty Forest saved to " << filename << std::endl;
    } else {
        std::cerr << "Failed to open " << filename << " for writing." << std::endl;
    }

    // Simple deserialization attempt
    DeepForest::Forest loadedForest;

    // Load and deserialize the Forest object from the binary file
    std::ifstream ifs(filename, std::ios::binary);
    if (ifs.is_open()) {
        boost::archive::binary_iarchive archive(ifs);
        archive >> loadedForest;

        /*
        // The following loop calls the deserializeBranch function for each tree's root branch. Not neccessary because of .load
        for (auto& tree : loadedForest.Trees) {
            DeepForest::deserializeBranch(archive, tree.Root.Branch0);
            DeepForest::deserializeBranch(archive, tree.Root.Branch1);
        }*/

        ifs.close();
        std::cout << "Empty Forest loaded from " << filename << std::endl;
    } else {
        std::cerr << "Failed to open " << filename << " for reading." << std::endl;
    }

    return 0;
}
