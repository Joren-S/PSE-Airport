//============================================================================
// Author      : Serge Demeyer
// Copyright   : Project Software Engineering - BA1 Informatica - Serge Demeyer - University of Antwerp
//============================================================================

#ifndef PROJECTVLIEGVELD_TESTUTILS_H
#define PROJECTVLIEGVELD_TESTUTILS_H

#include <iostream>

bool DirectoryExists(const std::string dirname);

bool FileExists(const std::string dirname);

bool FileIsEmpty(const std::string filename);

bool FileCompare(const std::string leftFileName, const std::string rightFileName);

std::string ToString(int i);

#endif //PROJECTVLIEGVELD_TESTUTILS_H
