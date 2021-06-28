.PHONY: all doc main

CXXFLAGS= -DDEBUG -Wall -Werror
LDFLAGS=
CC=mpicxx

OBJDIR=build
SRCDIR=src

OBJS = $(addprefix $(OBJDIR)/,\
				edge.o \
				point.o \
				graph.o \
				main.o \
				node.o \
				mst_algorithm.o \
				mst_prim.o \
				mst_boruvka.o \
				mst_kruskal.o \
                mst_prim_kumar.o \
				union_find.o \
				kmeans.o \
				compare_clustering.o \
			)


main: $(OBJDIR)/main


$(OBJDIR)/main: $(OBJS)	
	$(CC) -g -o $(OBJDIR)/main $(OBJS) $(LDFLAGS)


.SECONDEXPANSION:
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $$(wildcard $(SRCDIR)/$$*.hpp)
	mkdir -p build
	$(CC) -g -c $(CFLAGS) $(filter %.cpp,$^) -o $@



clean:
	# Client cleanup
	rm -rf $(OBJDIR)/*.o
	rm -rf $(OBJDIR)/main

	# valgrind cleanup
	rm -rf *.dSYM

mrproper: clean
	rm -rf doc/
	rm -rf build/

doc:
	# On peut avoir des problèmes en cas d'environnement parallèle
	# si la numéro de build est mis à jour après l'appel à doc.
	#A = $(shell git rev-parse HEAD)
	#A += generation date -- 
	#A += $(shell date +"%Y-%m-%dT%H:%M:%S%z")
	PROJECT_COMMIT_HASH="$(shell echo $$(git rev-parse HEAD) -- gendate $$(date +"%Y-%m-%dT%H:%M:%S%z"))" doxygen Doxyfile
