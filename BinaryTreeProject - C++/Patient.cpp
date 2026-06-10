/**
 * File: Patient.cpp
 * Project: Breast Cancer Decision Tree
 * Description: Implementation of Patient class
 * Author: Peter Gortemaker
 * Date: December 2025
 */

#include "Patient.hpp"
#include <sstream>

// Default constructor
Patient::Patient()
    : sampleCode(0), clumpThickness(0), uniformityCellSize(0),
    uniformityCellShape(0), marginalAdhesion(0), singleEpithelialCellSize(0),
    bareNuclei(0), blandChromatin(0), normalNucleoli(0), mitoses(0),
    diagnosis(0) {}

// Parameterized constructor
Patient::Patient(int code, int ct, int ucs, int ucsh, int ma, int secs,
                 int bn, int bc, int nn, int m, int diag)
    : sampleCode(code), clumpThickness(ct), uniformityCellSize(ucs),
    uniformityCellShape(ucsh), marginalAdhesion(ma),
    singleEpithelialCellSize(secs), bareNuclei(bn), blandChromatin(bc),
    normalNucleoli(nn), mitoses(m), diagnosis(diag) {}

/**
 * Validates patient data
 * Checks if all attributes are within valid range (1-10)
 * @return true if all data is valid, false otherwise
 */
bool Patient::isValid() const {
    if (sampleCode <= 0) return false;

    // Check each attribute is between 1 and 10
    if (clumpThickness < 1 || clumpThickness > 10) return false;
    if (uniformityCellSize < 1 || uniformityCellSize > 10) return false;
    if (uniformityCellShape < 1 || uniformityCellShape > 10) return false;
    if (marginalAdhesion < 1 || marginalAdhesion > 10) return false;
    if (singleEpithelialCellSize < 1 || singleEpithelialCellSize > 10) return false;
    if (bareNuclei < 1 || bareNuclei > 10) return false;
    if (blandChromatin < 1 || blandChromatin > 10) return false;
    if (normalNucleoli < 1 || normalNucleoli > 10) return false;
    if (mitoses < 1 || mitoses > 10) return false;

    return true;
}

/**
 * Converts patient data to CSV format
 * @return String of patient data comma-separated
 */
std::string Patient::toCSV() const {
    std::ostringstream oss;
    oss << sampleCode << ","
        << clumpThickness << ","
        << uniformityCellSize << ","
        << uniformityCellShape << ","
        << marginalAdhesion << ","
        << singleEpithelialCellSize << ","
        << bareNuclei << ","
        << blandChromatin << ","
        << normalNucleoli << ","
        << mitoses << ","
        << diagnosis;
    return oss.str();
}