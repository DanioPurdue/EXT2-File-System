SOURCES = *.py README Makefile *.sh
TARFILE = lab3b-005229714.tar.gz

default:
	cp lab3b.sh lab3b
	chmod 755 lab3b

clean:
	rm -f $(TARFILE) *~ lab3b

dist:
	tar -cvzf $(TARFILE) $(SOURCES)
	