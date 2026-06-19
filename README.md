# Patching Documentation
File source : [ https://github.com/JonathanSalwan/binary-samples/tree/master ]
Tool: radare2

 ## Understanding How The File Works
 
 Before running the commands the first command to be run should be :
```
aaa
```
 ### Imports and Exports
 List the imported and exported symbols to get the API surface of the binary
```
 ii # listing the imported Symbol
is # listing the exported symbols
```
### Entry Points
Look for main or _start and trace the initialization path. This tells you how the program boots up
```
ie # looks for _start
```
Found there was no main function using this command
```
 afl | grep main  #filter through the functions looking for main
```
![Entry Function](/images/image0.png)
 
### Critical  Functions
These are functions that drive the program's logic, interact with external libraries or handle sensitive data. The techniques include:
#### a) Cross References
To see which functions are heavily referenced. Functions with many callers are often central to program flow
```
axt
```
![](/images/image1.png)
#### b) String References
To list strings to see which functions use them. Functions tied to error messages, protocol names or file paths are usually important
```
iz	#to list strings
axt <string >	# to see which functions use them
```
#### c) Imports Usage
Functions that wrap calls to external libraries are critical glue points. Trace them with:
```
axt sym.imp<function>
```
#### d) Exports
If the binary exports functions, those are meant to be used externally - often key APIs

#### e) Function Size
List all functions. Larger functions ( many instructions ) often contain core logic rather than trival wrappers
```
afl
```
#### g) Control Flow Graphs
Visualize. Complex branching functions are usually critical decision points
```
VV
agf <function>
```
To use the Visualization src.interactive should be set to true
```
e scr.interactive=true
r2 -e scr.interactive=true ./binary #starting r2 with interactive mode enabled
```
![](/images/image2.png)
![](/images/image3.png)

Since I have already done the zoning in into the major sections of the binary, the next step is to decide on the patch to do. There are several ways to go about it by I choose Injection of Custom Logic. These involves several steps as shown below:
### a) Find a Code Cave
Here I am looking for padding alignment gaps or unused sections in .text or other executable segments. Incase no cave exists i can simply add mine to the binary.
```
px #hexdump
iS  #list Sections
```
The two commands are used to search for padding  by spotting long runs of 0x00 or 0xCC and the output is as follows
![](/images/image4.png)
![](/images/image5.png)
The first thing was to chech if the file sizes and the virtual sizes had a difference but from the output we can deduce there was no difference in the sizes thus no code cave. ( This is to be done on the iS output )
On the px output I was looking for long strings of 00 ( null ) or CC ( int3) but as seen in the output it si not there thus no code cave.
For px we can search for large ranges in .text section to see if we can get the indicators of a code cave:
```
px 512 
px 1024
```
Still there was not enough. What is required is about 16-32 contiguous bytes or more. Since I found no cave i decided to fallback to creating my own section and create the cave there
```
perl -e 'print "\x90"x256' > cave bin # creating the binary file to attach
```
Add the section to the elf binary. Though objcopy does not directly attach the new section to an existing segment but rather it only sets section flags
```
objcopy --add-section .cave=cave.bin \
        --set-section-flags .cave=code,alloc,load,readonly \
        elf-Linux-lib-x64.so patched.elf
```
After creating the LIEF file run it to properly add the section to the existing ELF binary
```
g++ LIEF.cpp -o cave -llief
./cave
```
Note: You need to do a download of the  LIEF system library before running the program
```
#Clone the repo

git clone https://github.com/lief-project/LIEF.git
cd LIEF

#Build and Install

mkdir build && cd build
cmake ..
make -j$(nproc)
sudo make install

#Compile the program

g++ LIEF.cpp -o cave -I/usr/local/include -L/usr/local/lib -llief

```
