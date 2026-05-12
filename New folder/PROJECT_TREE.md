# Target Project Structure

```text
EverSightDesigner/
│
├── CMakeLists.txt
├── cmake/
│
├── app/
│   ├── main.cpp
│   ├── AppBootstrap.cpp
│   ├── AppBootstrap.h
│   └── AppEnvironment.h
│
├── assets/
│   ├── icons/
│   ├── fonts/
│   ├── themes/
│   └── styles/
│
├── docs/
│   ├── architecture/
│   ├── ui-system/
│   ├── serialization/
│   └── plugin-system/
│
├── src/
│   │
│   ├── core/
│   │   ├── common/
│   │   ├── logging/
│   │   ├── serialization/
│   │   ├── filesystem/
│   │   ├── reflection/
│   │   ├── command/
│   │   ├── undo_redo/
│   │   ├── eventbus/
│   │   └── ids/
│   │
│   ├── runtime/
│   │   ├── engine/
│   │   ├── renderer/
│   │   ├── widgets/
│   │   │   ├── base/
│   │   │   ├── controls/
│   │   │   ├── display/
│   │   │   ├── charts/
│   │   │   └── containers/
│   │   │
│   │   ├── properties/
│   │   ├── bindings/
│   │   ├── screens/
│   │   └── runtime_api/
│   │
│   ├── editor/
│   │   ├── shell/
│   │   ├── canvas/
│   │   ├── palette/
│   │   ├── inspector/
│   │   ├── hierarchy/
│   │   ├── property_system/
│   │   ├── document/
│   │   ├── selection/
│   │   ├── commands/
│   │   ├── dragdrop/
│   │   ├── clipboard/
│   │   └── editor_services/
│   │
│   ├── shared/
│   │   ├── models/
│   │   ├── dto/
│   │   ├── enums/
│   │   ├── constants/
│   │   ├── interfaces/
│   │   └── utils/
│   │
│   ├── plugins/
│   │   ├── interfaces/
│   │   ├── loader/
│   │   └── builtin/
│   │
│   └── qml/
│       ├── components/
│       ├── editor/
│       ├── runtime/
│       └── themes/
│
├── tests/
│   ├── unit/
│   ├── integration/
│   └── ui/
│
├── third_party/
│
└── tools/
    ├── scripts/
    └── devtools/
```

---

# File Placement Rules

Canvas-related code:
src/editor/canvas/

Palette-related code:
src/editor/palette/

Inspector-related code:
src/editor/inspector/

Reusable QML:
src/qml/components/

Editor-specific overlays:
src/qml/editor/

Runtime rendering QML:
src/qml/runtime/

Runtime widgets:
src/runtime/widgets/

Infrastructure:
src/core/

Shared DTO/interfaces/constants:
src/shared/
