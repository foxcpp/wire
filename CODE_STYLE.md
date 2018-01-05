# Identation

* Use spaces, not tabs. Tabs should only appear in files that require them for semantic meaning, like Makefiles.

* The indent size is 4 spaces.

  Right:
  ```cpp
  int main()
  {
      return 0;
  }
  ```
  
  Wrong:
  ```cpp 
  int main() 
  {
          return 0;
  }
  ```


* A case label should line up with its switch statement. The case
  statement is **not** indented.

  Right:
  ```cpp 
  switch (condition) {
  case fooCondition:
  case barCondition:
      i++;
      break;
  default:
      i--;
  }
  ```
  
  Wrong:
  ```cpp
  switch (condition) {
      case fooCondition:
      case barCondition:
          i++;
          break;
      default:
          i--;
  }
  ```

* Boolean expressions at the same nesting level that span multiple lines
  should have their operators on the right side of the line.

  Right:
  ```cpp 
  return attribute.name() == srcAttr ||
      attribute.name() == lowsrcAttr ||
      (attribute.name() == usemapAttr && attr->value().string()[0] != '#');
  ```

  Wrong:
  ```cpp 
  return attribute.name() == srcAttr
      || attribute.name() == lowsrcAttr
      || (attribute.name() == usemapAttr && attribute.value().string()[0] != '#');
  ```

# Spacing

* **Do not** place spaces around unary operators.
  
  Right:
  ```cpp
  i++;
  ```

  Wrong:
  ```cpp 
  i ++;
  ```

* **Do** place spaces around binary and ternary operators.

  Right:
  ```cpp 
  y = m * x + b;
  f(a, b);
  c = a | b;
  return condition ? 1 : 0;
  ```

  Wrong:
  ```cpp 
  y=m*x+b;
  f(a,b);
  c = a|b;
  return condition ? 1:0;
  ```

* **Place** spaces around the colon in a range-based for loop.

  Right:
  ```cpp 
  std::vector<plugin_module_info> plugins;
  for (auto& plugin : plugins)
      register_plugin(plugin);
  ```

  Wrong:
  ```cpp 
  std::vector<plugin_module_info> plugins;
  for (auto& plugin: plugins)
      register_plugin(plugin);
  ```

* **Do not** place spaces before comma and semicolon.

  Right:
  ```cpp 
  for (int i = 0; i < 10; ++i)
      do_something();
  
  f(a, b);
  ```

  Wrong:
  ```cpp 
  for (int i = 0 ; i < 10 ; ++i)
      do_something();
  
  f(a , b) ;
  ```
  
* **Place** spaces between control statements and their parentheses.

  Right:
  ```cpp
  if (condition)
      do_it();
  ```

  Wrong:
  ```cpp
  if(condition)
      do_it();
  ```

* Do not place spaces between a function and its parentheses, or between a
  parenthesis and its content.

  Right:
  ```cpp 
  f(a, b);
  ```
  
  Wrong:
  ```cpp 
  f (a, b);
  f( a, b );
  ```


# Line breaking

* Multiple statements _can_ be placed on the same line only if they are small.

  Right:
  ```cpp
  x++; y++;
  if (condition) do();
  ```

  Wrong:
  ```cpp
  very_long_variable_names_is_bad++; they_hurt_more_than_help++;
  if (complex_condition_is_also_a_bad_practice) complex_statements_is_very_bad();
  ```

* An `else` statement should go on the same line as a preceding close brace.

  Right:
  ```cpp
  if (condition) {
      ...
  } else {
      ...
  }
  ```

  Wrong:
  ```cpp
  if (condition) {
      ...
  }
  else {
      ...
  }  
  ```

* An `else if` statement should be written as an `if` statement when the 
  prior `if` concludes with a return statement.

  Right:
  ```cpp
  if (condition) {
      return 0;
  }
  if (condition) {
      ...
  }
  ```

  Wrong:
  ```cpp
  if (condition) {
      return 0;
  } else if (condition) {
      ...
  }
  ```

# Braces

* Place the open brace on the line preceding the code block; place the close
  brace on its own line.

  Right:
  ```cpp 
  int main() {
      ...
  }
  ```
  
  Wrong:
  ```cpp 
  int main()
  {
      ...
  }

  int main 
  { ... }
  ```

