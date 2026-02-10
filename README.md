# ACF-Solutions

Solutions for ACF (Application Component Framework) Toolkit - A comprehensive collection of components and utilities built on the Application Component Framework.

## Overview

ACF-Solutions provides a rich set of components for building advanced applications including:

- **Image Processing**: Components for image analysis, measurement, and processing
- **Measurement Tools**: Data acquisition and analysis components
- **GUI Components**: Qt-based user interface components
- **File Processing**: File management and processing utilities
- **Calibration**: Camera and lens calibration tools
- **Communication**: Network and inter-process communication
- **Blob Analysis**: Shape detection and analysis
- **Edge Detection**: Advanced edge detection algorithms
- **Authorization**: User authentication and rights management

## License

This project is licensed under the GNU Lesser General Public License v2.1 (LGPL-2.1) or later. See the [LICENSE](LICENSE) file for details.

All source files include SPDX license identifiers for machine-readable license information:
```cpp
// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
```

## Dependencies

- **ACF Framework**: The base Application Component Framework
- **Qt Framework**: Version 5.x or 6.x for GUI and core functionality
- **C++ Compiler**: Supporting C++11 or later

## Building

The project can be built using CMake:

```bash
cd Build/CMake
cmake .
cmake --build .
```

## Security

For security policy and vulnerability reporting, see [SECURITY.md](SECURITY.md).

## EU Cyber Resilience Act (CRA) Compliance

This project complies with the EU Cyber Resilience Act requirements. See:

- [CRA_COMPLIANCE.md](CRA_COMPLIANCE.md) - Full CRA compliance documentation
- [SBOM.md](SBOM.md) - Software Bill of Materials guide
- [sbom.json](sbom.json) - CycloneDX format SBOM
- [sbom.spdx.json](sbom.spdx.json) - SPDX format SBOM

## Documentation

The repository includes:

- **Include/**: Header files and interfaces
- **Impl/**: Implementation packages
- **Partitura/**: Application definitions
- **Docs/**: Documentation and tutorials
- **Tests/**: Test suites
- **Config/**: Configuration files
- **Build/**: Build system files

## Contributing

Contributions are welcome! Please ensure:

- Follow the existing code style
- Add SPDX license headers to new files
- Update documentation as needed
- Test changes thoroughly

## Contact

- **Repository**: https://github.com/ImagingTools/AcfSln
- **Issues**: https://github.com/ImagingTools/AcfSln/issues
- **Security Issues**: See [SECURITY.md](SECURITY.md)

## Related Projects

- [ACF Framework](https://github.com/ImagingTools/Acf) - The base Application Component Framework

---

Copyright (C) 2007-2014 Witold Gantzke & Kirill Lepskiy
