
#include <iostream>
#include <iomanip>
#include <cmath>
#include <limits>
using namespace std;

// Motor data: rating in kW and estimated peak efficiency
struct Motor {
    double rating;
    double peakEfficiency;
};

// Example motor catalog, not manufacturer specifications
const Motor motors[] = {
    {0.37, 0.75},
    {0.55, 0.78},
    {0.75, 0.80},
    {1.10, 0.82},
    {1.50, 0.84},
    {2.20, 0.86},
    {3.00, 0.87},
    {4.00, 0.88},
    {5.50, 0.89},
    {7.50, 0.90},
    {11.0, 0.91}
};

const int MOTOR_COUNT = sizeof(motors) / sizeof(motors[0]);

// Read and validate a positive numerical input
double getPositiveInput(string prompt, double minimum,
                        double maximum) {
    double value;

    while (true) {
        cout << prompt;

        if (cin >> value && isfinite(value)
            && value >= minimum && value <= maximum) {
            return value;
        }

        cout << "Invalid input. Enter a number from "
             << minimum << " to " << maximum << ".\n";

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// Calculate mechanical output power in kW
double calculatePower(double torque, double rpm) {
    return torque * rpm / 9550.0;
}

// Calculate required motor rating using a service factor
double calculateDesignPower(double power, double serviceFactor) {
    return power * serviceFactor;
}

// Find the smallest motor rating meeting the design requirement
int selectMotor(double designPower) {
    for (int i = 0; i < MOTOR_COUNT; i++) {
        if (motors[i].rating >= designPower) {
            return i;
        }
    }

    return -1;
}

// Educational partial-load efficiency model
// Load factor = output power / motor rated power
// Efficiency is an estimate, not a manufacturer's performance curve
double calculateEfficiency(double loadFactor,
                           double peakEfficiency) {
    return peakEfficiency * (0.40 + 0.60 * loadFactor);
}

// Calculate annual electricity cost
double calculateAnnualCost(double outputPower,
                           double efficiency,
                           double hoursPerDay,
                           double daysPerYear,
                           double ratePerKWh) {
    double inputPower = outputPower / efficiency;
    double annualEnergy = inputPower * hoursPerDay * daysPerYear;

    return annualEnergy * ratePerKWh;
}

// Display results for one motor option
void displayMotorReport(int index, double outputPower,
                        double hoursPerDay, double daysPerYear,
                        double ratePerKWh) {
    double loadFactor = outputPower / motors[index].rating;
    double efficiency = calculateEfficiency(
        loadFactor, motors[index].peakEfficiency);

    double inputPower = outputPower / efficiency;
    double annualCost = calculateAnnualCost(
        outputPower, efficiency, hoursPerDay,
        daysPerYear, ratePerKWh);

    cout << fixed << setprecision(2);
    cout << "\nMotor rating: " << motors[index].rating << " kW\n";
    cout << "Operating load: " << loadFactor * 100 << "%\n";
    cout << "Estimated efficiency: " << efficiency * 100 << "%\n";
    cout << "Estimated electrical input: " << inputPower << " kW\n";
    cout << "Annual electricity cost: PHP "
         << annualCost << "\n";
}

// Main program
int main() {
    cout << "========================================\n";
    cout << " MOTOR SELECTION & EFFICIENCY CALCULATOR\n";
    cout << "========================================\n";

    cout << "\nEnter machine operating data:\n";

    double torque = getPositiveInput(
        "Torque (N.m, 0.01 to 100000): ", 0.01, 100000);

    double rpm = getPositiveInput(
        "Speed (RPM, 1 to 100000): ", 1, 100000);

    double serviceFactor = getPositiveInput(
        "Service factor (1.0 to 2.0): ", 1.0, 2.0);

    cout << "\nEnter operating cost data:\n";

    double hoursPerDay = getPositiveInput(
        "Operating hours per day (0.1 to 24): ", 0.1, 24);

    double daysPerYear = getPositiveInput(
        "Operating days per year (1 to 365): ", 1, 365);

    double ratePerKWh = getPositiveInput(
        "Electricity rate (PHP/kWh, 0.01 to 1000): ",
        0.01, 1000);

    double outputPower = calculatePower(torque, rpm);
    double designPower = calculateDesignPower(
        outputPower, serviceFactor);

    cout << fixed << setprecision(2);
    cout << "\n========== ENGINEERING REPORT ==========\n";
    cout << "Mechanical output power: " << outputPower << " kW\n";
    cout << "Service factor: " << serviceFactor << "\n";
    cout << "Required design power: " << designPower << " kW\n";

    int selected = selectMotor(designPower);

    if (selected == -1) {
        cout << "\nNo suitable motor in the sample catalog.\n";
        cout << "Consider a larger motor rating.\n";
        return 0;
    }

    cout << "\n========== MOTOR COMPARISON ============\n";

    displayMotorReport(selected, outputPower, hoursPerDay,
                       daysPerYear, ratePerKWh);

    // Compare with the next larger motor, if available
    if (selected + 1 < MOTOR_COUNT) {
        displayMotorReport(selected + 1, outputPower,
                           hoursPerDay, daysPerYear, ratePerKWh);

        double efficiency1 = calculateEfficiency(
            outputPower / motors[selected].rating,
            motors[selected].peakEfficiency);

        double efficiency2 = calculateEfficiency(
            outputPower / motors[selected + 1].rating,
            motors[selected + 1].peakEfficiency);

        double cost1 = calculateAnnualCost(
            outputPower, efficiency1, hoursPerDay,
            daysPerYear, ratePerKWh);

        double cost2 = calculateAnnualCost(
            outputPower, efficiency2, hoursPerDay,
            daysPerYear, ratePerKWh);

        cout << "\n========== COMPARISON RESULT ===========\n";

        if (cost1 < cost2) {
            cout << motors[selected].rating
                 << " kW motor has the lower estimated annual cost.\n";
        } else if (cost2 < cost1) {
            cout << motors[selected + 1].rating
                 << " kW motor has the lower estimated annual cost.\n";
        } else {
            cout << "Both motors have equal estimated annual costs.\n";
        }

        cout << "Estimated annual cost difference: PHP "
             << abs(cost1 - cost2) << "\n";
    } else {
        cout << "\nNo larger motor is available for comparison.\n";
    }

    cout << "\nNOTE: Efficiency values use a simplified educational "
         << "model.\n";
    cout << "Use manufacturer data for real motor selection.\n";
    cout << "========================================\n";

    return 0;
}
