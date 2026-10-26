
# AFRM Verification Record

## Environment

- IDE: Visual Studio Community 2026
- Compiler: MSVC
- Language standard: C++20
- Build system: CMake
- Test runner: CTest
- Build configuration: x64-Debug

## Automated Verification

| Test | Result |
|---|---|
| afrm_test_infrastructure | Pass |
| afrm_input_validation | Pass |
| afrm_fuel_calculations | Pass |
| afrm_fuel_status | Pass |
| afrm_csv_parser | Pass |
| afrm_csv_example | Pass |

## Reference Flight

AFRM-001 inputs:
- Distance: 200 NM
- Groundspeed: 200 knots
- Fuel burn: 200 kg/hour
- Available fuel: 500 kg
- Taxi fuel: 20 kg

Expected outputs:
- Trip fuel: 200 kg
- Contingency: 20 kg
- Required fuel: 440 kg
- Fuel margin: 60 kg
- Planning range: 254.55 NM
- Classification: SUFFICIENT

## Boundary Cases

| Available fuel | Expected margin | Expected status |
|---|---|---|
| 490 kg | 50 kg | SUFFICIENT |
| 489 kg | 49 kg | CAUTION |
| 440 kg | 0 kg | CAUTION |
| 439 kg | -1 kg | INSUFFICIENT |

## Verification Notes

```
Test project C:/Users/kyler/source/repos/AFRM_Prototype/out/build/x64-Debug
    Start 1: afrm_test_infrastructure
1/6 Test #1: afrm_test_infrastructure .........   Passed    0.04 sec
    Start 2: afrm_input_validation
2/6 Test #2: afrm_input_validation ............   Passed    0.04 sec
    Start 3: afrm_fuel_calculations
3/6 Test #3: afrm_fuel_calculations ...........   Passed    0.04 sec
    Start 4: afrm_fuel_status
4/6 Test #4: afrm_fuel_status .................   Passed    0.04 sec
    Start 5: afrm_csv_parser
5/6 Test #5: afrm_csv_parser ..................   Passed    0.04 sec
    Start 6: afrm_csv_example
6/6 Test #6: afrm_csv_example .................   Passed    0.05 sec

100% tests passed, 0 tests failed out of 6

Total Test time (real) =   0.31 sec
```

Commit ID: e17d5e5