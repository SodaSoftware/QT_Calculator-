A custom graphical calculator built with C++ and Qt, featuring a hand-written mathematical expression parser.

## 🛠 Tech Stack

* **Framework:** Qt5 / Qt6 (QtWidgets, QGridLayout)
* **Language Standard:** C++17
* **Key Components:** `QObject` signal-slot mechanism, dynamic memory management via raw and smart pointers.

## 🧠 Architecture & Algorithmic Design

Unlike basic calculators that evaluate input step-by-step, this project implements a custom expression parser designed to handle complex arithmetic tokenization.

* **Tokenization (`get_list`):** Parses raw UI input strings into a sequential `std::list<QString>`, automatically identifying negative numbers and isolating operators (`+`, `-`, `*`, `/`).
* **Bracket Handling (`math_quation`):** Employs structures to track and store the depth and exact coordinates of open `(` and closed `)` parentheses.
* **Operator Precedence:** Designed to evaluate high-priority operators (`*`, `/`) before low-priority ones (`+`, `-`) across the tokenized chain.
