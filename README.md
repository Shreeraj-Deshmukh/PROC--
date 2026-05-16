# PROC++

A C++ transpiler where keywords are renamed to corporate/DNA-inspired terms.  
Proc++ files use the `.proc++` extension and compile down to standard C++ via `g++`.

Feel free to add more mappings that relate to procDNA terminologies — PRs welcome.

---

## HOW TO RUN

**Step 1 — compile the transpiler (only once):**
```
g++ transpiler.cpp -o transpiler.exe
```
Works on g++14. Do this once per terminal session or until the exe is deleted.

**Step 2 — write your code in a `.proc++` file, then run:**
```bash
./transpiler.exe hello.proc++          # bash / MINGW
.\transpiler.exe hello.proc++          # PowerShell (bezos lovers soyboyz club)
```

**Flags:**
```
--show-cpp    print the generated C++ before compiling
--no-run      compile only, don't execute
```

---

## KEYWORD MAP

| proc++        | C++          |   | proc++       | C++        |
|---------------|--------------|---|--------------|------------|
| `procint`     | `int`        |   | `procprint`  | `cout`     |
| `procfloat`   | `float`      |   | `procin`     | `cin`      |
| `procdouble`  | `double`     |   | `procreturn` | `return`   |
| `procstring`  | `string`     |   | `procfor`    | `for`      |
| `procvoid`    | `void`       |   | `procloop`   | `while`    |
| `procauto`    | `auto`       |   | `procif`     | `if`       |
| `nes`         | `bool`       |   | `procelse`   | `else`     |
| `proctrue`    | `true`       |   | `zs`         | `switch`   |
| `procfalse`   | `false`      |   | `procase`    | `case`     |
| `salary`      | `const`      |   | `procbreak`  | `break`    |
| `base`        | `static`     |   | `proclife`   | `continue` |
| `vacant`      | `nullptr`    |   | `proclass`   | `class`    |
| `hire`        | `new`        |   | `memo`       | `struct`   |
| `fire`        | `delete`     |   | `roster`     | `vector`   |
| `promote`     | `push_back`  |   | `blueprint`  | `template` |
| `disclosed`   | `public`     |   | `department` | `namespace`|
| `procvate`    | `private`    |   | `attempt`    | `try`      |
| `procted`     | `protected`  |   | `escalate`   | `catch`    |
| `#onboard`    | `#include`   |   | `report`     | `throw`    |
| `basesalary`  | `main`       |   |              |            |

---

## EXAMPLE

```
#onboard <iostream>
#onboard <vector>

using department std;

procint basesalary() {
    roster<procint> numbers
    numbers.promote(1)
    numbers.promote(2)
    numbers.promote(3)

    procfor (procint i = 0; i < numbers.size(); i++) {
        procprint(numbers[i])
    }

    procreturn 0
}
```

---

## PREPROCESSOR FEATURES

- `procprint(expr)` — shorthand for `cout << expr << "\n";`
- `procin(var)` — shorthand for `cin >> var;`
- Semicolon inference — no need to type `;` at end of most lines

---

debug using claude code.
