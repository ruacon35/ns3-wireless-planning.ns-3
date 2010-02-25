/* 
 * File:   netinfo-reader.h
 * Author: patxi
 *
 * Created on 22 December 2009, 16:31
 */

#ifndef _RM_REPORT_READER_H
#define	_RM_REPORT_READER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "ns3/type-id.h"
#include "ns3/object.h"

#include "net-data-struct.h"

namespace ns3 {

using namespace std;

/**
 * @brief A class to read simplified Radio Mobile reports.
 *
 */
class NetinfoReader
{
public:
  /**
   * @brief Reads and parses a netinfo text file .
   *
   * This is the main method of the class.
   *
   * @param file netinfo.txt
   *
   */
  static NetDataStruct::NetData Read(ifstream &file);
private:  
  static vector<string> GetSection (vector<string> &input_lines, string start, string end);
  static vector<string> GetSubSectionNames (vector<string> &input_lines, string header);
  static vector<string> split(const string &s, char delim);
};

} // namespace ns3

#endif
