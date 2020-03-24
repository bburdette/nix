#include "error.hh"

#include <optional>
#include <iostream>

using std::optional;

int main() {

using namespace nix; 

  ColumnRange columnRange;
  columnRange.start = 24;
  columnRange.len = 14;

  ErrLine errline;
  errline.lineNumber = 7;
  errline.columnRange = optional(columnRange);
  errline.errLineOfCode = "line of code where the error occurred";

  NixCode nixcode;
  nixcode.nixFile = optional("myfile.nix");
  nixcode.errLine = errline; 
  
  ErrorInfo generic;
  generic.level = elError;
  generic.name = "error name";
  generic.description = "general error description";
  generic.program = "nixtool.exe";
  generic.nixCode = nixcode;

  print_error(generic);


  StandardError standardError;

  print_error(standardError.name("blah").description("blah"));

  return 0;
}



