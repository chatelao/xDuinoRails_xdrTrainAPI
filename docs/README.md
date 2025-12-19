# xTrainAPI - Unified Model Train Control

Welcome to the xTrainAPI project! This repository contains a C++-based API for controlling digital model trains and their accessories.

## Summary

xTrainAPI is a unified interface designed to control various digital model train systems through a single, consistent API. It simplifies the development of control software by abstracting the complexities of individual protocols, such as DCC and Märklin Motorola.

The API is defined in C++ as an abstract interface (`IUnifiedModelTrainListener`) and serves as the "Single Source of Truth." All other artifacts, including XML schemas, the OpenAPI specification, and utility classes, are derived from this central C++ header file. This repository provides the core API, documentation, and a set of examples to help you get started.

## Documentation

Our API documentation is automatically generated and hosted on GitHub Pages.

*   **Doxygen (C++ API):** Detailed documentation of the C++ interface, classes, and methods can be found here:
    [Doxygen Documentation](https://chatelao.github.io/xDuinoRails_xTrainAPI/doxygen)

*   **Swagger (REST API):** The specification for our REST interface, including interactive endpoints, is available here:
    [Swagger UI](https://chatelao.github.io/xDuinoRails_xTrainAPI/swagger)

*   **Web Tool:** A simple web application for interacting with the xTrainAPI is available here:
    [Web Tool](https://chatelao.github.io/xDuinoRails_xTrainAPI/webtool)

## Core Concepts

The API is defined in C++ as an abstract interface (`IUnifiedModelTrainListener`) and serves as the "Single Source of Truth". All other artifacts, such as the XML schemas and the OpenAPI specification, are derived from this C++ header file.

For a deeper understanding of the terms and data fields used in the API, please take a look at our [Glossary](XTTRAIN_GLOSSARY.md) and the [Data Dictionary](../spec/DATA_DICTIONARY.md).

## Repository Structure

The repository is organized as follows:

-   `/src`: Contains the main C++ API definition and utility classes (including `yxml`).
-   `/docs`: Contains additional documentation, such as the CLI syntax.
-   `/examples`: Arduino sketches demonstrating how to use the API.
-   `/json`: JSON schemas for the API events.
-   `/swagger`: The OpenAPI 3.0 specification for the REST API.
-   `/webtool`: A web-based tool for interacting with the API.
-   `/xml`: XML schemas and examples.

## Getting Started

To use the xTrainAPI in your own Arduino projects, follow these steps:

1.  **Install the Library:**
    *   Download the repository as a ZIP file.
    *   In the Arduino IDE, go to **Sketch** -> **Include Library** -> **Add .ZIP Library...** and select the downloaded file.
    *   Alternatively, you can install it manually by copying the repository folder into your Arduino `libraries` folder.

2.  **Implement the Interface:** Create a new class in your sketch that inherits from `IUnifiedModelTrainListener` and implement the methods for the events you want to handle.
3.  **Process Events:** In your `loop()` function, you can use one of the utility classes (like `CmdLineParser` or `XmlParser`) to parse incoming data and trigger the corresponding events in your listener class.

## Running the Examples

The `/examples` directory contains several sketches that demonstrate the functionality of the API. To run them, you will need to have the Arduino IDE or `arduino-cli` installed.

1.  Open one of the example sketches (e.g., `examples/debug_cli/debug_cli.ino`) in the Arduino IDE.
2.  Select your board and port.
3.  Upload the sketch to your Arduino.
4.  Open the Serial Monitor to interact with the example.

## Contributing

We welcome contributions! Please read our `AGENTS.md` before making any changes to the code.
