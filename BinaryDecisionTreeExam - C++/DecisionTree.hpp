#pragma once
#include <string>
#include <iostream>

using namespace std;

typedef bool (*DecisionFunction)(const int input);

bool healthy(int input) { return input == 1; }
bool gym(int input) { return input == 1; }
bool tired(int input) { return input == 1; }
bool rest(int input) { return input == 1; }
bool hungry(int input) { return input == 1; }
bool eat(int input) { return input == 1; }
bool sick(int input) { return input == 1; }
bool sickRest(int input) { return input == 1; }
bool sickGym(int input) { return input == 1; }

class DecisionTree {
private:
	struct Node {
		
		DecisionFunction decision;
		Node* left;
		Node* right;
		string output_;

		Node() : decision(nullptr), left(nullptr), right(nullptr), output_() {}
		Node(DecisionFunction func, string output) : decision(func), left(nullptr), right(nullptr), output_(output) {}
	};

	Node* root;

public: 
	DecisionTree() : root(nullptr) {}
	~DecisionTree() {}

	const void buildTree() {
		Node* healthyNode = new Node(healthy, "Do I feel healthy?");
		root = healthyNode;
		Node* gymNode = new Node(gym, "Go to gym");
		Node* tiredNode = new Node(tired, "Do I feel tired?");
		healthyNode->left = gymNode;
		healthyNode->right = tiredNode;
		Node* restNode = new Node(rest, "Rest");
		Node* hungryNode = new Node(hungry, "Do I feel hungry?");
		tiredNode->left = restNode;
		tiredNode->right = hungryNode;
		Node* eatNode = new Node(eat, "Eat");
		Node* sickNode = new Node(sick, "Do I feel sick");
		hungryNode->left = eatNode;
		hungryNode->right = sickNode;
		Node* sickRestNode = new Node(sickRest, "Rest");
		Node* sickGymNode = new Node(sickGym, "Go to gym");
		sickNode->left = sickRestNode;
		sickNode->right = sickGymNode;
	}

	void navigateTree() {
		Node* current = root;
		int input = 0;

		cout << root->output_ << endl;

		while (current->left != nullptr) {
			cin >> input;
			if (current->decision(input)) {
				current = current->left;
				cout << current->output_ << endl;
			}
			else {
				current = current->right;
				cout << current->output_ << endl;
			}
		}
	}
};