# AFRM Reproducibility and Technical Review

**Project:** Aircraft Fuel and Range Management (AFRM) educational prototype  
**Review documented:** 28 October 2026  
**Review type:** Fresh-clone reproducibility check and technical limitations review  
**Repository:** https://github.com/KMarshall33/AFRM_Prototype  
**Branch:** `main`  
**Repository HEAD observed when this record was prepared:** `19e7b399f6d49c6e3799d0aaf748c76b672e06e8`  

## 1. Development and verification environment

| Component | Configuration |
|---|---|
| Operating environment | Windows-based, x64 C++ desktop development |
| IDE | Microsoft Visual Studio **Community 2026**, **18.11.0** |
| Language | C++20 |
| Compiler | Microsoft Visual C++ (MSVC), x64/x86 Build Tools (Latest) |
| Build system | CMake, Visual Studio C++ CMake tools for Windows |
| Build configuration | `x64-Debug` |
| Compiler options | `/W4`, `/permissive-` |
| Windows SDK | Windows 11 SDK **10.0.26100.8249** |
| Test runner | CTest |
| Version control | Git, hosted on GitHub |
| Third-party application libraries | No additional third-party runtime library introduced by this prototype | 


## 2. Clean-clone verification

The repository was cloned to a separate verification workspace and the checks completed normally.

| Verification check | Recorded result |
|---|---|---|
| Clone the GitHub repository into a separate directory | **PASS** |
| Confirm source code, CMake configuration, datasets, tests and documentation are present | **PASS** |
| Configure the fresh project with CMake in Visual Studio | **PASS** |
| Build all C++ targets from the clean clone using `x64-Debug` | **PASS** |
| Execute all six registered CTest tests | **PASS (6/6 reported)** |
| Run the sample CSV dataset through the rebuilt executable | **PASS** |
| Confirm all six calculated fuel-status results | **PASS** |
| Abnormal configuration changes, build errors or unexpected behaviour | **None** |

### Reference dataset expectations

The dataset `data/flights.csv` uses a fictional 10% contingency allowance, 200 kg reserve and 50 kg caution threshold.

| Flight | Expected margin | Expected classification |
|---|---:|---|
| AFRM-001 | +60 kg | SUFFICIENT |
| AFRM-002 | +50 kg | SUFFICIENT |
| AFRM-003 | +49 kg | CAUTION |
| AFRM-004 | 0 kg | CAUTION |
| AFRM-005 | −1 kg | INSUFFICIENT |
| AFRM-006 | +30 kg | CAUTION |

The separate repository verification record (`docs/verification.md`) contains an earlier **six-tests-passed** CTest transcript for the original development workspace (reported total real time: 0.31 seconds).

## 3. Findings and reproducibility judgement

**Result:** The clean clone was able to configure, build and execute the six automated tests and process the synthetic dataset successfully without unexpected behaviour. This supports a **qualified reproducibility conclusion for the same Windows/Visual Studio/MSVC toolchain**.

## 4. Technical limitations and sustainability implications

| Limitation | Potential impact | Evaluation / disposition |
|---|---|---|
| Constant groundspeed and fuel-burn assumptions | The model does not represent real-world flight variability | Accept for this educational prototype; document non-operational scope |
| Fictional contingency, reserve and caution criteria | Results must not be interpreted as regulatory or operational thresholds | Retain clearly labelled simulation assumptions |
| Floating-point output rounded to two decimal places | Displayed margin can appear to equal a status boundary although the internal value is slightly below it | **Deferred:** document precision limitation; future improvement could align presentation with decision precision |
| Restricted six-column CSV parser | Does not support general quoted or multiline CSV records | Accept for controlled synthetic data; future enhancement only if input scope expands |
| Whole-file CSV parsing and storage | Memory demand increases with input size; no explicit maximum dataset size | Suitable for current small dataset; evaluate scaling rather than claiming efficiency without measurements |
| Sequential output during batch calculation | A later arithmetic failure can follow already printed flight results | **Deferred:** consider precomputing all results before displaying a batch |
| End-to-end CTest checks `Processed 6 flights` | The end-to-end test checks count rather than all output values | **Deferred:** stronger full-output assertions would improve independent verification |
| Windows/MSVC toolchain validation only | Portability to other platforms and compilers is unverified | Avoid cross-platform claims; CMake and standard-library choices reduce some migration effort but do not prove portability |
| No direct energy measurements | Energy or carbon reductions cannot be quantified from tests alone | Investigate elapsed time, memory and scaling separately; identify them as resource proxies, not measured energy |

The implementation uses separate CSV input, validation, calculation and status modules, with a shared `afrm_core` target and six CTest registrations. This separation supports understanding, modification and focused testing. The trade-off is additional source and test files to maintain, plus responsibility for the bespoke CSV parser. Avoiding additional application libraries simplifies dependency management, but long-term compatibility still depends on supported compilers, toolchain updates and the C++ standard library.

## 5. Improvement decisions

The following improvements were identified during the technical review but **are intentionally deferred** to prioritise assessment reporting and sustainability evaluation:

1. Make fuel-margin presentation unambiguous at floating-point classification boundaries.
2. Avoid partial console output if a later flight's calculation fails.
3. Strengthen the end-to-end test so it independently checks status and margin output rather than only the flight count.
4. Consider larger dataset handling and resource-efficiency evidence before implementing additional features.

**Justification:** The agreed educational functionality and existing automated test suite have been reported as successful. Further implementation changes would introduce additional verification work. The next priority is an evidence-based critical evaluation of the design's resource demands, maintainability, dependencies and obsolescence risks, rather than expanding scope.

## 6. Final assessment-relevant statement

The reported clean-clone check indicates that the AFRM can be reproduced within the documented Windows/Visual Studio/MSVC environment from its GitHub repository. Its CMake configuration, modular source and CTest suite provide a traceable development baseline. Reproducibility is **supported within the tested toolchain**, not universally demonstrated; resource efficiency and energy use must be investigated and discussed separately.
