#ifndef error_hh
#define error_hh

#include "ansicolor.hh"
#include <string>
#include <optional>
#include <iostream>
#include <iomanip>

#include "types.hh"

namespace nix
{

typedef enum {
    elWarning,
    elError
} ErrLevel;

struct PosInterface
{
    virtual int getLine() const = 0;
    virtual int getColumn() const = 0;
    virtual string getFile() const = 0;
};

class ErrPos
{
public:
   int line;
   int column;
   string file;

   ErrPos& operator=(const PosInterface &pos) {
     line = pos.getLine();
     column = pos.getColumn();
     file = pos.getFile();
     return *this;
   }
   ErrPos(const PosInterface &pos) {
     *this = pos; 
   }
};

class NixCode
{
public:
    ErrPos errPos;
    std::optional<string> prevLineOfCode;
    string errLineOfCode;
    std::optional<string> nextLineOfCode;
};

// ----------------------------------------------------------------
// format function for hints.  same as fmt, except templated values
// are always in yellow.

template <class T>
class yellowify
{
public:
    yellowify(T &s) : value(s) {}
    T &value;
};

template <class T>
std::ostream& operator<<(std::ostream &out, const yellowify<T> &y)
{
    return out << ANSI_YELLOW << y.value << ANSI_NORMAL;
}

class hintformat
{
public:
    hintformat(string format) :fmt(format)
    {
        fmt.exceptions(boost::io::all_error_bits ^ boost::io::too_many_args_bit);
    }
    template<class T>
    hintformat& operator%(const T &value)
    {
        fmt % yellowify(value);
        return *this;
    }

    std::string str() const
    {
        return fmt.str();
    }

    template <typename U>
    friend class AddHint;
private:
    format fmt;
};

std::ostream& operator<<(std::ostream &os, const hintformat &hf);

template<typename... Args>
inline hintformat hintfmt(const std::string & fs, const Args & ... args)
{
    hintformat f(fs);
    formatHelper(f, args...);
    return f;
}

// -------------------------------------------------
// ErrorInfo.
class ErrorInfo
{
public:
    ErrLevel level;
    string name;
    string description;
    std::optional<hintformat> hint;
    std::optional<NixCode> nixCode;

    static std::optional<string> programName;

private:
};

// --------------------------------------------------------
// error printing

// just to cout for now.
void printErrorInfo(const ErrorInfo &einfo);

}

#endif
