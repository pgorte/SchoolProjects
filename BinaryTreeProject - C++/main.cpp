/**
 * File: main.cpp
 * Project: Breast Cancer Decision Tree
 * Description: Main program to classify breast cancer patients
 * Author: Peter Gortemaker
 * Date: December 2025
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "Patient.hpp"
#include "BinaryDecisionTree.hpp"

using namespace std;

/**
 * Parse a CSV line and create a Patient object
 * @param line - CSV line to parse
 * @param patient - Patient object to populate
 * @return true if parsing successful, false otherwise
 */
bool parsePatientData(const string& line, Patient& patient) {
    stringstream ss(line);
    string token;
    vector<int> values;

    // Parse each comma-separated value
    while (getline(ss, token, ',')) {
        // Check for missing data (represented by '?')
        if (token == "?") {
            return false; // Invalid data
        }

        try {
            int value = stoi(token);
            values.push_back(value);
        }
        catch (...) {
            return false; // Invalid data format
        }
    }

    // Check if we have exactly 11 values
    if (values.size() != 11) {
        return false;
    }

    // Create patient with parsed values
    patient = Patient(values[0], values[1], values[2], values[3],
                      values[4], values[5], values[6], values[7],
                      values[8], values[9], values[10]);

    return patient.isValid();
}

/**
 * Read patient data from CSV file
 * @param filename - Input CSV filename
 * @param patients - Vector to store valid patients
 * @param invalidCount - Counter for invalid patients
 * @return true if file opened successfully
 */
bool readPatientData(const string& filename, vector<Patient>& patients, int& invalidCount) {
	
    // Open input file
    ifstream inFile(filename);

    if (!inFile.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return false;
    }

	// Read each line and parse patient data
    string line;
    int lineNum = 0;
    invalidCount = 0;

    while (getline(inFile, line)) {
        lineNum++;

        // Skip empty lines
        if (line.empty()) continue;

        Patient patient;
        if (parsePatientData(line, patient)) {
            patients.push_back(patient);
        }
        else {
            invalidCount++;
            // Report invalid patient
            cerr << "Invalid patient data at line " << lineNum << endl;
        }
    }
    cout << endl;

    inFile.close();
    return true;
}

/**
 * Write patient results to CSV file
 * @param filename - Output CSV filename
 * @param patients - Vector of patients with diagnoses
 * @return true if file written successfully
 */
bool writeResults(const string& filename, const vector<Patient>& patients) {

	// Open output file
    ofstream outFile(filename);

    if (!outFile.is_open()) {
        cerr << "Error: Could not create file " << filename << endl;
        return false;
    }

	// Write each patient's data to file
    for (const auto& patient : patients) {
        outFile << patient.toCSV() << endl;
    }

    outFile.close();
    return true;
}




int main() {
    const string input_file = "unformatted_data_v1.0.0.csv";
    const string output_file = "results.csv";

    // Read patient data
    vector<Patient> patients;
    int invalidCount = 0;

	// Load patients from input file
    if (!readPatientData(input_file, patients, invalidCount)) {
        return 1;
    }

    // Build decision tree
    BinaryDecisionTree tree;
    tree.buildDecisionTree();


    int benignCount = 0;
    int malignantCount = 0;

	// Classify each patient and update diagnosis
    for (auto& patient : patients) {
        int diagnosis = tree.classifyPatient(patient);
        patient.setDiagnosis(diagnosis);

        if (diagnosis == 2) { benignCount++; }
        else if (diagnosis == 4) { malignantCount++; }
    }

    // Write results to file
    if (!writeResults(output_file, patients)) { return 1; }

    // Print summary
    cout << "Total Patients Processed: " << patients.size() << endl;
    cout << "Total Benign: " << benignCount << endl;
    cout << "Total Malignant: " << malignantCount << endl;
    cout << "Total Invalid Patients: " << invalidCount << endl;

    return 0;
}