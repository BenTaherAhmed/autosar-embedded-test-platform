$ErrorActionPreference = "Stop"

cppcheck `
  --enable=warning,style,performance,portability `
  --inline-suppr `
  --suppress=missingIncludeSystem `
  --suppress=unusedFunction `
  -I src/mcal_mock `
  src
