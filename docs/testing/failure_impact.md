# AFRM Assignment 2: Software Failure Impact Assessment

**Assessment ID:** RISK-A2-001  
**Original baseline:** `a2-baseline-v1.0.0`  
**Application:** Aircraft Fuel and Range Management educational prototype  
**Status:** Preliminary qualitative assessment, subject to test findings

## 1. Purpose and Scope

This assessment identifies potential software failures affecting the AFRM application's fuel calculations, input validation, fuel-status classification and output processing. It establishes testing priorities according to the potential consequences and plausible occurrence conditions of each failure.

The AFRM uses synthetic flight data and fictional fuel-policy parameters. It is not certified or intended for operational flight planning. The severity ratings reflect hypothetical consequences of relying upon incorrect information rather than an established aircraft system safety classification.

## 2. Risk-Rating Method

Severity (S) is scored from 1 (Low) to 3 (High). Likelihood (L) is scored from 1 (Unlikely) to 3 (More likely). The qualitative priority score is calculated as S × L.

Scores 1–2 are Low, 3–4 are Medium and 6–9 are High. Detectability is assessed qualitatively and may influence testing priority independently of the numerical score.

The ratings represent engineering judgement rather than statistically established failure probabilities.

## 3. Failure Impact Assessment

| ID | Failure mode | Potential consequence | S | L | Risk | Verification |
|---|---|---|---|---|---|---|
| R01 | Incorrect fuel calculation | Understated or overstated fuel requirement | 3 | 2 | High | Independent calculation tests |
| R02 | Incorrect classification | Misleading fuel-status output | 3 | 2 | High | Functional and boundary tests |
| R03 | Incorrect threshold boundary | Misclassification at decision transitions | 3 | 2 | High | Exact and adjacent boundary values |
| R04 | Invalid inputs accepted | Invalid results or interrupted processing | 3 | 1 | Medium | Negative and validation tests |
| R05 | Incorrect measurement units | Plausible but inaccurate calculations | 3 | 2 | High | Input-contract and units review |
| R06 | Numerical overflow or precision error | Incorrect or unavailable results | 3 | 1 | Medium | Numerical stress tests |
| R07 | Misleading output | Incorrect interpretation of displayed results | 2 | 2 | Medium | Output and integration testing |
| R08 | Unexpected termination | Incomplete assessment | 2 | 2 | Medium | Error-path testing |
| R09 | Excessive resource usage | Degraded performance or resource exhaustion | 2 | 1 | Low | Targeted performance measurement |
| R10 | Incorrect dynamic caution threshold | Incorrect classification after planned modification | 3 | 2 | High | New requirements-based and regression tests |

R10 applies to the proposed Assignment 2 change and does not represent a confirmed defect in the original implementation.

## 4. Testing Implications

Highest priority is assigned to independent verification of fuel calculations, status classifications and threshold boundaries. Negative testing will evaluate invalid and extreme numerical inputs, while integration testing will assess complete processing from CSV input to displayed results.

The original test suite provides a starting point. Additional tests will target the proposed dynamic threshold and verify that original fuel calculations and insufficient-fuel classification behaviour remain unchanged.

## 5. Limitations and Judgement

The risk assessment is qualitative and does not establish an aviation Development Assurance Level. The application lacks operational requirements, aircraft-specific performance data and system-level hazard analysis.

The assessment will be reviewed following test execution. Risk ratings and priorities may be revised where actual evidence justifies a change.

The recommended verification approach is proportionate to the educational prototype: prioritise calculation correctness and classification boundaries, retain controlled regression testing and use targeted performance measurements rather than extensive certification-style activities.