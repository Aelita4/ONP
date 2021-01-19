# ONP

Simple implementation of ONP algorithm in C++ (only works on Windows platform)
## Installation

Use g++ compiler from [GNU Compiler Collection](https://gcc.gnu.org/) package like this: 
```bash
g++ main.cpp -o onp.exe
```

## Usage

You can provide additional arguments, like equation and speed of calculation

If not providen any, program will ask for equation and set all speeds to default (100 ms)

```bash
onp.exe "2+2=" 100 100 100
```

### Notes:
- Equation must be put in colons and end with equation mark (`=`)
- Next three numbers represents wait times inside program (all in miliseconds), use larger number to slow down calculations and analyze how algorithm works
  - first number is responsible for "checking" part of program
  - second for converting equation to ONP format
  - third for actual calculating

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update tests as appropriate.

## License
[MIT](https://choosealicense.com/licenses/mit/)