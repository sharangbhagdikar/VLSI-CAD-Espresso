# VLSI-CAD-Espresso

Reading the input file (*.pcn extension):
1> First line - No of literals in SOP Boolean Expression
2> Second line - No of cubes in SOP
3> Third line:
	i> First number gives number of literals present in current cube
	ii>The rest give the literal number and its sign in the cube

Eg.
4
3
2 -1 2
1 3
4 1 2 3 4

a'b + c + abcd
