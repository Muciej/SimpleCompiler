# Compiler_JFTT2022
Project created for subject Formal Languages and Translatation Technologies. It is a compiler of simple imperative language that was proposed by lecturer. Compiler generates code for virtual machine with given assembler instructions set.

## Build and usage

Project utilizes cmake, and in order to build it in separate directory I suggest executing following commands:
```
git clone https://github.com/Muciej/Compiler_JFTT2022.git
cd Compiler_JFTT2022
mkdir build
cmake ../
make
```

Then, you can run compiler with:
```
./kompilator input_file output_file debug_file
```

Debug file is optional and if present, will be filled with debug data from the compiler.


