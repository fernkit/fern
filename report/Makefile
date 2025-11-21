# Makefile for Fern OOP Project Report
# Author: Rishi Ahuja

MAIN = main
PDF = $(MAIN).pdf
TEX = $(MAIN).tex

# LaTeX compiler
LATEX = pdflatex

# Flags for pdflatex
LATEXFLAGS = -interaction=nonstopmode -halt-on-error

# Default target
all: $(PDF)

# Build PDF (run twice for references)
$(PDF): $(TEX)
	$(LATEX) $(LATEXFLAGS) $(TEX)
	$(LATEX) $(LATEXFLAGS) $(TEX)

# Clean auxiliary files
clean:
	rm -f *.aux *.log *.out *.toc *.lof *.lot *.bbl *.blg *.synctex.gz

# Clean everything including PDF
cleanall: clean
	rm -f $(PDF)

# Quick build (single pass)
quick: $(TEX)
	$(LATEX) $(LATEXFLAGS) $(TEX)

# Continuous build with file watching (requires latexmk)
watch:
	latexmk -pdf -pvc $(MAIN)

# Open PDF (macOS)
open: $(PDF)
	open $(PDF)

# Help
help:
	@echo "Fern OOP Project Report Makefile"
	@echo ""
	@echo "Targets:"
	@echo "  all      - Build PDF (default)"
	@echo "  quick    - Quick build (single pass)"
	@echo "  clean    - Remove auxiliary files"
	@echo "  cleanall - Remove all generated files"
	@echo "  open     - Open PDF (macOS)"
	@echo "  watch    - Continuous build (requires latexmk)"
	@echo "  help     - Show this help"

.PHONY: all clean cleanall quick watch open help
