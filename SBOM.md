# Software Bill of Materials (SBOM) Guide

## Overview

This document describes how to generate and use Software Bill of Materials (SBOM) for the ACF-Solutions project to support EU Cyber Resilience Act (CRA) compliance.

## What is an SBOM?

A Software Bill of Materials (SBOM) is a comprehensive inventory of all components, libraries, and dependencies used in a software product. It helps with:

- **Transparency**: Understanding what's in your software
- **Security**: Identifying vulnerable components
- **Compliance**: Meeting regulatory requirements like the CRA
- **Risk Management**: Assessing supply chain risks

## Pre-Generated SBOM

ACF-Solutions provides pre-generated SBOMs in both major formats:

### CycloneDX Format
A basic SBOM for ACF-Solutions is provided in [`sbom.json`](sbom.json) in CycloneDX 1.5 format. This includes:

- ACF-Solutions toolkit information
- Main runtime dependencies (ACF, Qt Framework)
- License information
- Project metadata

### SPDX Format
An SPDX 2.3 format SBOM is provided in [`sbom.spdx.json`](sbom.spdx.json). This includes:

- ACF-Solutions toolkit information
- Main runtime dependencies (ACF, Qt Framework)
- License information (using SPDX license identifiers)
- Package relationships

**Important Notes**:
- The version in both SBOM files uses `1.0.0-dev` as a placeholder
- **Maintainers should update both files with each release** to reflect:
  - Current ACF-Solutions version number
  - Creation timestamps (especially in SPDX format)
  - Document namespace UUID for SPDX (generate a new UUID for each release)
  - Specific Qt and ACF versions used
  - Any dependency changes
- Both SBOMs should be regenerated or updated before each release

**Update Schedule**:
- Per Release: Update version numbers, timestamps, UUIDs, and dependency information
- As Needed: When dependencies are added, removed, or updated
- Quarterly: Verify all information is current

## SBOM Standards

ACF-Solutions supports the following SBOM formats:

1. **CycloneDX** (Recommended)
   - Format: JSON or XML
   - Version: 1.5
   - Specification: https://cyclonedx.org/

2. **SPDX** (Alternative)
   - Format: JSON, YAML, or RDF
   - Version: 2.3
   - Specification: https://spdx.dev/

## Generating SBOM for Your Project

If you're using ACF-Solutions in your project, you need to generate an SBOM that includes ACF-Solutions and all its dependencies.

### Method 1: Using CycloneDX Tools

#### For CMake Projects

1. Install CycloneDX CMake plugin:
```bash
# Add to your CMakeLists.txt
find_package(CycloneDX QUIET)
if(CycloneDX_FOUND)
    cyclonedx_generate_sbom(TARGET your_target)
endif()
```

2. Generate SBOM:
```bash
cmake --build . --target cyclonedx-sbom
```

#### For C++ Projects with Conan

```bash
# Install the plugin
pip install cyclonedx-conan

# Generate SBOM
conan install . --install-folder=build
cyclonedx-conan sbom -o sbom.json
```

### Method 2: Using SPDX Tools

#### Using spdx-sbom-generator

```bash
# Install the tool
pip install spdx-sbom-generator

# Generate SBOM for your project
spdx-sbom-generator -p /path/to/your/project -o sbom.spdx.json
```

#### Manual SBOM Creation

You can manually create an SBOM by:

1. Starting with the provided templates (sbom.json or sbom.spdx.json)
2. Adding your project information
3. Including ACF-Solutions as a dependency
4. Listing all other dependencies

### Method 3: Using Syft

Syft can generate SBOMs from various sources:

```bash
# Install Syft
# See: https://github.com/anchore/syft

# Generate CycloneDX SBOM
syft packages dir:/path/to/your/project -o cyclonedx-json > sbom.json

# Generate SPDX SBOM
syft packages dir:/path/to/your/project -o spdx-json > sbom.spdx.json
```

## ACF-Solutions Dependencies

### Runtime Dependencies

