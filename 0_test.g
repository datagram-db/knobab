digraph {
rankdir=LR;
	fake1 [style=invisible]
	0 [label="{7, 9, 11, 10, 8}"]
	1 [root=true shape=doublecircle label="{1, 3, 5, 18446744073709551615, 12, 6, 4, 2, 0}"]
	fake1 -> 1 [style=bold]
	0 -> 0 [label=p7]
	0 -> 1 [label=p4]
	0 -> 1 [label=p3]
	0 -> 1 [label=p6]
	0 -> 1 [label=p5]
	0 -> 0 [label=p1]
	0 -> 0 [label=p0]
	0 -> 0 [label=p2]
	0 -> 0 [label=C]
	1 -> 1 [label=p7]
	1 -> 1 [label=p4]
	1 -> 1 [label=p3]
	1 -> 1 [label=p6]
	1 -> 1 [label=p5]
	1 -> 1 [label=p1]
	1 -> 1 [label=p0]
	1 -> 0 [label=p2]
	1 -> 1 [label=C]
}