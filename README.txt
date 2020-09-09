COMP_SCI_4280
Project: P2
BT: MICHAEL SAGO
Language C++

This project is done in the C++ programming language. This program is a scanner
and a PARSER for provided lexical definitions and a certain BNF. For testing 
purposes the scanner/PARSER will be tested using a testing driver that I 
implemented in the file tst_scanner.cpp. I have implemented and included my 
test files for this project. The later test files include more and more code
with loops and if statements.

*I also implemented this program with only spaces not tabs and not spaces at 
the end of each line. Also every token should be seperated by a space   

P2.test1.sp2020 - 
P2.test2.sp2020 - 
P2.test3.sp2020 - 
P2.test4.sp2020 -
P2.test5.sp2020 - 
P2.test6.sp2020 - 
P2.test7.sp2020 - 
P2.test8.sp2020 -
P2.test9.sp2020 - 
P2.test10.sp2020 - 
P2.test11.sp2020 - 

//To compile the program:
		>make frontEnd
//Then you have two options to run the program:
		1.
      	>./frontEnd [filename]
			/* 
				Testing files are included above 
			*/
		2.
      	>./frontEnd
/*
	If using option 2 you will then be able to enter diferent strings or
	integers of your choice but it must fit the BNF. I use a simulated end of 
	file so you should enter either eof or EOF on a line by itself to end your 
	input and the scanner will start going through that input and 
	indicating tokens. At the end it will then tell you if it parsed ok. It will
	also show you a parse tree ouputed to the screen.
*/

