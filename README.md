# Welcome to My Ls
***

## Task
The challenge here is to recreate with some flags (-a, -t, -at) the function lsand that take the directory in count.

## Description
I ceate a programm that will find the flag and the directory and that will sort it all out with the flags and will print it.
For the flag -a: take in count the hidden files .
        flag -t: sort by time.
        flag -at or -ta: do what the two other flags.

## Installation
I make a Makefile with make, make re, fclean etc 

## Usage
First of all the programm will start by finding all the flags and by finding if there are some directory's that we have to print.
For the flags the programm will go through all the characters in the argument to find if there the character '-' that define a flag and will increase to 1 a bit int a byte and just after the programm will search 
if there are any directory(ies) in the argument by going through all the characters if there not the chracter '-' it will take the argument.
So after the flag(s) and directory(ies) has been find the programm will create a list where all the var that are necessary for my_ls just after the programm will start the function flag_a_or_not that will
put in the list the name of the directory(ies) and the file(s) then it will verify if the flag -t is active if yes it will put in the list the time of all the files(s) and directory(ies) if no nothing will pass
then the list will be sort if the flag t is active the list will be sort by time then by the alphabetical order but the flag-t is not active the list will only be sort by the alphabetical order then when the 
sorting is finished all of the director(ies) and file(s) name who has been sort will be print and after that the list will be free.


```
Make
./my_ls option1 ....
```


### The Core Team


<span><i>Made at <a href='https://qwasar.io'>Qwasar SV -- Software Engineering School</a></i></span>
<span><img alt='Qwasar SV -- Software Engineering School's Logo' src='https://storage.googleapis.com/qwasar-public/qwasar-logo_50x50.png' width='20px' /></span>
