.PHONY: all

OBJDIR = ../build

all: test_point test_node test_edge test_graph test_union_find test_mst

main.o: main.cpp
	g++ -c -std=c++11 -Wall main.cpp

test_point: test_point.cpp ../build/point.o main.o 
	g++ -std=c++11 -Wall $(OBJDIR)/point.o main.o test_point.cpp -o test_point.o

test_node: test_node.cpp ../build/point.o ../build/node.o main.o
	g++ -std=c++11 -Wall $(OBJDIR)/point.o $(OBJDIR)/node.o main.o test_node.cpp -o test_node.o

test_edge: test_edge.cpp ../build/point.o ../build/node.o ../build/edge.o main.o 
	g++ -std=c++11 -Wall $(OBJDIR)/point.o $(OBJDIR)/node.o $(OBJDIR)/edge.o main.o test_edge.cpp -o test_edge.o

test_graph: test_graph.cpp ../build/point.o ../build/node.o ../build/edge.o ../build/graph.o main.o
	g++ -std=c++11 -Wall $(OBJDIR)/point.o $(OBJDIR)/node.o $(OBJDIR)/edge.o $(OBJDIR)/graph.o main.o test_graph.cpp -o test_graph.o

test_union_find: test_union_find.cpp ../build/point.o ../build/node.o ../build/union_find.o main.o 
	g++ -std=c++11 -Wall $(OBJDIR)/point.o $(OBJDIR)/node.o $(OBJDIR)/union_find.o main.o test_union_find.cpp -o test_union_find.o

test_mst: test_mst.cpp ../build/point.o ../build/node.o ../build/edge.o ../build/graph.o ../build/union_find.o ../build/mst_algorithm.o ../build/mst_prim.o ../build/mst_boruvka.o ../build/mst_kruskal.o 
	g++ -std=c++11 -Wall $(OBJDIR)/point.o $(OBJDIR)/node.o $(OBJDIR)/edge.o $(OBJDIR)/graph.o $(OBJDIR)/union_find.o $(OBJDIR)/mst_algorithm.o $(OBJDIR)/mst_prim.o $(OBJDIR)/mst_boruvka.o $(OBJDIR)/mst_kruskal.o main.o test_mst.cpp -o test_mst.o

clean:
	rm -f *.o