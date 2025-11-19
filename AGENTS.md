# Agent Instructions for xTrainAPI

This document provides guidance for AI agents working on this repository.

## Source of Truth

The canonical definition of the xTrainAPI is the C++ abstract interface located at:

`/xDuinoRails_xTrainAPI.h`

This header file is the **single source of truth** for all API events, data structures, and enums.

## Derivative Artifacts

The following files are derivatives of the main `.h` file and **must** be kept in sync with it:

1.  **XML Schema:** `/xml/xTrainEvents.xsd`
2.  **OpenAPI Schema:** `/swagger/openapi.yaml`
3.  **Utility Implementations:** `/xDuinoRails_xTrainAPI_utils.h` (specifically the `CmdLinePrinter`, `XmlPrinter`, and `CmdLineParser` classes).

### Agent Responsibility

When making changes to the API, you **must** follow this workflow:

1.  **Modify the `.h` file first.** All new events, or changes to existing ones, should be implemented in `xDuinoRails_xTrainAPI.h`.
2.  **Update the derivatives.** Propagate the changes to the `.xsd`, `openapi.yaml`, and `xDuinoRails_xTrainAPI_utils.h` files to reflect the modifications made to the header.
3.  **Run validation.** Ensure that the XML and OpenAPI validation checks pass after your changes.

Failure to keep these files consistent will result in build failures and a broken API contract. Always check for deviations from the main `.h` file and correct them.

## Language

All communication, and code comments must be in English. This ensures that the project is accessible to a global audience.

## Documentation Language

Create all documentation files in both German and English.

-   **German:** Files must end with the `.de.md` extension.
-   **English:** Files must end with the `.en.md` extension.

## File Naming Conventions

Documentation files should be in `UPPER_SNAKE_CASE` (e.g., `DATA_DICTIONARY.md`). Standard files like `README.md`, `AGENTS.md`, and `LICENSE` are exceptions to this rule.
