# CMake + vcpkg Template

Template base para projetos C++ usando CMake, Ninja e vcpkg no Windows (x64).

## Pré-requisitos

- [Visual Studio](https://visualstudio.microsoft.com/) com o workload **"Desenvolvimento para desktop com C++"** instalado (inclui MSVC, CMake e Ninja).
- [vcpkg](https://github.com/microsoft/vcpkg) instalado localmente. Se ainda não tiver:
```powershell
  git clone https://github.com/microsoft/vcpkg C:\vcpkg
  C:\vcpkg\bootstrap-vcpkg.bat
```

## Como usar este template

1. Clique em **"Use this template"** no GitHub (ou clone normalmente).
2. Renomeie o projeto:
   - No `CMakeLists.txt`, troque `project(MeuProjeto ...)` pelo nome desejado.
   - No `vcpkg.json`, troque o campo `"name"` (minúsculo, sem espaços — apenas letras, números e hífen).
3. Se o seu `vcpkg` estiver instalado em um caminho diferente de `C:/vcpkg`, edite o `VCPKG_ROOT` no `CMakeUserPresets.json`.
4. Adicione as dependências que precisar no array `"dependencies"` do `vcpkg.json`.

## Build

Abra a pasta no Visual Studio (**Arquivo → Abrir → Pasta**) — ele detecta o `CMakePresets.json` automaticamente e já builda usando o preset `default`.

Ou, pelo terminal (com o ambiente MSVC x64 carregado):

```powershell
cmake --preset default
cmake --build build
```

O executável final fica em `build/`.

## Estrutura

.
├── CMakeLists.txt # Definição do projeto e targets
├── CMakePresets.json # Preset base (compilador, generator, toolchain do vcpkg)
├── CMakeUserPresets.json # Caminho local do VCPKG_ROOT (ajustar por máquina, se necessário)
├── vcpkg.json # Dependências do projeto (manifest do vcpkg)
└── src/
└── main.cpp


## Notas

- Dependências do vcpkg são reinstaladas automaticamente a cada `cmake --preset default`, a partir do `vcpkg.json` — não é necessário rodar `vcpkg install` manualmente.