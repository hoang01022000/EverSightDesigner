# Suggested Codex Prompt

Read:
- AGENTS.md
- PROJECT_TREE.md
- ARCHITECTURE.md

Then:

Refactor ONLY the project structure.

Requirements:
- preserve existing logic
- preserve runtime behavior
- fix imports
- fix CMake
- no feature rewrite
- no unnecessary abstractions
- no redesign

Tasks:
1. Create target folder structure
2. Move files incrementally
3. Update CMake paths
4. Fix QML imports
5. Keep project buildable after each step

Important:
- runtime must never depend on editor
- editor wraps runtime
- avoid manager hell
- avoid service locator patterns
- avoid giant god classes

Do NOT:
- implement plugin system
- redesign runtime engine
- rewrite UI behavior
- add new architecture layers not listed in PROJECT_TREE.md
