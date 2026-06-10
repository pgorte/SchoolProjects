/**
 * File: BinaryDecisionTree.cpp
 * Project: Breast Cancer Decision Tree
 * Description: Implementation of Binary Decision Tree based on Figure 13
 *              Heavily commented for clarity
 * Author: Peter Gortemaker
 * Date: December 2025
 */

#include "BinaryDecisionTree.hpp"

/**
 * Decision functions
 * @param p - Patient to evaluate
 * @return true if matches named condition, false otherwise
 */
bool uniformityCellSize_le_2(const Patient& p) {return p.getUniformityCellSize() <= 2;}
bool bareNuclei_le_3(const Patient& p) {return p.getBareNuclei() <= 3;}
bool clumpThickness_le_3(const Patient& p) {return p.getClumpThickness() <= 3;}
bool blandChromatin_le_2(const Patient& p) {return p.getBlandChromatin() <= 2;}
bool marginalAdhesion_le_3(const Patient& p) {return p.getMarginalAdhesion() <= 3;}
bool uniformityCellShape_le_2(const Patient& p) {return p.getUniformityCellShape() <= 2;}
bool clumpThickness_le_5(const Patient& p) {return p.getClumpThickness() <= 5;}
bool uniformityCellSize_le_4(const Patient& p) {return p.getUniformityCellSize() <= 4;}
bool bareNuclei_le_2(const Patient& p) {return p.getBareNuclei() <= 2;}
bool uniformityCellSize_le_3(const Patient& p) {return p.getUniformityCellSize() <= 3;}
bool marginalAdhesion_le_5(const Patient& p) {return p.getMarginalAdhesion() <= 5;}
bool clumpThickness_le_6(const Patient& p) {return p.getClumpThickness() <= 6;}

BinaryDecisionTree::BinaryDecisionTree() : root(nullptr) {}
BinaryDecisionTree::~BinaryDecisionTree() {deleteTree(root);}

/**
 * Helper function to recursively delete tree nodes
 * @param node - Current node to delete
 */
