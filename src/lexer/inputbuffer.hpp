#pragma once
#include <fstream>   // For file stream operations (e.g., std::ifstream)
#include <iostream>  // For input/output operations (e.g., std::cout, std::cerr)
#include <sstream>   // For string stream operations (e.g., std::stringstream)
#include <stdexcept> // For standard exception handling
#include <string>    // For string manipulation

class InputBuffer {
private:
  std::stringstream buffer;
  std::string file_content;
  const char *beginptr;
  const char *forwardptr;
  const char *endptr;
  int line_num;
  int col_num;

public:
  // Function to read the entire content of a file into a std::string
  void readFileIntoBuffer(const std::string &filePath) {
    // Open the file for reading
    std::ifstream file(filePath);

    // Check if the file was successfully opened
    if (!file.is_open()) {
      // If not, throw a runtime error with a descriptive message
      throw std::runtime_error("Error: Could not open file '" + filePath + "'");
    }

    // Use a stringstream to efficiently read the entire file content
    // rdbuf() returns a pointer to the file's streambuf object,
    // which is then inserted into the stringstream.
    buffer << file.rdbuf();
    // Close the file stream
    file.close();

    file_content = buffer.str();

    // initializing pointers
    beginptr = file_content.c_str();
    forwardptr = file_content.c_str();
    endptr = beginptr + file_content.length();
  }

  char next_char() {
    if (forwardptr >= endptr) {
      return '\0';
    }

    char c = *forwardptr;
    if (c == '\n') {
      line_num++;
      col_num = 1;
    } else {
      col_num++;
    }
    forwardptr++;
    return c;
  }

  // The const qualifier in the function guarantees immutability
  // A read-only function

  char peek_char(int offset = 1) const {
    if (forwardptr + offset - 1 >= endptr) {
      return '\0';
    }
    return *(forwardptr + offset - 1);
  }

  void advance_begin_ptr() { beginptr = forwardptr; }

  bool is_eof() const { return beginptr >= endptr; }

  int get_line_number() const { return line_num; }

  int get_column_number() const { return col_num; }
};
