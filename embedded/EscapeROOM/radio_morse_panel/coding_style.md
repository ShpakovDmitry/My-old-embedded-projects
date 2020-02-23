### Coding style guidelines


- This guidelines are used as reminder for my personal purposes and use.
- This coding guidelines are just recommendation and could be violated if code may become way better
to look.
- When contributing to existing group project, it is good practice to use coding style that is
already in use.

##### 1. Local variables and function names starts with lower case and use camelCase
```c
unsigned int start;
unsigned int getLetterPosition(char letter);
```

##### 2. Names representing types must be in mixed case starting with upper case
```c
MorseCode morseCode;
Location point;
```

##### 3. Use underscore *`_`* only in constant names, because they must be all caps
```c
#define MAX_MORSE_PARTS 5
#define MATH_TAU = 6.283185f;
```

##### 4. Do not use negative logic.
```c
bool isVisible;
// instead
bool isNotVisible;
```

##### 5. Do not use super-complex expressions.
This is direct violation of "code must be easy to read" mantra. And sometimes it signals that you are trying to do too many things at once. If you can't get rid of such complex check (and yes, sometimes you just can't), then consider moving it to static inline bool function with meaningful name.

##### 6. Do not use hard-coded magic numbers

##### 7. Reduce the scope of local variables. For example define *`i`* inside *`for`* statement.
```c
for (int i = 0; i < num; i++) {
  // ...
}

if (isClear == true) {
  int value;
  // ...
}
```
##### 8. Use *`i, j, k`* for iterator variables - especially if scope is short
```c
for (int i = 0; i < nElements; i++) {
  // Do something
}
```

##### 9. Placement of opening bracket - on the same line as function name.
```c
unsigned int getLetterPosition(char letter) {
  // ...
}

if (isLarger == true) {
  // ...
}
```

##### 10. Use self-documented code. No need to comment obvious things.

##### 11. Consider using spaces to separate statements from expressions.
```c
if (expression) {
    statement();
}
```

##### Example code
```c
#define MY_MAGIC_CONSTANT 5
const int CONST_A = 6;

void functionName (int paramName) {
    int a = anotherFunction();
    for (int i = 0; i < a; ++i) {
        doThis(i, a, MY_MAGIC_CONSTANT);
    }
    if (a > MY_MAGIC_CONSTANT)
        return;
    while (1) {
        switch (a): {
        case CONST_A:
            doThat();
            break:
        default:
            printf("%s@%d: Unexpected A=%x\n\r", __FUNCTION__, __LINE__, a);
        }
    }
}
```

##### For more information, please visit

[**C++ Programming Style Guidelines**](http://geosoft.no/development/cppstyle.html)






