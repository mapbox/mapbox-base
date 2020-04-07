# Mapbox Base

## master

### ✨ New features
- [extras] Bump googletest to 1.10.0
    - Better gmock syntax.

## v1.3.0

### ✨ New features
- [base] Add platform definitions

### ⚠️ Breaking changes
- [base] jni.hpp @ e2bbae090005ac1ce2a88c51c0cff2d22d957545

### 💫️ Other
- [build] Update LICENSE.thirdparty

## v1.2.1

### 💫️ Other
- Add `MAPBOX_BASE_BUILD_TESTING` option for overriding building tests when building outside of mapbox-base project.

## v1.2.0

### ✨ New features
- Update minimist dependency to v1.2.5
- [base] Added GeoJSON VT and ShelfPack

### ⚠️ Breaking changes
- Move mapbox-base-owned libraries to a single folder
- [base] cheap-ruler-cpp @ 746044cfbb7d254f1d67929eb564a0dcaaa39cc1
- [base] jni.hpp @ 385dede669c843144b9ad68bbb7af3989d76104e
- [base] geometry.hpp @ a5571a3ace5853e0d1e8d5fbdc87163c824ebeb7

### 💫️ Other
- Use googletest framework

## v1.1.0

### ✨ New features
Extras:
- extras/args @ 6.2.2-9-g401663c
- extras/expected-lite @ v0.3.0-6-g6fad61b
- extras/filesystem @ v1.2.4-7-g135015f
- extras/kdbush.hpp @ v0.1.3-1-ge1e847b
- extras/rapidjson @ v1.1.0-490-gd87b698d

New libraries:
- mapbox/io
- mapbox/typewrapper
- mapbox/value
- mapbox/weak

### ⚠️ Breaking changes
- mapbox/geojson.hpp @ v0.4.3
- mapbox/geometry.hpp @ v1.0.0-8-gb3f4bd4
- mapbox/jni.hpp @ v4.0.1
- mapbox/optional @ f6249e7f
- mapbox/supercluster.hpp @ v0.3.2-1-g03c026c
- mapbox/variant @ v1.1.6

## v1.0.0

### ✨ New features
- geometry.hpp @ v1.0.0
- variant @ v1.1.6
