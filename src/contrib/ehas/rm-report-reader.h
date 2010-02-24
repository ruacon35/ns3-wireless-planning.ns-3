/* 
 * File:   rm-report-reader.h
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
class RmReportReader
{
public:
  RmReportReader();
  ~RmReportReader();
  static TypeId GetTypeId(void);

  /**
   * @brief Reads and parses simplified Radio Mobile report.
   *
   * This is the main method of the class.
   *
   * @param file simple-report.txt
   *
   */
  NetDataStruct::NetData ReadRmReport(ifstream &file);
private:  
  vector<string> GetSection (vector<string> &input_lines, string start, string end);
  vector<string> GetSubSectionNames (vector<string> &input_lines, string header);
  vector<string> split(const string &s, char delim);
};

} // namespace ns3

#endif	/* _RM_REPORT_READER_H */
