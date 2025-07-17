TEX=pdflatex
BIB=bibtex
MAIN=main
SRC=$(wildcard chapters/*.tex) preamble.tex $(MAIN).tex bibliography.bib

all: $(MAIN).pdf

$(MAIN).pdf: $(SRC)
	$(TEX) $(MAIN)
	$(BIB) $(MAIN) || true
	$(TEX) $(MAIN)
	$(TEX) $(MAIN)

clean:
	rm -f *.aux *.log *.out *.toc *.bbl *.blg *.lof *.lot *.pdf
