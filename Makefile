SOURCE_FILES = helper.cpp tree_gen.tab.c lex.yy.c
.PHONY = all

GVC_FLAGS = $(shell pkg-config --cflags --libs libgvc)
DOT = dot
OUTPUT = tree_gen

all: tree_gen;

flex: tree_gen.l
	flex tree_gen.l

bison: tree_gen.y
	bison -d tree_gen.y

tree_gen: flex bison $(SOURCE_FILES)
	g++ $(SOURCE_FILES) -o $(OUTPUT) -lfl -Werror $(GVC_FLAGS)

debug_tree_gen: flex bison $(SOURCE_FILES)
	g++ -g $(SOURCE_FILES) -o $(OUTPUT) -lfl -Werror $(GVC_FLAGS)

test_png: $(tree_gen)
	echo "[S [NP [Det The] [N cat]] [VP [V chased] [NP [Det the] [N mouse]]]]" | ./$(OUTPUT) | $(DOT) -Tpng -o test_graph.png