* One-line control clauses should use braces.

  Right:
  ```cpp
  if (condition) {
      do_it();
  }

  // You can also write it this way if clause is very small (e.g. return statement).
  if (condition) return -1;
  ```

  Wrong:
  ```cpp
  if (condition) {
      do_it();
  }

  if (condition) pretty_long_statement_blah_blah_blah_blah_blah_blah_blah();
  ```

* Control clauses without a body should **not** use empty braces:

  Right:
  ```
  for (; current; current = current->next);
  ```

  Wrong:
  ```cpp
  for (; current; current = current->next) {}
  ```

# Null, false and zero

* In C++, the null pointer value should be written as `nullptr`. In C, it
  should be written as `NULL`. 

* C++ and C bool values should be written as `true` and `false`.

* Tests for true/false should all be done **without** equality comparisons.

* Test for null/non-null and zero/non-zero should be done **with** equality
  comparisons.

  Right:
  ```cpp
  if (condition) {
      do_it();
  }

  if (pointer == nullptr) {
      do_it();
  }

  if (size == 0) {
      do_it();
  }
  ```

  Wrong:
  ```cpp
  if (condition == true) {
      do_it();
  }

  if (!pointer) {
      do_it();
  }

  if (!size) {
      do_it();
  }
  ```

# Floating point literals

* Unless required in order to force floating point math, do not append `.0`,
  `.f` and `.0f` to floating point literals.

  Right:
  ```cpp 
  const double duration = 60;

  void set(float i);

  set(10);

  const int fps = 12;
  double frame_duration = 1.0 / fps;
  ```

  Wrong:
  ```cpp 
  const double duration = 60.0;

  void set(float i);

  set(10.0f);

  const int fps = 12;
  double frame_duration = 1 / fps; // integer division 
  ```

# Names

* Use snake_case everywhere.

  Right:
  ```cpp
  size_t raw_block_size;
  ```

  Wrong:
  ```
  size_t rawBlockSize;
  ```

* Don't capitalize letters in acronyms.
  
  Right:
  ```cpp
  bool aes_used;
  ```

  Wrong:
  ```cpp
  bool AES_used;
  ```

* Use full words, except in the rare case where an abbreviation would be
  more canonical and easier to understand.

  Right:
  ```cpp 
  size_t char_size; // questionable 
  size_t length;
  int16_t tab_index; // more canonical
  ```

  Wrong:
  ```cpp
  size_t character_size;
  size_t length;
  int16_t tabulation_index; // bizarre
  ```

* Data members **should not** be prefixed by anything like `m_`.

  Right:
  ```cpp
  struct string {
      int16_t length;
  };
  ```

* Avoid getters/setters when possible
  - If data member should be changeable - define as public data member.
  - If data member should be read-only - define as private data member and
    define getter that returns const reference.
  - If data member should be read-only and not changed by implementation code -
    define as const public data member.

  Right:
  ```cpp 
  class foo {
  public:
      int mutable_attribute;
      const int immutable_attribute;

      const int& readonly_attribute() const { 
          return readonly_attribute_;
      }
  private:
      int readonly_attribute_;
  }
  ```

  Wrong:
  ```cpp
  class foo {
  public:
      int mutable_attribute() const { 
          return mutable_attribute_;
      }

      int mutable_attribute(int value) {
          mutable_attribute_ = value;
      }
  private:
      int mutable_attribute_;
  };
  ```

* Use bare words for getters. Getter names should match the names of 
  the variables being gotten.

  Right:
  ```cpp
  class foo {
  public:
      const int& readonly_attribute() const { 
          return readonly_attribute_;
      }
  private:
      int readonly_attribute_;
  };
  ```

  Wrong:
  ```cpp
  class foo {
  public:
      const int& get_readonly_attribute() const { 
          return readonly_attribute_;
      }
  private:
      int readonly_attribute_;
  };
  ```

* **Do not** precede boolean values with words like “is” and “did”.

  Right:
  ```cpp 
  bool valid;
  bool send_data;
  ```

  Wrong:
  ```cpp 
  bool is_valid;
  bool did_sent_data;
  ```

* Prefer `const` to `#define`. Prefer inline functions to macros.

* Use UPPER_CASE for macros.

  Right:
  ```cpp
  #define SOMETHING()
  ```

  Wrong:
  ```cpp
  #define something_()
  ```

