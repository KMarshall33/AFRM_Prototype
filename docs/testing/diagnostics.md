# AFRM Assignment 2: Debugging and Performance Assessment

**Production baseline:** `a2-baseline-v1.0.0`  
**Development branch:** `a2/test-harness`  
**Environment:** Visual Studio Community 2026, MSVC, C++20, CMake  
**Build configuration:** x64-Debug  
**Assessment dates:** 11–12 November 2026

## 1. Debugging Investigation

**Objective:** Examine the existing fuel-status classification algorithm and establish the behaviour of the original fixed-threshold implementation.

**Method:** Visual Studio debugger with a breakpoint at the `classifyFuelStatus()` call in `src/main.cpp`. The seven-flight dataset `tests/data/threshold_scenarios.csv` was used.

### Observed Results

| Variable | Observed value |
|---|---|
| Flight identifier | A2-DYN-LOW |
| Total required fuel | 550 kg |
| Fuel margin | 54 kg |
| Caution threshold | 50 kg |
| Classification | SUFFICIENT |

The debugger confirmed that neither the negative-margin condition nor the below-caution-threshold condition was satisfied. The existing classifier therefore returned `SUFFICIENT`.

This behaviour is correct under the original fixed-threshold specification.

### Engineering Assessment

The investigation confirmed that classification currently uses a fixed 50 kg threshold regardless of the calculated fuel requirement.

Under the proposed dynamic-threshold rule, the same flight would have a threshold of 55 kg and would consequently be classified as `CAUTION`.

This is an identified enhancement opportunity rather than a demonstrated defect in the existing software.

## 2. Baseline Performance Measurement

**Objective:** Establish a repeatable end-to-end execution reference for comparison with the revised application.

**Dataset:** 5,000 synthetic flights, each using distance 200 NM, groundspeed 200 knots, fuel burn 300 kg/hour, available fuel 604 kg and taxi fuel 20 kg.

**Method:** One warm-up execution followed by five measured executions using PowerShell's `System.Diagnostics.Stopwatch`. Console output was redirected to `$null`.

### Recorded Results

| Run | Execution time (ms) |
|---|---:|
| 1 | 288.18 |
| 2 | 304.33 |
| 3 | 285.50 |
| 4 | 286.62 |
| 5 | 290.48 |
| **Median** | **288.18** |
| Mean | 291.02 |
| Minimum | 285.50 |
| Maximum | 304.33 |

### Evaluation

All five measured executions completed successfully.

The measurements include application startup, CSV parsing, calculations, classification and output formatting. They do not isolate the computational cost of fuel-status classification.

The results demonstrate observable timing variation. Consequently, small timing differences between software versions should not be interpreted as meaningful performance changes without consideration of measurement variability.

The measurements used a Debug configuration and should not be treated as representative of optimised production performance.

## 3. Conclusion and Reassessment

Debugging confirmed that the original AFRM implements its specified fixed caution threshold correctly. The investigation supports the proposed change to introduce a requirement-dependent caution threshold.

The recorded performance measurements provide a preliminary baseline. The modified application will be assessed using the same dataset, build configuration and measurement procedure.

The revised software will be evaluated primarily for functional correctness and regression behaviour, with performance treated as supporting evidence.