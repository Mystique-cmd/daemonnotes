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
### Cross References
This builds a map of dependencies without need to decode every line

### Assertions and Error Paths
These often reveal assumptions about runtime behaviour

```
aaa #analyze all the things
alf  #list the functions
```

![Functions snippet](/images/image.png)

## Analyzing the Functions
` pdf @ function_name`
### sym.imp.xcb_render_create_picture
![Function code](/images/image1.png)

Used XREFs to list all places where the stub is called
` axt sym.imp.xcb_render_create_picture `
![](/images/image2.png)

Disassembled the call site, to see the logic around it
` pdf @ <address> `
![](/images/image3.png)