1. **ACF (Application Component Framework)**
   - Version: Latest or compatible version
   - License: LGPL-2.1-or-later
   - Source: https://github.com/ImagingTools/Acf
   - Purpose: Core component framework

2. **Qt Framework**
   - Version: 5.x or 6.x
   - License: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only OR Commercial
   - Source: https://www.qt.io/
   - Purpose: Cross-platform UI and core libraries

3. **Standard C++ Library**
   - Version: Compiler-dependent
   - License: Implementation-dependent
   - Purpose: Standard C++ functionality

### Build-Time Dependencies

1. **CMake**
   - Version: 3.x or later
   - License: BSD-3-Clause
   - Purpose: Build system

2. **QMake**
   - Version: Part of Qt
   - License: Part of Qt Framework
   - Purpose: Alternative build system

3. **C++ Compiler**
   - Version: C++11 or later support
   - Examples: GCC, Clang, MSVC
   - Purpose: Source code compilation

## SBOM Maintenance

### When to Update SBOM

Update your SBOM when:

- **New Release**: Before every release
- **Dependency Changes**: When adding, removing, or updating dependencies
- **Security Updates**: When security vulnerabilities are addressed
- **Quarterly Reviews**: Regular verification of accuracy

### SBOM Validation

Validate your SBOM using:

#### For CycloneDX:
```bash
# Install validator
npm install -g @cyclonedx/cyclonedx-cli

# Validate SBOM
cyclonedx-cli validate --input-file sbom.json
```

#### For SPDX:
```bash
# Install validator
pip install spdx-tools

# Validate SBOM
pyspdxtools -i sbom.spdx.json --validate
```

## Using SBOM for Vulnerability Scanning

### With Grype (Anchore)

```bash
# Install Grype
# See: https://github.com/anchore/grype

# Scan SBOM for vulnerabilities
grype sbom:./sbom.json
```

### With OSV-Scanner

```bash
# Install OSV-Scanner
go install github.com/google/osv-scanner/cmd/osv-scanner@latest

# Scan SBOM
osv-scanner --sbom=sbom.json
```

### With Dependency-Track

Upload your SBOM to [OWASP Dependency-Track](https://dependencytrack.org/) for continuous vulnerability monitoring.

## SBOM Format Comparison

| Feature | CycloneDX | SPDX |
|---------|-----------|------|
| Focus | Security & License | License & Legal |
| Vulnerability tracking | Native support | Limited |
| License expression | Flexible | Standardized |
| Tool ecosystem | Growing | Mature |
| EU CRA compliance | Good | Good |

## Best Practices

1. **Keep SBOM Current**: Update with every release
2. **Include All Dependencies**: Don't forget transitive dependencies
3. **Document Versions**: Always specify exact versions
4. **Validate Regularly**: Use validation tools
5. **Automate Generation**: Integrate into CI/CD pipeline
6. **Monitor Vulnerabilities**: Use SBOM for continuous scanning
7. **Archive SBOMs**: Keep historical records for compliance

## CRA Compliance

For EU Cyber Resilience Act compliance:

1. **Generate SBOM**: For every release
2. **Include in Distribution**: Make available to users
3. **Update Regularly**: When dependencies change
4. **Document Dependencies**: Include all components
5. **Track Vulnerabilities**: Monitor SBOM for known issues

## Support and Questions

For questions about SBOM generation or maintenance:

- Open an issue on GitHub
- Refer to [CRA_COMPLIANCE.md](CRA_COMPLIANCE.md)
- Check [SECURITY.md](SECURITY.md) for security-related questions

## Additional Resources

- [CycloneDX Specification](https://cyclonedx.org/specification/overview/)
- [SPDX Specification](https://spdx.github.io/spdx-spec/)
- [NTIA Minimum Elements for SBOM](https://www.ntia.gov/sites/default/files/publications/sbom_minimum_elements_report_0.pdf)
- [EU Cyber Resilience Act](https://digital-strategy.ec.europa.eu/en/library/cyber-resilience-act)

**Last Updated**: February 2026
