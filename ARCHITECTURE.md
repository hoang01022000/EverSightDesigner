# Runtime Interface Designer — Architecture Notes

## Architectural Philosophy

This project is an industrial-grade runtime UI designer.

Think:

- Qt Creator mini
- WinForms Designer
- SCADA/HMI builder
- Runtime screen designer

NOT:

- node graph editor
- flow editor
- visual scripting tool

---

# Most Important Principle

EDITOR wraps RUNTIME.

Runtime must stay clean and independent.

Editor manipulates runtime objects but runtime must never depend on editor.

---

# Runtime Responsibilities

Runtime owns:

- widget tree
- rendering
- runtime interaction
- runtime properties
- bindings
- screen rendering
- runtime lifecycle

Runtime should be usable without the editor.

Future possibility:

Player.exe

should work using runtime only.

---

# Editor Responsibilities

Editor owns:

- selection
- resize handles
- snapping
- guides
- overlays
- inspector
- palette
- hierarchy
- undo/redo commands
- clipboard

Editor should manipulate runtime/document state.

---

# Shared Responsibilities

Shared layer owns:

- enums
- constants
- DTOs
- interfaces
- common identifiers

Avoid business logic here.

---

# Core Responsibilities

Core layer owns:

- logging
- serialization
- command system
- filesystem
- undo/redo infrastructure
- event systems
- reflection helpers

Core must not depend on QML.

---

# MVVM Direction

Use small focused ViewModels.

Preferred:

- CanvasViewModel
- PaletteViewModel
- InspectorViewModel
- SelectionViewModel
- DocumentViewModel

Avoid giant god ViewModels.

---

# Document Model

The center of the system is the UI document model.

Example:

Screen
 ├── Widget
 │    ├── properties
 │    ├── children
 │    └── bindings

Editor edits this model.

Runtime renders this model.

---

# Plugin Direction

Plan for plugins early.

Likely future plugin areas:

- custom widgets
- property editors
- runtime integrations
- serialization extensions

Do not pluginize everything.

---

# Anti-Patterns

Avoid:

- Manager hell
- Service locator patterns
- Circular dependencies
- Runtime/editor coupling
- Over-abstraction
- Generic enterprise architecture
- Massive inheritance trees

---

# Refactor Strategy

Phase 1:
- restructure folders
- fix imports
- fix CMake
- preserve behavior

Phase 2:
- document model

Phase 3:
- selection system

Phase 4:
- command/undo system

Phase 5:
- widget registry

Do not rewrite everything at once.
