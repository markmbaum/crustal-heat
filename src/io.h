#ifndef IO_H_
#define IO_H_

//! \file io.h

#include <string>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <iostream>

//!prints a message then exits the program
void print_exit (const char *msg);

//------------------------------------------------------------------------------
//string manipulation

//!removes white space at the beginning and end of a string
/*!
\param[in] s string to strip of white space
*/
void strip_string (std::string &s);

//!splits a string on the first instance of a specified character
/*!
\param[in] c character to split on
\param[in] s string to split
\param[out] s1 first part of split string
\param[out] s2 second part of split string
*/
void split_string (char c, std::string &s, std::string &s1, std::string &s2);

//!converts an integer into a string
std::string int_to_string (long i);

//------------------------------------------------------------------------------
//writing

//!checks if a file can be written to
void check_file_write (const char *fn);

//!writes an array of doubles to a binary file
/*!
\param[in] fn target file path
\param[in] a array of numbers to write
\param[in] size length of array
*/
void write_double (const char *fn, double *a, long size);

//!writes an array of doubles to a binary file
/*!
\param[in] fn target file path
\param[in] a array of numbers to write
\param[in] size length of array
*/
void write_double (const std::string &fn, double *a, long size);

//!writes an array of doubles to a binary file
/*!
\param[in] fn target file path
\param[in] a vector of numbers to write
*/
void write_double (const char *fn, std::vector<double> a);

//!writes an array of doubles to a binary file
/*!
\param[in] fn target file path
\param[in] a vector of numbers to write
*/
void write_double (const std::string &fn, std::vector<double> a);

//------------------------------------------------------------------------------
//reading

//!checks if a file can be opened for reading
/*!
\param[in] fn path to file to check
*/
void check_file_read (const char *fn);

//!reads a single integer out of a file and into a long type
/*!
\param[in] dir directory of target file
\param[in] fn name of target file
    \return integer found in file
*/
long read_one_long (const std::string &dir, const char *fn);

//!reads a binary file of doubles into an array
/*!
\param[in] dir directory of target file
\param[in] fn name of target file
\param[in] a an array to read numbers into
\param[in] size length of array, number of doubles to read
*/
void read_double (const std::string &dir, const std::string &fn, double *a, long size);

//!reads a settings file into a vector of vectors of strings
/*!
\param[in] fn path to settings file
*/
std::vector< std::vector< std::string > > read_values (const char *fn);

#endif
