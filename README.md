PseudoCode
==========

A simple interpreted language written in C.

The design of programming languages is something that is very interesting to me, and so I decided to see what I could build without any prior knowledge on the subject. I chose to build a simple interpreted language in C, figuring out how to build it as I went (next time I will certainly spend time designing the language properly). This project required me to build a simple stack in C, but by far the most difficult thing was figuring out how to integrate mathematical, logical, and comparison operations into a single parser. 

Previously I had coded an infix to postfix converter, and had also written a function to evaluate a postfix expression, so it didn’t take long for me to realize that logical expressions could be processed in this same way. Once that had been figured out, it was a relatively simple matter to add conditions and error checking for switching between the three different types of operations.
