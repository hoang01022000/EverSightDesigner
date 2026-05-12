# Runtime Interface Designer

Industrial-grade Qt/QML runtime UI designer.

---

# Product Type

This project is similar to:

- Qt Designer
- WinForms Designer
- Ignition Perspective
- SCADA/HMI runtime builders

This project is NOT:

- node graph editor
- flow editor
- visual scripting system
- vision pipeline editor

---

# Tech Stack

- Qt 6.8
- QML
- C++17
- CMake 3.26+

---

# Architecture Goals

- scalable
- maintainable
- MVVM-friendly
- plugin-friendly
- industrial-grade
- runtime/editor separation

Avoid:
- over-engineering
- generic web architecture
- microservice patterns
- unnecessary abstractions

---

# Core Architecture Rule

EDITOR wraps RUNTIME.

RUNTIME must NEVER depend on EDITOR.

Correct dependency direction:

app
 └── editor
      └── runtime
           └── core
                └── shared

Forbidden:
- runtime -> editor
- core -> qml

---

# Folder Responsibilities

core/
- infrastructure only
- logging
- serialization
- command system
- filesystem
- undo/redo
- event bus

runtime/
- runtime rendering only
- runtime widgets only
- screen rendering
- property evaluation
- runtime interaction

editor/
- editor interaction only
- selection
- resize handles
- snapping
- overlays
- inspector
- palette
- hierarchy

shared/
- enums
- dto
- constants
- interfaces
- common types

qml/components/
- reusable visual primitives only

---

# Important Design Rules

- Do NOT invent architecture
- Do NOT create unnecessary managers
- Do NOT create service locator patterns
- Do NOT create abstract factories unless requested
- Preserve existing working logic
- Prefer incremental refactor
- Refactor structure first
- Features second

---

# Naming Convention

## C++

Class/Struct/Enum:
PascalCase

Functions/Methods:
camelCase

Member variables:
m_memberVariable

Constants:
SCREAMING_SNAKE_CASE

Namespaces:
namespace eversight

Getter:
width()

Setter:
setWidth()

Use:
- nullptr
- override
- explicit
- #pragma once

---

## QML

QML files:
PascalCase.qml

IDs:
camelCase

Properties:
camelCase

Root item id:
root

Signals:
camelCase
without 'on'

---

# Anti-patterns

Avoid:
- WidgetManager
- GlobalManager
- UIManager
- giant god classes
- generic service locator
- over-abstraction
- circular dependency
- runtime/editor coupling

---

# Current Refactor Objective

ONLY reorganize project structure.

DO:
- move files
- fix imports
- fix CMake
- preserve behavior

DO NOT:
- rewrite logic
- redesign systems
- add new features
- introduce plugin implementation
- implement serialization yet

---

# Build Stability

Project must remain buildable after every refactor step.
