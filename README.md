# SuperJupiter SignalFilter

<!-- SUPERJUPITER-CONSOLIDATION-MIGRATION:START -->
## Consolidated library available

For new projects, start with **[SuperJupiter Sensor Toolkit](https://github.com/Super-Jupiter/superjupiter-sensor-toolkit)**. This standalone package and its published releases remain available. Existing installations need no change.

If migrating with PlatformIO, replace the standalone dependency with:

```ini
lib_deps = superjupiter/SuperJupiterSensorToolkit@0.1.0
```

Replace this standalone include:

```cpp
#include <SuperJupiterSignalFilter.h>
```

with this consolidated include:

```cpp
#include <SuperJupiter/SensorToolkit/SignalFilter.h>
```

The component namespace and behaviour are preserved from standalone **0.1.1**. If using an earlier version, review its API migration requirements first. Use one implementation of this component per application; do not mix standalone and consolidated copies.

See the [consolidated migration guide](https://github.com/Super-Jupiter/superjupiter-sensor-toolkit/blob/v0.1.0/docs/MIGRATION.md) and consolidated README for Arduino/CMake installation. **Hardware validation pending.**
<!-- SUPERJUPITER-CONSOLIDATION-MIGRATION:END -->

Elapsed-time exponential smoothing with explicit gap and input handling.

**Version 0.1.1: evolving preview. Hardware validation pending.**

Original MIT-licensed portable C++11 by [SuperJupiter](https://www.superjupiter.net).
No mandatory service, hidden clock reads, network calls or heap allocation in the core.

## What it does

The filter uses alpha=-expm1(-elapsed/tau) for positive finite tauMs.
First valid observation seeds the state; a gap beyond maxGapMs reseeds rather than bridging missing data.
Duplicate/backward timestamps and invalid numeric inputs leave state unchanged.
Timestamps must advance by less than 2^31 ms; maxGapMs is in that same horizon.
Output is a derived value only: applications should retain raw observations separately. No anti-aliasing guarantee.

## Install

PlatformIO exact release (after registry publication):

```ini
lib_deps = superjupiter/SuperJupiterSignalFilter@0.1.1
```

Arduino: use the versioned [GitHub release ZIP](https://github.com/Super-Jupiter/superjupiter-signal-filter/releases/tag/v0.1.1).
Library Manager registration and index propagation are separate steps; check the official index before relying on IDE availability.

## Example

```cpp
#include <SuperJupiterSignalFilter.h>
using namespace superjupiter::signal_filter;
int main() {
SignalFilter filter(Config(1000,5000));
filter.push(20,0);
Result result=filter.push(21,100);
(void)result;
return 0;
}

```

For Arduino, open `examples/Minimal`. It uses synthetic data only.
Results are deliberately local so applications can inspect outcomes before adding I/O.

## CMake

```cmake
add_subdirectory(path/to/superjupiter-signal-filter)
target_link_libraries(your_app PRIVATE SuperJupiter::SignalFilter)
```

For an installed package use `find_package(SuperJupiterSignalFilter 0.1 CONFIG REQUIRED)`.
Top-level CMake builds tests/examples; a subproject does not enable developer tools by default.

## Documentation and verification

- [API and boundaries](https://github.com/Super-Jupiter/superjupiter-signal-filter/blob/v0.1.1/docs/API.md)
- [Tests](https://github.com/Super-Jupiter/superjupiter-signal-filter/blob/v0.1.1/tests/test.cpp)
- [Validation status](https://github.com/Super-Jupiter/superjupiter-signal-filter/blob/v0.1.1/docs/VALIDATION.md)
- [Shared conventions](https://github.com/Super-Jupiter/superjupiter-signal-filter/blob/v0.1.1/docs/CONVENTIONS.md)
- [Migration policy](https://github.com/Super-Jupiter/superjupiter-signal-filter/blob/v0.1.1/docs/MIGRATION.md)

```sh
cmake -S . -B build/cmake
cmake --build build/cmake
ctest --test-dir build/cmake --output-on-failure
python3 tools/check_release.py --tag v0.1.1
```

Keep raw observations and their provenance in your application. Derived results are not physical truth.
Do not use fast-math or finite-math-only compiler assumptions. AVR double may have float precision.
Software tests, board compilation and physical validation are reported separately.

## Fits the SuperJupiter measurement portfolio

Complements [SensorQuality](https://github.com/Super-Jupiter/superjupiter-sensor-quality) and
[SampleWindow](https://github.com/Super-Jupiter/superjupiter-sample-window) without a required dependency.
Each component can be used independently. Domain models, customer data and proprietary applications are excluded.

## Support and licence

MIT. Report reproducible bugs through repository issues; private security reports: info@superjupiter.net.
Include version, configuration, inputs, expected and observed behaviour. No credentials or private customer data.


## Compatibility review 0.1.1

This patch includes independent regression tests and a reviewed [interoperability contract](https://github.com/Super-Jupiter/superjupiter-signal-filter/blob/v0.1.1/docs/INTEROPERABILITY.md). Hardware validation pending.

- Removed unused duplicated math helpers and unnecessary includes.
- Stable convex smoothing preserves constant extreme values.
