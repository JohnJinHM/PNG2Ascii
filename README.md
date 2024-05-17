# PNG2Ascii
**Note: This project is in a very early stage of development for practing purposes, so not quite functional**

**Extra Note: I found the algorithm performing bad on complex images, and the project structure is becoming hard to maintain. Since the basic functions of this project has been implemented, it's development is now concluded.**

Parses a PNG file to generate ASCII-Art style output:

<img src="https://raw.githubusercontent.com/JohnJinHM/PNG2Ascii/main/examples/Sample_6_GS_v0.1.0.png" width="250" height = "280">

GreyScale

* Takes the standard deviation of grey-value((R * 11 + G * 16 + B * 5) / 32 * (A / 255))
* Map tokens(chunks of pixels) according to the standard deviation of pixel percentages of fonts

<img src="https://raw.githubusercontent.com/JohnJinHM/PNG2Ascii/main/examples/Sample_6_BM_v0.2.0.png" width="250" height = "189">


BitMap (Unstable)

* Similar as GreyScale, though compares the grey-value of each "bits" (smaller chunks of pixels) with each font bitmap
* Formula: 0.5+(bit_greyvalue-font_greyvalue_mean)/font_greyvalue_sd/2 (gives a value between 0 and 1 in most cases)
* Requires 8 * 8 bits per token, therefore works bad on small images
* Though it can (occasionally) condense details of larger images into less ascii characters (notice the "tentacle" of sample github logo, represented by the gaps around "!")

### Final comments
I doubt if anyone will ever read this but whatever

This is my first C/C++ project, so the structure and coding habits are horrible. Though it was a good practice for me :P

In total it took around 24 hrs, plus around 3-4 hrs configuring vscode and figure out how things work

The project it self is simply based on the idea of comparing image chunks with fonts to find matches - if you wonder, it doesn't work quite well. It's not like impossible, though C++ is definitely not a good option. 
* Complex images tend to have multiple color gradients and edges types, which would be hard analyze with simple algorithms
* A neural network is probably much effective and easy to implement for this task
* C++ lacks easy-to-use libraries for algorithms and calculations. Takes too much time "Reinventing the wheel"... Why not use Python?

So that's it for this project. It's not even close to perfect, but quite enough for it's simple task, and again it was a good practice.

Thanks for reading all these, have a nice day.

### Credits
https://github.com/dhepper/font8x8 Library for font bitmaps
