/**
 * File: BinaryDecisionTree.hpp
 * Project: Breast Cancer Decision Tree
 * Description: Binary Decision Tree for breast cancer diagnosis
 * Author: Peter Gortemaker
 * Date: December 2025
 */

#ifndef BINARY_DECISION_TREE_H
#define BINARY_DECISION_TREE_H

#include "Patient.hpp"

// Decision function type
typedef bool (*DecisionFunction)(const Patient&);

// Decision functions based on actual Figure 13 from the paper
bool uniformityCellSize_le_2(const Patient& p);
bool bareNuclei_le_3(const Patient& p);
bool clumpThickness_le_3(const Patient& p);
bool blandChromatin_le_2(const Patient& p);
bool marginalAdhesion_le_3(const Patient& p);
bool uniformityCellShape_le_2(const Patient& p);
bool clumpThickness_le_5(const Patient& p);
bool uniformityCellSize_le_4(const Patient& p);
bool bareNuclei_le_2(const Patient& p);
bool uniformityCellSize_le_3(const Patient& p);
bool marginalAdhesion_le_5(const Patient& p);
bool clumpThickness_le_6(const Patient& p);

class BinaryDecisionTree {
private:
    struct Node {
        DecisionFunction decision;
        Node* left;   // True path
        Node* right;  // False path
        int classification; // 2 = Benign, 4 = Malignant, 0 = not a leaf

        Node() : decision(nullptr), left(nullptr), right(nullptr),
            classification(0) {}
        Node(int classif) : decision(nullptr), left(nullptr), right(nullptr),
            classification(classif) {}
        Node(DecisionFunction func) : decision(func), left(nullptr),
            right(nullptr), classification(0) {}
    };

    Node* root;

    // Helper function to delete tree
    void deleteTree(Node* node);

public:
    BinaryDecisionTree();
    ~BinaryDecisionTree();

    void buildDecisionTree();

    // Classify a patient using the decision tree
    int classifyPatient(const Patient& patient) const;
};

#endif // BINARY_DECISION_TREE_H