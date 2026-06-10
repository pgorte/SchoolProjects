/**
 * File: Patient.h
 * Project: Breast Cancer Decision Tree
 * Description: Patient class representing breast cancer patient data
 * Author: Peter Gortemaker
 * Date: December 2025
 */

#ifndef PATIENT_H
#define PATIENT_H

#include <string>

class Patient {
private:
    int sampleCode;
    int clumpThickness;
    int uniformityCellSize;
    int uniformityCellShape;
    int marginalAdhesion;
    int singleEpithelialCellSize;
    int bareNuclei;
    int blandChromatin;
    int normalNucleoli;
    int mitoses;
    int diagnosis; // 0 = unknown, 2 = benign, 4 = malignant

public:
    // Constructor
    Patient();
    Patient(int code, int ct, int ucs, int ucsh, int ma, int secs,
            int bn, int bc, int nn, int m, int diag = 0);

    // Getters
    int getSampleCode() const { return sampleCode; }
    int getClumpThickness() const { return clumpThickness; }
    int getUniformityCellSize() const { return uniformityCellSize; }
    int getUniformityCellShape() const { return uniformityCellShape; }
    int getMarginalAdhesion() const { return marginalAdhesion; }
    int getSingleEpithelialCellSize() const { return singleEpithelialCellSize; }
    int getBareNuclei() const { return bareNuclei; }
    int getBlandChromatin() const { return blandChromatin; }
    int getNormalNucleoli() const { return normalNucleoli; }
    int getMitoses() const { return mitoses; }
    int getDiagnosis() const { return diagnosis; }

    // Setters
    void setDiagnosis(int diag) { diagnosis = diag; }

    // Validation - checks if all attributes are within valid range (1-10)
    bool isValid() const;

    // Output to CSV format
    std::string toCSV() const;
};

#endif // PATIENT_H