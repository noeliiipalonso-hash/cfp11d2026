
#include <iostream>
#include <iomanip>
using namespace std;

// Calculate mechanical power in kilowatts
double calculatePower(double torque, double rpm) {
    return (torque * rpm) / 9550.0;
}

// Select the smallest motor that meets the required power
double selectMotor(double requiredPower) {
    double motorRatings[] = {0.37, 0.55, 0.75, 1.1, 1.5, 2.2, 3.0, 4.0, 5.5, 7.5};
    int size = 10;

    for (int i = 0; i < size; i++) {
        if (motorRatings[i] >= requiredPower) {
            return motorRatings[i];
        }
    }

    return -1;
}

int main() {
    double torque, rpm, serviceFactor;

    cout << "=== MOTOR SELECTION CALCULATOR ===\n";

    cout << "Enter required torque (N.m): ";
    if (!(cin >> torque) || torque <= 0) {
        cout << "Error: Torque must be a positive number.\n";
        return 1;
    }

    cout << "Enter rotational speed (RPM): ";
    if (!(cin >> rpm) || rpm <= 0) {
        cout << "Error: Speed must be a positive number.\n";
        return 1;
    }

    cout << "Enter service factor (e.g. 1.15): ";
    if (!(cin >> serviceFactor) || serviceFactor < 1.0) {
        cout << "Error: Service factor must be at least 1.0.\n";
        return 1;
    }

    double power = calculatePower(torque, rpm);
    double requiredPower = power * serviceFactor;
    double selectedMotor = selectMotor(requiredPower);

    cout << fixed << setprecision(2);
    cout << "\n=== ENGINEERING REPORT ===\n";
    cout << "Mechanical power: " << power << " kW\n";
    cout << "Design power: " << requiredPower << " kW\n";

    if (selectedMotor < 0) {
        cout << "No suitable motor in the sample catalog.\n";
    } else {
        cout << "Selected motor rating: " << selectedMotor << " kW\n";
    }

    return 0;
}