void BinaryDecisionTree::deleteTree(Node* node) {
    if (node == nullptr) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

/**
 * Builds binary decision tree based on Figure 13
 * Constructs tree structure using function pointers
 * Heavily commented for clarity
 */
void BinaryDecisionTree::buildDecisionTree() {

    // LEFT BRANCH: Uniformity of Cell Size <= 2
    // Bare Nuclei <= 3: Benign
    Node* benign1 = new Node(2);

    // Bare Nuclei > 3 branch
    // Clump Thickness <= 3: Benign
    Node* benign2 = new Node(2);

    // Clump Thickness > 3 branch
    // Bland Chromatin <= 2, Marginal Adhesion <= 3: Malignant
    Node* malignant1 = new Node(4);
    // Bland Chromatin <= 2, Marginal Adhesion > 3: Benign
    Node* benign3 = new Node(2);

    // When Bland Chromatin <= 2, check Marginal Adhesion
    // Left (<=3): Malignant, Right (>3): Benign
    Node* node_ma_le_3_left = new Node(marginalAdhesion_le_3);
    node_ma_le_3_left->left = malignant1;
    node_ma_le_3_left->right = benign3;

    // Bland Chromatin > 2: Malignant
    Node* malignant2 = new Node(4);

    // When Clump Thickness > 3, check Bland Chromatin
    // Left (<=2): check Marginal Adhesion, Right (>2): Malignant
    Node* node_bc_le_2 = new Node(blandChromatin_le_2);
    node_bc_le_2->left = node_ma_le_3_left;
    node_bc_le_2->right = malignant2;

    // When Bare Nuclei > 3, check Clump Thickness
    // Left (<=3): Benign, Right (>3): check Bland Chromatin
    Node* node_ct_le_3 = new Node(clumpThickness_le_3);
    node_ct_le_3->left = benign2;
    node_ct_le_3->right = node_bc_le_2;

    // First decision in left branch: check Bare Nuclei
    // Left (<=3): Benign, Right (>3): check Clump Thickness
    Node* node_bn_le_3 = new Node(bareNuclei_le_3);
    node_bn_le_3->left = benign1;
    node_bn_le_3->right = node_ct_le_3;

    // RIGHT BRANCH: Uniformity of Cell Size > 2
    // Uniformity of Cell Shape <= 2
    // Clump Thickness <= 5: Benign
    Node* benign4 = new Node(2);
    // Clump Thickness > 5: Malignant
    Node* malignant3 = new Node(4);

    // When Cell Shape <= 2, check Clump Thickness
    // Left (<=5): Benign, Right (>5): Malignant
    Node* node_ct_le_5 = new Node(clumpThickness_le_5);
    node_ct_le_5->left = benign4;
    node_ct_le_5->right = malignant3;

    // Uniformity of Cell Shape > 2
    // Uniformity of Cell Size <= 4
    
    // Bare Nuclei <= 2, Marginal Adhesion <= 3: Benign
    Node* benign5 = new Node(2);
    // Bare Nuclei <= 2, Marginal Adhesion > 3: Malignant
    Node* malignant4 = new Node(4);

    // When Bare Nuclei <= 2, check Marginal Adhesion
    // Left (<=3): Benign, Right (>3): Malignant
    Node* node_ma_le_3_right = new Node(marginalAdhesion_le_3);
    node_ma_le_3_right->left = benign5;
    node_ma_le_3_right->right = malignant4;

    // Bare Nuclei > 2, Uniformity of Cell Size <= 3: Malignant
    Node* malignant5 = new Node(4);

    // Bare Nuclei > 2, Uniformity of Cell Size > 3
    // Marginal Adhesion <= 5: Benign
    Node* benign6 = new Node(2);
    // Marginal Adhesion > 5: Malignant
    Node* malignant6 = new Node(4);

    // When Cell Size > 3, check Marginal Adhesion
    // Left (<=5): Benign, Right (>5): Malignant
    Node* node_ma_le_5 = new Node(marginalAdhesion_le_5);
    node_ma_le_5->left = benign6;
    node_ma_le_5->right = malignant6;

    // When Bare Nuclei > 2, check Uniformity of Cell Size
    // Left (<=3): Malignant, Right (>3): check Marginal Adhesion
    Node* node_ucs_le_3 = new Node(uniformityCellSize_le_3);
    node_ucs_le_3->left = malignant5;
    node_ucs_le_3->right = node_ma_le_5;

    // When Cell Size <= 4, check Bare Nuclei
    // Left (<=2): check Marginal Adhesion, Right (>2): check Cell Size again
    Node* node_bn_le_2 = new Node(bareNuclei_le_2);
    node_bn_le_2->left = node_ma_le_3_right;
    node_bn_le_2->right = node_ucs_le_3;

    // Clump Thickness > 6: Malignant
    Node* malignant7 = new Node(4);

    // When Cell Size <= 4, check Clump Thickness
    // Left (<=6): check Bare Nuclei, Right (>6): Malignant
    Node* node_ct_le_6 = new Node(clumpThickness_le_6);
    node_ct_le_6->left = node_bn_le_2;
    node_ct_le_6->right = malignant7;

    // When Cell Shape > 2, check if Cell Size <= 4
    // Left (<=4): check Clump Thickness, Right (>4): Malignant
    Node* node_ucs_le_4 = new Node(uniformityCellSize_le_4);
    node_ucs_le_4->left = node_ct_le_6;
    // Uniformity of Cell Size > 4: Malignant
    node_ucs_le_4->right = new Node(4);

    // Check Uniformity of Cell Shape in right main branch
    // Left (<=2): check Clump Thickness, Right (>2): branching
    Node* node_ucsh_le_2 = new Node(uniformityCellShape_le_2);
    node_ucsh_le_2->left = node_ct_le_5;
    node_ucsh_le_2->right = node_ucs_le_4;

    // Root node
    root = new Node(uniformityCellSize_le_2);
    root->left = node_bn_le_3;
    root->right = node_ucsh_le_2;
}

/**
 * Classifies a patient using the decision tree
 * Traverses tree from root to leaf based on patient attributes
 * @param patient - Patient to classify
 * @return 2 for benign, 4 for malignant, 0 if tree is empty
 */
int BinaryDecisionTree::classifyPatient(const Patient& patient) const {
    if (root == nullptr) return 0;

    Node* current = root;

    // Traverse the tree until we reach a leaf node
    while (current->classification == 0) {
        if (current->decision(patient)) {
            current = current->left;  // True path
        }
        else {
            current = current->right; // False path
        }
    }

    return current->classification;
}