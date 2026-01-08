# Fail the script if cppcheck reports issues
cppcheck `
  --enable=warning,style,performance,portability `
  --error-exitcode=1 `
  --inline-suppr `
  --suppress=missingIncludeSystem `
  -I src/mcal_mock `
  src