* Use #pragma once instead of #define and #ifdef for header guards.

  Right:
  ```cpp
  #pragma once
  
  ...
  ```

  Wrong:
  ```cpp 
  #ifndef LSMP_SECURE_MEMORY_HPP
  #define LSMP_SECURE_MEMORY_HPP

  ...

  #endif
  ```

# Other Punctuation

* If constructor's initializer lust is long then each member (and superclass) 
  should be indented on a separate line, with the colon or comma preceding 
  the member on that line.

  Right:
  ```cpp
  string::string(uint64_t length, char* contents) 
      : container_base(length)
      , another_member(11)
      , another_another_member(11)
      , another_another_another_member(11) {}
  
  // Initalizer list is short.
  string::string() : container_base(0) {}
  ```

  OK:
  ```
  string::string()
      : container_base(0) {}
  ```

  Wrong:
  ```cpp 
  string::string(uint64_t length, char* contents)  : container_base(length), another_member(11), another_another_member(11), another_another_another_member(11) {}
  ```

* Prefer for-each loop over iterator for loop.
  Prefer iterator for loop over index for loop if index value is not used.

  Right:
  ```cpp
  for (auto& item : items) {
      process(item);
  }

  OK:
  ```cpp
  // Index value used in output.
  for (size_t i = 0; i < items.size(); ++i) {
      std::cout << i << ". " << item.name << '\n';
  }
  ```

# Pointer and References

* Both pointer types and reference types should be written with no space
  between the type name and the * or &.

# `#include` Statements

Must go in following order:
* Corresponding header if any (`"secure_memory.hpp"` for `secure_memory.cpp`)
  This guarantees that each header’s completeness is tested. This also assures
  that each header can be compiled without requiring any other header
  files be included first.

* C standard library headers (`<cstring>`)
* C++ standard library headers (`<vector>`)
* 3rd party library headers (`<boost/optional/optional.hpp>`)
* Project headers (`"secure_memory.hpp"`)

# `using` Statements

* In header files, do not use “using” statements in namespace (or global) scope.

* In implementation files, do not use “using” declarations of any kind to
  import names in the standard template library. Directly qualify the names
  at the point they’re used instead.

* In implementation files, put all other “using” statements at the beginning
  of the file, before any namespace definitions and after any
  “include” statements.

# Types

* Omit “int” when using “unsigned” modifier. Do not use “signed” modifier.
  Use “int” by itself instead.

* Use types defined in `<cstdint>` (`uint64_t`, etc) instead of `short` and
  `long`. Use `int` (or `unsigned`) if you don't care about exact size.

* Use a constructor to do an implicit conversion when the argument is
  reasonably thought of as a type conversion and the type conversion is
  fast. Otherwise, use the `explicit` keyword or a function returning the
  type. This only applies to single argument constructors.

  Right:
  ```cpp
  class big_int {
  public:
      big_int(int); // fast conversion from int
      ...
  };

  template<typename T>
  class vector {
      explicit vector(size_t size); // not a type conversion

      tempate<size_t N>
      array<T, N> to_array() const; // costly conversion 
  };
  ```

  Wrong:
  ```cpp
  class big_int {
  public:
      explicit big_int(int); // no reason for explicit
      ...
  };

  template<typename T>
  class vector {
      vector(size_t size); // works like conversion from size_t to vector (WTF?)

      tempate<size_t N>
      operator array<T, N>() const; // costly implicit conversion!
  };
  ```

# Comments 

* Use multi-line comments only for documentation, use single-line comments everywhere else.

* Use Doxygen annotations with backslash.

* Use FIXME: (without attribution) to denote __problems__ that need to be 
  addressed in the future.

* Use TODO: (without attribution) to mark incomplete implemetation.

# Inheritance

* The base level declaration of a virtual method inside a class must be 
  declared with the `virtual` keyword. All subclasses of that class must 
  override virtual method with `virtual` and either `override` or `final`
  keywords.

  Right:
  ```cpp
  class person {
  public:
      virtual void do1();
      virtual void do2();
      virtual void do3();
  };

  class student : public person {
  public:
      virtual void do1() override;
      virtual void do2() final;
      virtual void do3() override;
  };
  ```

  Wrong:
  ```cpp
  class person {
  public:
      virtual void do1();
      virtual void do2();
  };

  class student : public person {
  public:
      void do2() final;
      virtual void do2(); // missing override 
  };
  ```

* Do not use private/protected inheritance. Use composition instead.

