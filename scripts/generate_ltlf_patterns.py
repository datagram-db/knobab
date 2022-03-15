#!/usr/bin/env python3
import flloat.parser.ltlf
import flloat.ltlf
import os

def serialize(ltlf, file):
	print(str(flloat.parser.ltlf.LTLfParser()(ltlf).to_automaton().to_graphviz()), file=open(file, 'w'))

if __name__ == '__main__':
	serialize("a U b", os.path.join(os.getcwd(), os.pardir, 'data', 'cache', 'graph_pattern_cache', 'Until'))
	serialize("a R b", os.path.join(os.getcwd(), os.pardir, 'data', 'cache', 'graph_pattern_cache', 'Release'))
	serialize("F a", os.path.join(os.getcwd(), os.pardir, 'data', 'cache', 'graph_pattern_cache', 'Diamond'))
	serialize("G a", os.path.join(os.getcwd(), os.pardir, 'data', 'cache', 'graph_pattern_cache', 'Box'))
	serialize("(a U b) | G(b)", os.path.join(os.getcwd(), os.pardir, 'data', 'cache', 'graph_pattern_cache', 'WeakUntil'))
