
# AFRM Requirements and Traceability

## Project Scope

Educational Aircraft Fuel and Range Management prototype
using synthetic flight data. Not intended for operational use.

## Functional Requirements

| ID | Requirement | Implementation | Verification |
|---|---|---|---|
| FR01 | Load simulated flights | csv_loader.cpp | afrm_csv_parser |
| FR02 | Validate input values | validation.cpp | afrm_input_validation |
| FR03 | Calculate duration | fuel_calculator.cpp | afrm_fuel_calculations |
| FR04 | Calculate trip fuel | fuel_calculator.cpp | afrm_fuel_calculations |
| FR05 | Calculate contingency | fuel_calculator.cpp | afrm_fuel_calculations |
| FR06 | Calculate required fuel | fuel_calculator.cpp | afrm_fuel_calculations |
| FR07 | Calculate fuel margin | fuel_calculator.cpp | afrm_fuel_calculations |
| FR08 | Classify fuel status | fuel_status.cpp | afrm_fuel_status |
| FR09 | Calculate planning range | fuel_calculator.cpp | afrm_fuel_calculations |
| FR10 | Display results and errors | main.cpp | Manual and end-to-end checks |

## Agreed Simulation Assumptions

- Contingency: 10% of trip fuel
- Reserve: 200 kg
- Caution threshold: 50 kg
- Constant groundspeed and fuel-burn rate
- Simulated inputs only
- No operational aviation use

## Classification Criteria

- Margin below 0 kg: INSUFFICIENT
- Margin from 0 kg to below 50 kg: CAUTION
- Margin of 50 kg or above: SUFFICIENT

## Known Limitations

- Simplified fuel-consumption model
- Fixed-format CSV parser
- No real-time aircraft data
- No aircraft-specific performance modelling
- No certification or operational assurance