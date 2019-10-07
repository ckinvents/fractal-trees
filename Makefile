.PHONY: build
build:
	gcc src/vector.c src/tree.c src/leafy-bois.c -lm -lSDL2 -lSDL2main -o build/leafy-bois

cleanup:
	rm build/leafy-bois