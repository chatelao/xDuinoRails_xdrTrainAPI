# xTrainAPI - Unified Model Train Control

Welcome to the xTrainAPI project! This repository contains a C++-based API for controlling digital model trains and their accessories.

## Documentation

Our API documentation is automatically generated and hosted on GitHub Pages.

*   **Doxygen (C++ API):** Detailed documentation of the C++ interface, classes, and methods can be found here:
    [Doxygen Documentation](https://xtrainapi.github.io/xTrainAPI/doxygen/)

*   **Swagger (REST API):** The specification for our REST interface, including interactive endpoints, is available here:
    [Swagger UI](https://xtrainapi.github.io/xTrainAPI/swagger/)

## What is xTrainAPI?

xTrainAPI is a unified interface that allows various digital model train systems to be controlled through a single, consistent API. This simplifies the development of control software by abstracting the complexity of individual protocols (such as DCC, Märklin Motorola, etc.).

## Core Concepts

The API is defined in C++ as an abstract interface (`IUnifiedModelTrainListener`) and serves as the "Single Source of Truth". All other artifacts, such as the XML schemas and the OpenAPI specification, are derived from this C++ header file.

For a deeper understanding of the terms and data fields used in the API, please take a look at our [Glossary](XTTRAIN_GLOSSARY.md) and the [Data Dictionary](DATA_DICTIONARY.md).

## Contributing

We welcome contributions! Please read our `AGENTS.md` before making any changes to the code.
