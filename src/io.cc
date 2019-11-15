//! \file io.cc

#include "io.h"

void print_exit (const char *msg) {

    //print the given message
    printf("\nFAILURE: %s\n\n", msg);
    //cancel the program
    exit(EXIT_FAILURE);
}

//------------------------------------------------------------------------------
//string manipulation

void strip_string (std::string &s) {
    //do nothing if the string is empty
    size_t idx;
    //remove leading white space
    idx = s.find_first_not_of(" \t\n\r");
    s.erase(0, idx);
    //remove trailing white space
    idx = s.find_last_not_of(" \t\n\r");
    s.erase(idx+1);
}

void split_string (char c, std::string &s, std::string &s1, std::string &s2) {
    size_t idx = s.find(c);
    if (idx == std::string::npos) {
        std::cout << "FAILURE: cannot split string" << s << "on character" << c << std::endl;
        exit(EXIT_FAILURE);
    }
    s1 = s.substr(0, idx);
    s2 = s.substr(idx+1, s.length());
}

std::string int_to_string (long i) {

    // https://www.geeksforgeeks.org/converting-string-to-number-and-vice-versa-in-c/
    std::ostringstream str;
    str << i;
    std::string out = str.str();
    return(out);
}

//------------------------------------------------------------------------------
//writing

void check_file_write (const char *fn) {
    FILE* ofile;
    ofile = fopen(fn, "w");
    if (ofile == NULL) {
        std::cout << "FAILURE: cannot open file " << fn << std::endl;
        exit(EXIT_FAILURE);
    }
    fclose(ofile);
}

void write_double (const char *fn, double *a, long size) {
    FILE* ofile;
    check_file_write(fn);
    ofile = fopen(fn, "wb");
    fwrite(a, sizeof(double), size, ofile);
    fclose(ofile);
}

void write_double (const std::string &fn, double *a, long size) {
    write_double(fn.c_str(), a, size);
}

void write_double (const char *fn, std::vector<double> a) {
    write_double(fn, a.data(), long(a.size()));
}

void write_double (const std::string &fn, std::vector<double> a) {
    write_double(fn.c_str(), a.data(), long(a.size()));
}

//------------------------------------------------------------------------------
//reading

void check_file_read (const char *fn) {
    FILE* ofile;
    ofile = fopen(fn, "r");
    if (ofile == NULL) {
        std::cout << "FAILURE: cannot open file " << fn << std::endl;
        exit(EXIT_FAILURE);
    }
    fclose(ofile);
}

long read_one_long (const std::string &d, const char *fn){

    std::string path = d + "/" + fn;
    check_file_read(path.c_str());
    long num;
    FILE *ifile;
    ifile = fopen(path.c_str(), "r");
    fscanf(ifile, "%li", &num);
    fclose(ifile);
    return(num);
}

void read_double (const std::string &dir, const std::string &fn, double *a, long size) {

    //create path string of file
    std::string path = dir + "/" + fn;
    //check file can be read
    check_file_read(path.c_str());

    FILE *ifile;
    ifile = fopen(path.c_str(), "rb");
    fread(a, sizeof(double), size, ifile);
    fclose(ifile);
}

std::vector< std::vector< std::string > > read_values (const char *fn) {

    std::vector< std::vector< std::string > > iset;

    //open the settings file
    check_file_read(fn);
    std::ifstream ifile(fn); //automatically closed
    //read in settings
    char readline[1000];
    std::string line;
    std::string::size_type idx;
    std::string key, val;
    std::vector< std::string > str;
    while (ifile.getline(readline, 1000)) {
        line = readline;
        strip_string(line);
        idx = line.find('#');
        //ignore empty lines and whole comment lines
        if ( (line.length() > 0)  && (idx != 0) ) {
            //remove partial comments
            if (idx != std::string::npos) line = line.substr(0, idx);
            //split on the equal sign
            split_string('=', line, key, val);
            //strip the key and val
            strip_string(key);
            strip_string(val);
            //create a new "line" in the return object
            str.clear();
            str.push_back(key);
            str.push_back(val);
            iset.push_back(str);
        }
    }

    return(iset);
}
