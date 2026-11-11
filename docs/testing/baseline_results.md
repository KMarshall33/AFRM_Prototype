# Assignment 2: Baseline Test Results

**Production baseline:** `a2-baseline-v1.0.0`  
**Production commit:** `6c848e537885f323677809d52352dc5d0fefa716`  
**Test-harness commit:** `f5ba2e733f44f7597af3a3cb1ab3655c7fd5e1d1`  
**Environment:** Visual Studio, MSVC, C++20, CMake and CTest  
**Configuration:** x64-Debug  
**Execution date:** 11 November 2026

## Test Execution Summary

| Test | Result |
|---|---|
| afrm_test_infrastructure | Pass |
| afrm_input_validation | Pass |
| afrm_fuel_calculations | Pass |
| afrm_fuel_status | Pass |
| afrm_csv_parser | Pass |
| afrm_csv_example | Pass |
| afrm_threshold_characterisation | Pass |

**Total:** 7 passed, 0 failed (100% pass rate).  
**Total CTest execution time:** 0.18 seconds.

## Threshold Characterisation

The additional test evaluates seven synthetic flight scenarios using the original fixed 50 kg caution threshold.

**Expected baseline behaviour:** The application uses a fixed caution threshold of 50 kg regardless of the total calculated fuel requirement.

**Observed result:** The `afrm_threshold_characterisation` test passed, confirming that all seven scenarios satisfied the automated assertions for required fuel, fuel margin and fuel-status classification under the existing fixed-threshold policy.

The original six CTest registrations also passed, providing evidence that the extended test infrastructure has not disrupted the existing verification suite.

## Assessment

The baseline test results provide evidence that the original AFRM implementation behaves consistently with its specified calculations, validation rules and fixed fuel-status thresholds for the executed test cases.

The additional threshold-characterisation test establishes a reference against which the proposed dynamic caution-threshold algorithm can be evaluated. This will allow subsequent testing to distinguish intended changes in classification from unintended regressions.

However, the results do not establish exhaustive correctness or operational aviation suitability. The tests exercise selected requirements and input conditions, while the application retains simplified flight assumptions. The reported 0.18-second CTest execution time includes test orchestration and is not a controlled application-performance benchmark.

The baseline results will be compared with the revised dynamic-threshold implementation following CR-001.