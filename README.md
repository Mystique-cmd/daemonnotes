# Patching Documentation
 The file itself was sourced from : [ https://github.com/JonathanSalwan/binary-samples/tree/master ]

## Function Listing
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
