## Current Functionality

The AFRM prototype processes simulated flight data from CSV files. It validates each record, calculates fuel requirements and theoretical planning range, then classifies the fuel margin as `INSUFFICIENT`, `CAUTION` or `SUFFICIENT`.

The application uses a fictional fuel policy comprising a 10% trip-fuel contingency, a 200 kg reserve and a 50 kg caution threshold.

**This is an educational prototype, not operational aviation software.**

## Dataset Format

The application accepts CSV files with the following header:

```text
flight_id,distance_nm,groundspeed_knots,fuel_burn_kg_per_hour,available_fuel_kg,taxi_fuel_kg
```

Each subsequent non-empty line represents one simulated flight. Fields are separated by commas. Quoted and multiline CSV fields are not supported.

All numeric values must be finite and must satisfy the application's validation rules. If any record is invalid, the dataset is rejected.

A sample dataset is available at `data/flights.csv`.

## Build Instructions

Open the repository folder in Visual Studio with the C++ development workload, MSVC and CMake installed.

Select the `x64-Debug` CMake configuration and build all targets.

## Running the Application

From the repository root, use Developer PowerShell:

```powershell
& ".\out\build\x64-Debug\afrm.exe" ".\data\flights.csv"
```

If Visual Studio uses a different build directory, update the executable path accordingly.

The application displays a fuel assessment for each validated flight followed by the total number of processed flights.

## Automated Verification

Run the CTest suite from Developer PowerShell:

```powershell
ctest --test-dir "out/build/x64-Debug" -C Debug --output-on-failure
```

The automated tests cover development infrastructure, input validation, calculations, fuel-status boundaries, CSV parsing and end-to-end dataset execution.

## Limitations

The application uses simplified, constant-speed and constant-fuel-burn assumptions. It does not model changing flight conditions, aircraft performance envelopes or operational fuel regulations.

CSV support is intentionally restricted to a fixed six-column format. The software is not suitable for actual aircraft operation or flight-planning decisions.