.PHONY: clean
clean:
	$(MAKE) -C src/ clean

xyz:
	$(MAKE) -C src/ xyz

handin:
	make clean
	tar czvf sat.tgz Makefile src doc/description.pdf doc/*.R doc/*.Rdata doc/description.org res/*
