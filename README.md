# Patching Documentation
File source : [ https://github.com/JonathanSalwan/binary-samples/tree/master ]
Tool: radare2

 ## Understanding How The File Works
 ### Imports and Exports
 List the imported and exported symbols to get the API surface of the binary
 ~~~
 ii # listing the imported Symbol
is # listing the exported symbols
~~~
### Entry Points
Look for main or _start and trace the initialization path. This tells you how the program boots up
```
ie # looks for _start
```
Found there was no main function using this command
```
 afl | grep main  #filter through the functions looking for main
```
![The main function](/images/image0.png)
 
### Critical  Functions
Focusing on functions that interact with external libraries

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
